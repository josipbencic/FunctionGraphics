#pragma once

#include <vector>

inline GLuint allocGPU(std::vector<float>& mesh);

inline std::vector<float> CubeMesh();

struct Mesh
{

  static GLuint VertexArrayID;

  static void setupVAO() {
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
  }

  static void clearVAO() {
    glBindVertexArray(VertexArrayID);
    glDeleteVertexArrays(1, &VertexArrayID);
  }

  Mesh(std::vector<float>& mesh)
    : data(mesh) {
    vertexBuffer = allocGPU(mesh);
  }

  ~Mesh()
  {
    glBindVertexArray(VertexArrayID);
    glDeleteBuffers(1, &vertexBuffer);
  }

  GLuint vertexBuffer;
  std::vector<float> data;
  glm::vec3 bounds;
};

GLuint Mesh::VertexArrayID = 888;


inline GLuint allocGPU(std::vector<float>& mesh) {
  if (Mesh::VertexArrayID == 888) {
    Mesh::setupVAO();
  }
  glBindVertexArray(Mesh::VertexArrayID);
  GLuint vertexBuffer = 777;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, (mesh.size()) * sizeof(float), mesh.data(), GL_STATIC_DRAW);
  return vertexBuffer;
}


inline std::vector<float> CubePoints() {
  return std::vector<float>{
     -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, 0.5f, -0.5f,
     0.5f, 0.5f, -0.5f,
     -0.5f, 0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     -0.5f, 0.5f, 0.5f,
     -0.5f, -0.5f, 0.5f,
     -0.5f, 0.5f, 0.5f,
     -0.5f, 0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, 0.5f,
     -0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     -0.5f, -0.5f, 0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, 0.5f, -0.5f,
     0.5f, 0.5f, -0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     -0.5f, 0.5f, 0.5f,
     -0.5f, 0.5f, -0.5f,
  };
}
