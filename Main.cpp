
#include <iostream>
#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>


#include <GL/glew.h>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./Core/Display.h"

#include "Shaders.h"
#include "Collision.h"
#include "Body.h"

#include "Functions.h"
using namespace std;
using namespace glm;

void setupOpenGL() {
  //  setup opengl
  glClearColor(0.2f, 0.2f, 0.7f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  //glDepthFunc();
  //glEnable(GL_PROGRAM_POINT_SIZE);
  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable(GL_POINT_SPRITE);
  //glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

int main(int, char**) {
  Display display("Graphics");


  //  create shader and get uniform had
  GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
  GLuint modelMatrixID =      glGetUniformLocation(programID, "model");
  GLuint viewMatrixID =       glGetUniformLocation(programID, "view");
  GLuint projectionMatrixID = glGetUniformLocation(programID, "projection");
  GLuint objectColorID =      glGetUniformLocation(programID, "objectColor");
  
  //  setup floor
  auto cubePoints = CubePoints();
  auto cubeMesh = Mesh(cubePoints);
  auto floor = Body<NoCollision>(cubeMesh);

  floor.Scale(vec3(10.0f, 1.0f, 10.0f));
  floor.setupRender();

  //  ==========

  func::Poly p{ { 0.25f, -1.0f, 1.0f } };

  func::PointContainer funcMesh(func::CreatePoints<func::Poly>(5, p));
  func::SimpleRenderer<func::FunctionInputReaction> r(funcMesh);

  r.Paint(vec3(0.0f, 0.2f, 0.0f));
  r.setupRender();

  auto m = r.ComputeModelMatrix();

  do {
    display.BeginFrame();

    glm::mat4 view = display.View();
    glm::mat4 projection = display.Projection();

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

  } while (display.FinishFrame());

  Mesh::clearVAO();
  func::PointContainer::clearVAO();
  glDeleteProgram(programID);
  return 0;
}


