#ifndef INCLUDED_MANIFOLD_H
#define INCLUDED_MANIFOLD_H

#include <glm/glm.hpp>

#include "../Core/Mesh.h"
#include "../Shapes/Transformable.h"

#include <functional>
#include <cmath>



/*  A base class for all shapes. Contains all information needed
    to make a draw call. Classes that inherit from Manifold need
    only to handle mathematics.
*/
class Manifold : public Transformable {

  friend class Display;

protected:

  Mesh mesh;

  Manifold() = default;
  Manifold(const Manifold&) = default;
};


/*  Func is a mathematical function from R2 to R3.
    Usually std::function<vec3(vec2)>, but it needn't be.
*/
template <
  typename Func = std::function<glm::vec3(glm::vec2)>,
  unsigned Resolution = 200
>
class Surface : public Manifold {
public:
  Surface(
    const Func& f,
    glm::vec2 from = glm::vec2(0.0f, 0.0f),
    glm::vec2 to = glm::vec2(1.0f, 1.0f),
    glm::vec3 color = Colors::GREEN
  ) {

    auto vertices = CreateVertices(f, from, to);
    mesh.SpecifyVertices(vertices);
    mesh.Paint(color);
    mesh.BindRenderAttributes();
  }

private:

  std::vector<float> CreateVertices(const Func& f, glm::vec2 from, glm::vec2 to) {
    const float res = static_cast<float>(Resolution);
    std::vector<float> v;

    for (int i = 0; i <= Resolution; i++) {
      for (int j = 0; j < Resolution; j++) {
        double x = (static_cast<double>(i) / res) * (to.x - from.x) + from.x;
        double y = (static_cast<double>(j) / res) * (to.y - from.y) + from.y;
        glm::vec3 p = f(glm::vec2(x, y));

        v.push_back(p.x);
        v.push_back(p.y);
        v.push_back(p.z);
      }
      double x = (static_cast<double>(i) / res) * (to.x - from.x) + from.x;
      glm::vec3 p = f(glm::vec2(x, to.y));
      v.push_back(p.x);
      v.push_back(p.y);
      v.push_back(p.z);
    }
    return v;
  }
};


template <
  typename Func = std::function<glm::vec3(float)>,
  unsigned Resolution = 1000
>
class Curve : public Manifold {
public:
  Curve(
    const Func& f,
    float from = 0.0f,
    float to = 1.0f,
    glm::vec3 color = Colors::GREY
  ) {
    auto vertices = CreateVertices(f, from, to);
    mesh.SpecifyVertices(vertices);
    mesh.Paint(color);
    mesh.BindRenderAttributes();
  }

private:
  std::vector<float> CreateVertices(const Func& f, float from = 0.0f, float to = 1.0f) {
    const float res = static_cast<float>(Resolution);

    std::vector<float> v;
    for (int i = 0; i <= Resolution; i++) {
      float x = static_cast<float>(i) / res;
      glm::vec3 p = f(x);
      v.push_back(p.x);
      v.push_back(p.y);
      v.push_back(p.z);
    }
    return v;
  }
};


#endif
