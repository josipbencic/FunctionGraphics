#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include <GL/glew.h>

#include <vector>

#include <glm/glm.hpp>

struct Colors {
  static const glm::vec3 BLUE;
  static const glm::vec3 GREY;
  static const glm::vec3 PINK;
  static const glm::vec3 GREEN;
};


/*  Mesh used here is much broader structure than usually in 3D applications.
    In short, Mesh contains almost all communication done with GL.
    Shader is only used here, so that the code on higher level doesn't need
    to tackle its details. There is only one shader for all meshes.
    Mesh can contain vertices, color, normals, indices...; everything is inside one VAO.
    This does not scale well, but there will always be less than 3 meshes.
    Mesh is initialized with the data that is sent to GPU.
    Mesh assignement and copying is WEAK.

    It is used as an abstraction for a set of points that are rendered on the screen.
    The color, shading and physics are not important above this level.
*/
class Mesh
{
  GLuint VAOid;
  GLuint VBOid;

  GLsizei numVertices;

  struct Shader {
    static constexpr char* vertexShader = "SimpleVertexShader.vertexshader";
    static constexpr char* fragmentShader = "SimpleFragmentShader.fragmentshader";

    GLuint id;
    GLuint modelID;
    GLuint viewID;
    GLuint projectionID;
    GLuint colorID;

    Shader();
    ~Shader();
    Shader(const Shader& src) = default;
    Shader& operator =(const Shader& rhs) = default;

    /*  Loads, compiles and links shaders. */
    GLuint BuildShaders(
      const char* vertex_file_path,
      const char* fragment_file_path);
  };

  /*  The same shader is used for all meshes. */
  Shader shader;

  /*  ATM, the whole mesh is painted in one color. */
  glm::vec3 color = glm::vec3(0.37f, 0.73f, 0.43f);

  /*  Will be turned on when a Mesh is initialized with vertex data. */
  bool bMeshSpecified;

public:
  /* Only creates VAO. Vertex buffer still needs to be specifed. */
  Mesh();

  /*  Creates both VAO and VBO with the specified data. */
  Mesh(const std::vector<float>& data);

  ~Mesh();

  /*  Mesh copying is WEAK.
      New copies only receive a reference to already-existing
      VAO and VBOs. Other fields are copied by value.
      If the source has no VAO, nothing happens.
  */
  Mesh(const Mesh&);

  /*  WEAK assignment.
      Assignement from a RHS with no specifed vertices does nothing.
      Otherwise, references are copied.
  */
  const Mesh& operator =(const Mesh&);

  /*  When using move semantics, if there is no vertex data in the
      source, copying is ignored.
      Otherwise, ownership of the VAO is translated to
      this, while the source is flagged as if it doesn't have specified mesh.
  */
  Mesh(Mesh&&);
  const Mesh& operator =(Mesh&&);

  /*  Used to specify data if it wasn't originially supplied. */
  void SpecifyVertices(const std::vector<float>& data);

  /*  Sets VAO attributes before render. */
  void BindRenderAttributes();

  /*  Sends uniforms and makes a draw call.
      Assumes the mesh vertices have been setup.
  */
  void Render(
    const glm::mat4& model,
    const glm::mat4& view,
    const glm::mat4& projection) const;

  /*  Set mesh color. */
  inline void Paint(glm::vec3 c) {
    color = c;
  }

private:

  /*  Allocates a specified vertex set on VBO. */
  void AllocGPU(const std::vector<float>& mesh);

  /* */
  void copyFrom(const Mesh& src);

};

#endif
