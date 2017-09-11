
#include <iostream>
#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Controls.h"
#include "Body.h"
#include "Collision.h"

struct SDL_Initer {

  SDL_Window *mainwindow;
  SDL_GLContext maincontext;

  void sdldie(const char *msg)
  {
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  void checkSDLError(int line = -1)
  {
#ifndef NDEBUG
    const char *error = SDL_GetError();
    if (*error != '\0')
    {
      printf("SDL Error: %s\n", error);
      if (line != -1)
        printf(" + line: %i\n", line);
      SDL_ClearError();
    }
#endif
  }

  SDL_Initer(std::string ProgramName, int width, int height) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
      sdldie("Unable to initialize SDL");

    mainwindow = SDL_CreateWindow(ProgramName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!mainwindow)
      sdldie("Unable to create window");

    checkSDLError(__LINE__);
    maincontext = SDL_GL_CreateContext(mainwindow);
    checkSDLError(__LINE__);
  }

  ~SDL_Initer() {
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
  }
};

struct GL_Initer {
  GL_Initer() {
    glewExperimental = true;
    glewInit();

    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  }
};

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if (VertexShaderStream.is_open()) {
    std::string Line = "";
    while (getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }
  else {
    printf("Impossible to open %s.\n", vertex_file_path);
    getchar();
    return 0l;
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if (FragmentShaderStream.is_open()) {
    std::string Line = "";
    while (getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;


  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    printf("%s\n", &VertexShaderErrorMessage[0]);
  }



  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    printf("%s\n", &FragmentShaderErrorMessage[0]);
  }



  // Link the program
  printf("Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  if (InfoLogLength > 0) {
    std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    printf("%s\n", &ProgramErrorMessage[0]);
  }


  glDetachShader(ProgramID, VertexShaderID);
  glDetachShader(ProgramID, FragmentShaderID);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

using B = Body<AABBCollision>;

int main(int, char**) {
  using namespace std;
  using namespace glm;
  const int width = 1024;
  const int height = 768;
  SDL_Initer sdl("Raycaster", width, height);
  GL_Initer glinit;

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  //  create shader and get uniform had
  GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
  GLuint modelMatrixID =      glGetUniformLocation(programID, "model");
  GLuint viewMatrixID =       glGetUniformLocation(programID, "view");
  GLuint projectionMatrixID = glGetUniformLocation(programID, "projection");
  GLuint objectColorID =      glGetUniformLocation(programID, "objectColor");
  GLuint lightColorID =       glGetUniformLocation(programID, "lightColor");

  GLuint light_programID = LoadShaders("SimpleVertexShader.vertexshader", "WhiteFragmentShader.fragmentshader");
  GLuint light_modelMatrixID = glGetUniformLocation(programID, "model");
  GLuint light_viewMatrixID = glGetUniformLocation(programID, "view");
  GLuint light_projectionMatrixID = glGetUniformLocation(programID, "projection");


  //  setup opengl
  glClearColor(0.2f, 0.2f, 0.7f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glDepthFunc(GL_LESS);

  //  create cubes
  auto cubeMesh = Mesh::Cube();
  vector<B> cubeBodies;
  cubeBodies.push_back(B(cubeMesh));
  //cubeBodies.push_back(B(cubeMesh));
  //cubeBodies[1].Translate(vec3(-4.0f, 0.0f, 4.0f));

  //B floor = B(cubeMesh);
  //floor.Translate(vec3(-5.0f, -10.0f, -5.0f));
  //floor.Scale(vec3(50.0f, 0.1f, 50.0f));
  //floor.Paint(vec3(0.2f, 0.55f, 0.55f));

  B light(cubeMesh);
  light.Translate(vec3(0.0f, 5.0f, 0.0f));
  light.Scale(0.25f);

  SDL_Event e;
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    computeMatricesFromInputs(sdl.mainwindow);
    glm::mat4 view = getViewMatrix();
    glm::mat4 projection = getProjectionMatrix();


    //  light
    light.Render(light_programID, view, projection, vec3(0.0f), { light_modelMatrixID, light_viewMatrixID, light_projectionMatrixID, 0u, 0u });
    auto lightColor = vec3(1.0f);

    // bodies

    //floor.Render(programID, view, projection, lightColor, { modelMatrixID, viewMatrixID, projectionMatrixID, objectColorID, lightColorID });
    //cubeBodies[0].Translate(vec3(-0.001f, 0.0f, 0.001f));
    for (auto& c : cubeBodies) {
      c.Render(programID, view, projection, lightColor, { modelMatrixID, viewMatrixID, projectionMatrixID, objectColorID, lightColorID });
    }

    for (auto& c : cubeBodies) {
      //if (Physics::Resolve(c.Collision(), floor.Collision())) {
      //  c.Paint(vec3(1.0f, 0.0f, 0.0f));
      //}
      //else {
      //  c.Paint(B::defaultColor);
      //}

      for (auto& c2 : cubeBodies) {
        if (&c == &c2) {
          continue;
        }
        if (Physics::Resolve(c.Collision(), c2.Collision())) {
          c.Paint(vec3(1.0f, 0.0f, 0.0f));
          c2.Paint(vec3(1.0f, 0.0f, 0.0f));
        }
        else {
          c.Paint(B::defaultColor);
          c2.Paint(B::defaultColor);
        }
      }
    }

    SDL_GL_SwapWindow(sdl.mainwindow);
    SDL_PollEvent(&e);
  } while ( e.type != SDL_QUIT );

  glDeleteVertexArrays(1, &VertexArrayID);
  glDeleteProgram(programID);
  return 0;
}


