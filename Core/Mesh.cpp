#include "Mesh.h"

#include <iostream>
#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>

#include <SDL.h>
#include <SDL_opengl.h>


Mesh::Mesh() {
  glGenVertexArrays(1, &VAOid);
  glBindVertexArray(VAOid);

  bMeshSpecified = false;
}

Mesh::Mesh(const std::vector<float>& data) {
  glGenVertexArrays(1, &VAOid);
  glBindVertexArray(VAOid);

  AllocGPU(data);
  numVertices = static_cast<GLsizei>(data.size());

  bMeshSpecified = true;
}

Mesh::Shader::Shader() {
  id = LoadShaders(Shader::vertexShader, Shader::fragmentShader);
  modelID = glGetUniformLocation(id, "model");
  viewID = glGetUniformLocation(id, "view");
  projectionID = glGetUniformLocation(id, "projection");
  colorID = glGetUniformLocation(id, "objectColor");
}

Mesh::Shader::~Shader() {
  glDeleteProgram(id);
}

Mesh::~Mesh() {
  if (bMeshSpecified) {
    glBindVertexArray(VAOid);
    glDeleteBuffers(1, &VBOid);
  }

  glBindVertexArray(VAOid);
  glDeleteVertexArrays(1, &VAOid);
}

void Mesh::SpecifyVertices(std::vector<float>& data) {
  if (bMeshSpecified) {
    return;
  }
  AllocGPU(data);
  numVertices = static_cast<GLsizei>(data.size());
  bMeshSpecified = true;
}

void Mesh::BindRenderAttributes() {
  glBindVertexArray(VAOid);

  glBindBuffer(GL_ARRAY_BUFFER, VBOid);
  glVertexAttribPointer(
    0,                  // index in layout in shader
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );
  glEnableVertexAttribArray(0);
}

void Mesh::Render(
  glm::mat4 model, glm::mat4 view, glm::mat4 projection) {

  glUseProgram(shader.id);
  glUniformMatrix4fv(shader.modelID, 1, GL_FALSE, &model[0][0]);
  glUniformMatrix4fv(shader.viewID, 1, GL_FALSE, &view[0][0]);
  glUniformMatrix4fv(shader.projectionID, 1, GL_FALSE, &projection[0][0]);
  glUniform3fv(shader.colorID, 1, &color[0]);

  // glBindVertexArray(VAOid); // only call if there are multiple VAOs

  glDrawArrays(GL_TRIANGLES, 0, (GLsizei)numVertices);
}

void Mesh::AllocGPU(const std::vector<float>& mesh) {
  glBindVertexArray(VAOid);
  glGenBuffers(1, &VBOid);
  glBindBuffer(GL_ARRAY_BUFFER, VBOid);
  glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), mesh.data(), GL_STATIC_DRAW);
}

GLuint Mesh::Shader::LoadShaders(
  const char* vertex_file_path,
  const char* fragment_file_path) {

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

