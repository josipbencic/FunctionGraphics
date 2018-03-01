#include "./Core/Platform.h"

#if WINDOW_APP > 0

#include <iostream>
#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <functional>

#include <GL/glew.h>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./Core/Display.h"
#include "./Shapes/Manifold.h"
#include "./Math/Math.h"

using namespace std;
using namespace mth;



int main(int, char**) {
  Display display;

  auto f = [] (R2 x) -> R {
    return x.x * x.x - x.y * x.y;
  };

  auto S = Surface<> {
    graph(f), R2(-1.0, -1.0), R2(1.0, 1.0), Colors::GREY
  };

  S.Scale(5);
  S.Translate({ 5.0, 0.0, 0.0 });

  auto rectangle = [] (R2 x) -> R3 {
    return { x.x, 0.0f, x.y };
  };

  auto floor = Surface<>{
    rectangle, { -5.0, -5.0 }, { 5.0, 5.0 }
  };

  auto g = [](R x) {
    return sin(1.0 / x);
  };

  auto F = Curve<>{
    graph(g), 0.4f, 100.0f,
  };
  F.Scale(2);

  do {
    display.BeginFrame();

    mat4 view = display.View();
    mat4 projection = display.Projection();

    floor.Render(view, projection);
    S.Render(view, projection);
    F.Render(view, projection);

  } while (display.FinishFrame());
  return 0;
}

#endif
