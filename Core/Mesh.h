#ifndef INCLUDED_MESH_H
#define INCLUDED_MESH_H

#include <GL/glew.h>

#include <vector>

#include <glm/glm.hpp>

/*  Mesh used here is much broader structure than usually in 3D applications.
    Shader is only used here, so that the code on higher level doesn't need
    to tackle its details. There is only one shader for all meshes.
    Mesh can contain vertices, color, normals, indices...
    The whole mesh is inside one VAO.
    This does not scale well, but there will always be less than 3 meshes.
    Mesh is initialized with the data that is sent to GPU.

    In short, Mesh contains almost all communication done with GL.
    It is used as an abstraction for a set of points that are rendered
    on the screen. The color, shading and physics are not important above this level.
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

    /*  Loads, compiles and links shaders. */
    GLuint LoadShaders(
      const char* vertex_file_path,
      const char* fragment_file_path);
  };

  /*  The same shader is used for all meshes. */
  Shader shader;

  /*  Currently, the whole mesh is painted in one color. */
  const glm::vec3 color = glm::vec3(0.37f, 0.73f, 0.43f);;

  /*  Will be turned on when a Mesh is initialized with function or explicit vertex data. */
  bool bMeshSpecified = false;

public:
  /* Only creates VAO. Vertex buffer still needs to be specifed. */
  Mesh();

  /*  Creates both VAO and VBO with the specified data. */
  Mesh(const std::vector<float>& data);

  ~Mesh();

  /*  There will not be multiple meshes for now. */
  Mesh(const Mesh&) = delete;
  Mesh(const Mesh&&) = delete;
  const Mesh& operator =(const Mesh&) = delete;
  const Mesh& operator =(const Mesh&&) = delete;

  /*  Used to specify data if it wasn't originially supplied. */
  void SpecifyVertices(const std::vector<float>& data);

  /*  Sets VAO attributes before render. */
  void BindRenderAttributes();

  /*  Sends uniforms and makes a draw call. */
  void Render(glm::mat4 model, glm::mat4 view, glm::mat4 projection);

private:

  /*  Allocates a specified vertex set on VBO. */
  void AllocGPU(const std::vector<float>& mesh);
};

inline std::vector<float> CubePoints() {
  return std::vector<float>{
     -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, 0.5f, -0.5f,
     0.5f, 0.5f, -0.5f,
     -0.5f, 0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     -0.5f, 0.5f, 0.5f,
     -0.5f, -0.5f, 0.5f,
     -0.5f, 0.5f, 0.5f,
     -0.5f, 0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, 0.5f,
     -0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, 0.5f,
     0.5f, -0.5f, 0.5f,
     -0.5f, -0.5f, 0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, 0.5f, -0.5f,
     0.5f, 0.5f, -0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f, 0.5f,
     -0.5f, 0.5f, 0.5f,
     -0.5f, 0.5f, -0.5f,
  };
}

#endif
