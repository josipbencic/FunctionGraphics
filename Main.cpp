#include "./Core/Platform.h"

#if WINDOW_APP > 0


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
#include "./Shapes/Manifold.h"
#include "./Math/Math.h"

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


  Surface<Sphere> S{ Sphere(), vec2(0.0), vec2(2 * 3.1415, 2 * 3.1415) };
  S.Scale(3.0f);
  S.Precompute();

  do {
    display.BeginFrame();

    mat4 view = display.View();
    mat4 projection = display.Projection();

    S.Render(view, projection);

  } while (display.FinishFrame());

  return 0;
}
#endif
