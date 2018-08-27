#include "../Core/Platform.h"

#if MATH_PLAYGROUND > 0

#include <iostream>
#include <iomanip>
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

#include "Math.h"

ostream& operator <<(ostream& stream, const glm::vec3& x) {
  return stream << "(" << x.x << ", " << x.y << ", " << x.z << ")";
}

int main() {

  auto f = [](const vec2& x) { Sphere s; return s(x);  };
  cout << setprecision(2);
  for (float i = -1.0f; i <= 1.0f; i += 1 / 10.0f) {
    for (float j = -1.0f; j <= 1.0f; j += 1 / 10.0f) {
      cout << f(vec2(i,j)) << endl;
    }
  }

  getchar(); getchar();
}


#endif
