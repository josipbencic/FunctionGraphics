add_executable(graphics
  Core/Display.cpp
  Core/Display.h
  Core/Mesh.cpp
  Core/Mesh.h
  Core/Platform.h
  Main.cpp
  Math/MathPlayground.cpp
  Math/Math.h
  Math/RealFunctions.h
  PlaygroundMain.cpp
  Shapes/Manifold.cpp
  Shapes/Manifold.h
  Shapes/Transformable.h
)

# Not portable! TODO: Make FindSDL work.
set(SDL2_LIBRARY C:\\lib\\SDL2-2.0.8\\lib\\x86\\SDL2.lib)
set(SDL2_INCLUDE_DIR C:\\lib\\SDL2-2.0.8\\include)
set(GLEW_LIBRARY C:\\lib\\glew-2.0.0\\lib\\Release\\Win32\\glew32.lib)
set(GLEW_INCLUDE_DIR C:\\lib\\glew-2.0.0\\include)

find_package(OpenGL REQUIRED)

# Link and include SDL2, glew and openGL
target_include_directories(graphics PRIVATE ${SDL2_INCLUDE_DIR}) 
target_link_libraries(graphics ${SDL2_LIBRARY})
target_include_directories(graphics PRIVATE ${GLEW_INCLUDE_DIR}) 
target_link_libraries(graphics ${GLEW_LIBRARY})
target_include_directories(graphics PRIVATE ${OPENGL_INCLUDE_DIRS}) 
target_link_libraries(graphics ${OPENGL_LIBRARIES})

# include submodule glm (header-only)
target_include_directories(graphics PRIVATE ../glm)

# C++14
target_compile_features(graphics PUBLIC cxx_constexpr)

# add warnings
if(MSVC)
  add_compile_options(/W3 /WX)
else()
  add_compile_options(-W -Wall -Werror)
endif()

# create shader directory in the binary directory
if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/shaders/")
  file (MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/shaders/")
  message ("Generating Shader directory")
endif ()

# copy shaders in the binary directory
add_custom_command(
  TARGET graphics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/shaders/SimpleFragmentShader.fragmentshader ${CMAKE_CURRENT_BINARY_DIR}/shaders
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/shaders/SimpleVertexShader.vertexshader ${CMAKE_CURRENT_BINARY_DIR}/shaders 
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/shaders/WhiteFragmentShader.fragmentshader ${CMAKE_CURRENT_BINARY_DIR}/shaders 
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/shaders/WhiteVertexShader.vertexshader ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

# copy dlls to targets (not that great solution - 64-bit case missing)
add_custom_command(
  TARGET graphics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${GLEW_INCLUDE_DIR}/../lib/Release/Win32/glew32.dll ${CMAKE_CURRENT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_INCLUDE_DIR}/../lib/x86/SDL2.dll ${CMAKE_CURRENT_BINARY_DIR}
)
