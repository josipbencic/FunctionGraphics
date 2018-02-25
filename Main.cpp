
#include <iostream>
#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>

#include <GL/glew.h>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./Core/Display.h"

#include "Body.h"

#include "Functions.h"
using namespace std;
using namespace glm;

void setGLVariables() {
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

  Body floor(CubePoints());
  floor.Precompute();
  floor.Scale(vec3(10.0f, 1.0f, 10.0f));

  do {
    display.BeginFrame();

    mat4 view = display.View();
    mat4 projection = display.Projection();

    floor.Render(view, projection);

  } while (display.FinishFrame());

  return 0;
}


