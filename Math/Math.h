#ifndef INCLUDED_MATH_H
#define INCLUDED_MATH_H

#include <glm/glm.hpp>

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

struct Sphere {
  inline glm::vec3 operator ()(const glm::vec2& x) const {
    float z = sin(x.y);
    return glm::vec3(
      cos(x.x) * z,
      sin(x.x) * z,
      cos(x.y)
    );
  }
};

struct Circle {
  inline glm::vec2 operator ()(float x) const {
    return glm::vec2( cos(x), sin(x) );
  }
};


#endif
