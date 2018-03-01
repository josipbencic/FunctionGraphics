#ifndef INCLUDED_MANIFOLD_H
#define INCLUDED_MANIFOLD_H

#include <glm/glm.hpp>

#include "../Core/Mesh.h"
#include "../Shapes/Transformable.h"


#include <cmath>



class Manifold : public Transformable {
protected:

  Mesh mesh;

  Manifold() = default;
  Manifold(const Manifold&) = default;

public:

  inline void Precompute() {
    mesh.BindRenderAttributes();
  }

  inline void Render(const glm::mat4& view, const glm::mat4 projection) {
    mesh.Render(ComputeModelMatrix(), view, projection);
  }
};


/*  Func is a mathematical function from R2 to R3.
    Usually std::function<vec3(vec2)>, but it needn't be.
*/
template <typename Func, unsigned Resolution = 100>
class Surface : public Manifold {
public:
  Surface(
    const Func& f,
    glm::vec2 from = glm::vec2(0.0f, 0.0f),
    glm::vec2 to = glm::vec2(1.0f, 1.0f)) {

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
    mesh.SpecifyVertices(v);
  }
};


template <typename Func, unsigned Resolution = 1000>
class Curve : public Manifold {
public:
  Curve(const Func& f, float from = 0.0f, float to = 1.0f) {

    const float res = static_cast<float>(Resolution);

    std::vector<float> v;
    for (int i = 0; i <= Resolution; i++) {
      float x = static_cast<float>(i) / res;
      glm::vec3 p = f(x);
      v.push_back(p.x);
      v.push_back(p.y);
      v.push_back(p.z);
    }

    mesh.SpecifyVertices(v);
  }
};


#endif
