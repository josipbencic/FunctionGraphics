
#include <iostream>
#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>


#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Initers.h"

#include "Controls.h"
#include "Shaders.h"
#include "Collision.h"
#include "Body.h"

#include "Functions.h"

int main(int, char**) {
  using namespace std;
  using namespace glm;
  const int width = 1024;
  const int height = 768;
  SDL_Initer sdl("Raycaster", width, height);
  GL_Initer glinit;


  //  create shader and get uniform had
  GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
  GLuint modelMatrixID =      glGetUniformLocation(programID, "model");
  GLuint viewMatrixID =       glGetUniformLocation(programID, "view");
  GLuint projectionMatrixID = glGetUniformLocation(programID, "projection");
  GLuint objectColorID =      glGetUniformLocation(programID, "objectColor");

  //  setup opengl
  glClearColor(0.2f, 0.2f, 0.7f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glDepthFunc();
  //glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_POINT_SPRITE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);


  func::Poly p{ {1.0f, 1.0f, 1.0f, 5.0f} };

  func::PointContainer funcMesh(func::CreatePoints<func::Poly>(100, p));
  func::SimpleRenderer r(funcMesh);

  r.Paint(vec3(0.0f, 0.2f, 0.0f));
  //r.Translate(vec3(0.0f, 1.2f, 0.0f));
  r.Scale(vec3(1.0f, 1.0f, 1.0f));
  r.setupRender();

  //auto model = r.ComputeModelMatrix();
  //glm::mat4 view = getViewMatrix();
  //glm::mat4 projection = getProjectionMatrix();
  //cout << "Points to draw are:\n";
  //for (int i = 0; i < v.size(); i++) {
  //  auto res = projection * view * model * vec4(v[i], 1);
  //  cout << res.x << " " << res.y << " " << res.z << endl;
  //}

  auto cubePoints = CubePoints();
  auto cubeMesh = Mesh(cubePoints);
  auto floor = Body<NoCollision>(cubeMesh);
  floor.Scale(vec3(10.0f, 2.0f, 10.0f));
  floor.setupRender();


  SDL_Event e;
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    computeMatricesFromInputs(sdl.mainwindow);
    glm::mat4 view = getViewMatrix();
    glm::mat4 projection = getProjectionMatrix();

    floor.Render(programID, view, projection, vector<GLuint>{
      modelMatrixID,
        viewMatrixID,
        projectionMatrixID,
        objectColorID
    });


    r.Render(programID, view, projection,
      modelMatrixID,
      viewMatrixID,
      projectionMatrixID,
      objectColorID
    );







    SDL_GL_SwapWindow(sdl.mainwindow);
    SDL_PollEvent(&e);
  } while ( e.type != SDL_QUIT );

  Mesh::clearVAO();
  func::PointContainer::clearVAO();
  glDeleteProgram(programID);
  return 0;
}


