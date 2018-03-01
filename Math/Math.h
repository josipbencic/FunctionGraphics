#ifndef INCLUDED_MATH_H
#define INCLUDED_MATH_H

#include <glm/glm.hpp>

namespace mth {

  using R3 = glm::vec3;
  using R2 = glm::vec2;
  using R = float;
  using glm::mat4;

  template <class F, class G>
  auto compose(F f, G g) {
    return [f, g](auto x) { return f(g(x)); };
  }

  template <class F, class G>
  auto operator *(F f, G g) {
    return compose(f, g);
  }

  template <class F0, typename... F>
  auto compose(F0 f, F&&... fs) {
    return f * compose(fs...);
  }

  auto graph(const std::function<R(R2)>& f) {
    return [&f](R2 x) -> R3 {
      return { x.x, f(x), x.y };
    };
  }

  auto graph(const std::function<R(R)>& f) {
    return [&f](R x) -> R3 {
      return { x, f(x), x };
    };
  }

  struct Sphere {
    inline R3 operator ()(R2 x) const {
      float z = sin(x.y);
      return {
        cos(x.x) * z,
        sin(x.x) * z,
        cos(x.y)
      };
    }
  };

  struct Circle {
    inline R2 operator ()(R x) const {
      return { cos(x), sin(x) };
    }
  };
}

#endif
