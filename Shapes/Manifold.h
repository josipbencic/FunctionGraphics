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


template <typename Func>
class Surface : public Manifold {
public:
  Surface(
    const Func& f,
    glm::vec2 from = glm::vec2(0.0f, 0.0f),
    glm::vec2 to = glm::vec2(1.0f, 1.0f)) {

    std::vector<float> v;
    for (int i = 0; i <= 100; i++) {
      for (int j = 0; j <= 100; j++) {
        float x = (static_cast<float>(i) / 100.0f) * (to.x - from.x) + from.x;
        float y = (static_cast<float>(j) / 100.0f) * (to.y - from.y) + from.y;
        glm::vec3 p = f(glm::vec2(x, y));
        v.push_back(p.x);
        v.push_back(p.y);
        v.push_back(p.z);
      }
    }
    mesh.SpecifyVertices(v);
  }
};


template <typename Func>
class Curve : public Manifold {
public:
  Curve(const Func& f) {
    std::vector<float> v;
    for (int i = 0; i <= 100; i++) {
      float x = static_cast<float>(i) / 100.0;
      glm::vec3 p = f(x);
      v.push_back(p.x);
      v.push_back(p.y);
      v.push_back(p.z);
    }

    mesh.SpecifyVertices(v);
  }
};


#endif
