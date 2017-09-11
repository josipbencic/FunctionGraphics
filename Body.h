#pragma once
#include <GL/glew.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

/*  Physical representation of some mesh
    memset to clear.
*/
template <typename __Collision>
class Body
{
public:

  static const glm::vec3 defaultColor;


  Body(Mesh& mesh)
    :collisionModel(mesh) {
    vertexBuffer = mesh.vertexBuffer;
    verticesNum = mesh.data.size();
    objectColor = glm::vec3(0.73f, 0.37f, 0.43f);
    scale = glm::vec3(1.0f);
    rotation = glm::quat();
    position = glm::vec3(1.0f);
    velocity = glm:: vec3(1.0f);
  }

  Body(const Body<__Collision>& Src)
    : collisionModel(Src.collisionModel) {
    vertexBuffer = Src.vertexBuffer;
    verticesNum = Src.verticesNum;
    objectColor = Src.objectColor;
    scale = Src.scale;
    rotation = Src.rotation;
    position = Src.position;
    velocity = Src.velocity;
  }

  inline void Render(GLuint shader, glm::mat4 view, glm::mat4 projection, glm::vec3 lightColor, std::vector<GLuint> Ids) {
    glUseProgram(shader);

    auto model = ComputeModelMatrix();
    glUniformMatrix4fv( Ids[0], 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv( Ids[1], 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv( Ids[2], 1, GL_FALSE, &projection[0][0]);
    if (Ids[3]) glUniform3fv(Ids[3], 1, &objectColor[0]);
    if (Ids[4]) glUniform3fv(Ids[4], 1, &lightColor[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    if (!Ids[3]) {
      glVertexAttribPointer(
        0,                  // index in layout in shader
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        6 * sizeof(float),  // stride
        (void*)0            // array buffer offset
      );
    }
    else if (Ids[3] > 0) {
      glVertexAttribPointer(
        0,                  // index in layout in shader
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        6 * sizeof(float),  // stride
        (void*)0
      );
      glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        (void*) (3 * sizeof(float))
      );
    }
    glDrawArrays(GL_TRIANGLES, 0, verticesNum/2);
    glDisableVertexAttribArray(0);
  }

  inline void Paint(glm::vec3 color) { objectColor = color; }

  void Translate(glm::vec3& vec) { position += vec; }

  void Scale(glm::vec3& alpha) { scale = alpha; }

  void Scale(float alpha) { scale = glm::vec3(alpha); }

  void Rotate(glm::quat& rot) { rotation = rot; }

  void Rotate(glm::vec3& rot) { rotation = glm::quat(rot); }

  const glm::vec3 Pos() const { return position; }

  const glm::vec3 Scale() const { return scale; }

  __Collision Collision() {
    collisionModel.Update(this);
    return collisionModel;
  }

private:
  __Collision           collisionModel;

  GLuint                vertexBuffer;
  size_t                verticesNum;
  glm::vec3             objectColor;

  glm::vec3             scale;
  glm::quat             rotation;
  glm::vec3             position;

  glm::vec3             velocity;


  inline glm::mat4 ComputeModelMatrix() {
    glm::mat4 scaleMat =  glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rotMat =    glm::mat4(rotation);
    glm::mat4 transMat =  glm::translate(glm::mat4(1.0f), position);
    return transMat * rotMat * scaleMat;
  }

};


template <typename __Collision>
const glm::vec3 Body<__Collision>::defaultColor = glm::vec3(0.73f, 0.37f, 0.43f);

