#pragma once

#include <iostream>
#include <vector>
#include <utility>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"

namespace func {

  using fl = float;
  using u = unsigned long long;
#define pb push_back
  using vf = std::vector<fl>;
  using vff = std::vector<std::pair<fl, fl>>;

  struct Polynomial
  {
    vf a;

    inline fl operator() (fl x) const
    {
      fl t = 1;
      fl ret = 0.0f;
      for (u i = 0; i < deg(); i++) {
        ret += a[i] * t;
        t *= x;
      }
      return ret;
    }

    inline u deg() const { return a.size(); }
  };

  template <class Poly> inline u deg(Poly p) { return p.deg(); }
  using Poly = Polynomial;

  template <typename Function>
  inline std::vector< glm::vec3 > CreatePoints(u n, Function& f)
  {
    std::vector< glm::vec3 > ret;
    fl amount = static_cast<fl>(n);
    for (fl t = 0.0f; t < 1.0f; t += 1.0f / amount) {
      ret.pb(glm::vec3(t, f(t), 0.0f));
    }
    ret.pb(glm::vec3(1.0f, f(1.0f), 0.0f));
    return ret;
  }


  struct PointContainer
  {
    std::vector<glm::vec3> data;
    GLuint vertexBuffer;

    static GLuint VertexArrayID;

    static void setupVAO()
    {
      glGenVertexArrays(1, &VertexArrayID);
      glBindVertexArray(VertexArrayID);
    }

    static void clearVAO()
    {
      glDeleteVertexArrays(1, &VertexArrayID);
    }

    PointContainer(std::vector<glm::vec3>& data)
      : data(data)
    {
      if (VertexArrayID == 777) {
        PointContainer::setupVAO();
      }
      glBindVertexArray(VertexArrayID);

      glGenBuffers(1, &vertexBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
      glBufferData(GL_ARRAY_BUFFER, (data.size()) * 3 * sizeof(float), data.data(), GL_STATIC_DRAW);
    }

    ~PointContainer()
    {
      glBindVertexArray(VertexArrayID);
      glDeleteBuffers(1, &vertexBuffer);
    }

  };

  GLuint PointContainer::VertexArrayID = 777;

  struct SimpleRenderer
  {
    glm::vec3             scale;
    glm::quat             rotation;
    glm::vec3             position;

    GLuint                vertexBuffer;
    size_t                verticesNum;
    glm::vec3             objectColor;

    SimpleRenderer(PointContainer& points)
    {
      vertexBuffer = points.vertexBuffer;
      verticesNum = points.data.size();
      rotation = glm::quat(1.0f, -1.0f, 0.0f, 1.0f);
      position = glm::vec3(0.0f);
      objectColor = glm::vec3(0.73f, 0.37f, 0.43f);
      scale = glm::vec3(1.0f);
    }

    inline void setupRender()
    {
      glBindVertexArray(PointContainer::VertexArrayID);

      glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
      glVertexAttribPointer(
        0,                  // index in layout in shader
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
      );
      glEnableVertexAttribArray(0);
    }

    inline void Render(
      GLuint shader, glm::mat4 view, glm::mat4 projection,
      GLuint modelID, GLuint viewID, GLuint projectionId, GLuint colorID)
    {
      glUseProgram(shader);
      auto model = ComputeModelMatrix();
      glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
      glUniformMatrix4fv(viewID, 1, GL_FALSE, &view[0][0]);
      glUniformMatrix4fv(projectionId, 1, GL_FALSE, &projection[0][0]);
      glUniform3fv(colorID, 1, &objectColor[0]);

      glBindVertexArray(PointContainer::VertexArrayID);

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDrawArrays(GL_POLYGON, 0, (GLsizei)verticesNum);
    }

    inline glm::mat4 ComputeModelMatrix() {
      glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
      glm::mat4 rotMat = glm::mat4(rotation);
      glm::mat4 transMat = glm::translate(glm::mat4(1.0f), position);
      return transMat * rotMat * scaleMat;
    }

    //  random accessors

    inline void Paint(glm::vec3 color) { objectColor = color; }

    void Translate(glm::vec3& vec) { position += vec; }

    void Scale(glm::vec3& alpha) { scale = alpha; }

    void Scale(float alpha) { scale = glm::vec3(alpha); }

    void Rotate(glm::quat& rot) { rotation = rot; }

    void Rotate(glm::vec3& rot) { rotation = glm::quat(rot); }

    const glm::vec3 Pos() const { return position; }

    const glm::vec3 Scale() const { return scale; }
  };
}

