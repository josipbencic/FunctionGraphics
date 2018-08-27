#ifndef INCLUDED_TRANSFORMABLE_H
#define INCLUDED_TRANSFORMABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformable {
private:

  glm::vec3 scale;
  glm::quat rotation;
  glm::vec3 position;

public:
  Transformable(
    glm::vec3 scale = glm::vec3(1.0f),
    glm::quat rotation = glm::quat(),
    glm::vec3 position = glm::vec3(0.0f)
  ) : scale(scale), rotation(rotation), position(position) { }

  inline glm::mat4 ComputeModelMatrix() const {
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rotMat = glm::mat4_cast(rotation);
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), position);
    return transMat * rotMat * scaleMat;
  }

  inline void Translate(const glm::vec3& vec) {
    position += vec;
  }

  inline void Scale(const glm::vec3& alpha) {
    scale = alpha;
  }

  inline void Scale(float alpha) {
    scale = glm::vec3(alpha);
  }

  inline void Rotate(const glm::quat& rot) {
    rotation = rot;
  }

  inline void Rotate(const glm::vec3& rot) {
    rotation = glm::quat(rot);
  }

  inline const glm::vec3 Pos() const {
    return position;
  }

  inline const glm::vec3 Scale() const {
    return scale;
  }
};

#endif
