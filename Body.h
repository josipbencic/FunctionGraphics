#ifndef INCLUDED_BODY_H
#define INCLUDED_BODY_H

#include <vector>
#include <glm/glm.hpp>

#include "./Core/Mesh.h"
#include "./Shapes/Transformable.h"


/*  Physical representation of some mesh
    Shares Mesh among its copies.

    TODO: Remove this
*/
class Body : public Transformable {
  Mesh mesh;

public:
  Body(const std::vector<float>& data)
    : mesh(data) { }

  void Precompute() {
    mesh.BindRenderAttributes();
  }

  inline void Render(glm::mat4 view, glm::mat4 projection) {
    mesh.Render(ComputeModelMatrix(), view, projection);
  }
};

#endif
