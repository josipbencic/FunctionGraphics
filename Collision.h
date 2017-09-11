#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include <iostream>

#include "Body.h"

//  No collision
struct NoCollision {
  NoCollision(Mesh&) { }
  NoCollision(const NoCollision&) { }
  void Update(Body<NoCollision>*) { }
};

//  AABB represented by a center and "radius" for each axis
struct AABBCollision {

  //  radius is fixed for certain mesh
  AABBCollision(Mesh& mesh) {
    r = mesh.bounds;
  }

  AABBCollision(const AABBCollision& Src) {
    r = Src.r;
    c = Src.c;
  }

  //  only position is updated
  void Update(Body<AABBCollision>* b) {
    c = b->Pos();
    r = r * (b->Scale());
  }

  glm::vec3 c;
  glm::vec3 r;
};


namespace Physics {

  using namespace std;  ///  REMOVE THIS LATER

  //  if not implemented -> don't collide at all
  template <typename CollisionA, typename CollisionB>
  bool Resolve(CollisionA A, CollisionB B) {
    cout << __PRETTY_FUNCTION__ << endl;
    return false;
  }

  //  if any side has NoCollision, nothing happens
  template <typename CollisionA>
  bool Resolve(CollisionA A, NoCollision B) {
    cout << __PRETTY_FUNCTION__ << endl;
    return false;
  }

  template <typename CollisionB>
  bool Resolve(NoCollision A, CollisionB B) {
    cout << __PRETTY_FUNCTION__ << endl;
    return false;
  }

  //  AABB vs AABB detect
  bool Resolve(AABBCollision a, AABBCollision b) {
    if (abs(a.c[0] - b.c[0]) > (a.r[0] + b.r[0])) return 0;
    if (abs(a.c[1] - b.c[1]) > (a.r[1] + b.r[1])) return 0;
    if (abs(a.c[2] - b.c[2]) > (a.r[2] + b.r[2])) return 0;
    return 1;
  }

}
