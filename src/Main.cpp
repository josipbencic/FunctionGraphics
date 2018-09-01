#include <iostream>
#include <cstdlib>
#include <ostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <iomanip>
using namespace std;

/*
int main(int, char**) {

 glm::mat4 L = { { 1, 0, 0, 0 },{ 0, 1, 0, 0 },{ 1 / 4.0, -1 / 16.0, 0, 0},{ -3 / 4.0, -1 / 16.0, -1 / 81.0, 0 } };
 glm::mat4 R = { { 12, -18, 0, -12 },{ 0, -8, -19, -2 },{ 0, 0, 243.0 / 16, 105.0 / 8 },{ 0,0,0, -328.0 / 108 } };
 auto a = L * R;
 cout << std::setprecision(5) << std::fixed << std::showpos;
 for (int i = 0; i < 16; i++) {
   cout << a[i / 4][i % 4] << " ";
   if (i % 4 == 3) cout << endl;
 }

 getchar();
 return 0;
}
*/

#include "Core/Platform.h"
#include "Core/Display.h"
#include "Core/Mesh.h"
#include "Math/Math.h"
#include "Math/RealFunctions.h"
#include "Shapes/Manifold.h"
using namespace mth;

int main() {
  Display display;

  auto f = [](R2 x) -> R {
    return x.x * x.x - x.y * x.y;
  };


  auto S = Surface<>{
    graph(f), R2(-1.0, -1.0), R2(1.0, 1.0), Colors::GREY
  };

  S.Scale(5);
  S.Translate({ 5.0, 0.0, 0.0 });

  auto rectangle = [](R2 x) -> R3 {
    return { x.x, 0.0f, x.y };
  };

  auto sphere = [](R2 x) -> R3 {
    return { x.x * cosf(x.y), x.x * sinf(x.y), sqrt(1 - x.x*x.x) };
  };

  auto body = [](R2 x) -> R3 {
    return { x.x, x.y, sqrt(x.x*x.x + x.y*x.y) };
  };

  auto floor = Surface<>{
    rectangle, { -5.0, -5.0 }, { 5.0, 5.0 }
  };

  auto S1 = Surface<>{
    sphere, { 0.0, 4.0 }, {}
  };

  do {
    display.BeginFrame();
    display << floor << S;
  } while (display.FinishFrame());
  return 0;

}
