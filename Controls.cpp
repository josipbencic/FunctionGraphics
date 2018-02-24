#include <cmath>

#include <SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Controls.h"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
  return ViewMatrix;
}

glm::mat4 getProjectionMatrix() {
  return ProjectionMatrix;
}


// Initial position : on +Y
glm::vec3 position = glm::vec3(0, 5, 5);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = -3.14f/3.f;
// Initial Field of View
float initialFoV = 70.0f;

float speed = 5.0f; // 3 units / second
float mouseSpeed = 0.005f;



void computeMatricesFromInputs(SDL_Window* window) {

  // glfwGetTime is called only once, the first time this function is called
  static double lastTime = double(SDL_GetTicks()) / 1000.0;
  SDL_ShowCursor(SDL_DISABLE);

  // Compute time difference between current and last frame
  double currentTime = double(SDL_GetTicks()) / 1000.0;
  float deltaTime = float(currentTime - lastTime);

  // Get mouse position
  int xpos, ypos;
  SDL_GetMouseState(&xpos, &ypos);

  // Reset mouse position for next frame
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  SDL_WarpMouseInWindow(window, w/2, h/2);

  // Compute new orientation
  horizontalAngle += mouseSpeed * float(w/2 - xpos);
  verticalAngle += mouseSpeed * float(h/2 - ypos);

  // Direction : Spherical coordinates to Cartesian coordinates conversion
  glm::vec3 direction(
    cos(verticalAngle) * sin(horizontalAngle),
    sin(verticalAngle),
    cos(verticalAngle) * cos(horizontalAngle)
    );

  // Right vector
  glm::vec3 right = glm::vec3(
    sin(horizontalAngle - 3.14f / 2.0f),
    0,
    cos(horizontalAngle - 3.14f / 2.0f)
    );

  // Up vector
  glm::vec3 up = glm::cross(right, direction);

  SDL_PumpEvents();
  const Uint8* keys = SDL_GetKeyboardState(NULL);


  // Move forward
  if (keys[ SDL_SCANCODE_W ]) {
    position += direction * deltaTime * speed;
  }
  // Move backward
  if (keys[ SDL_SCANCODE_S ]) {
    position -= direction * deltaTime * speed;
  }
  // Strafe right
  if (keys [ SDL_SCANCODE_D ]) {
    position += right * deltaTime * speed;
  }
  // Strafe left
  if (keys[ SDL_SCANCODE_A ]) {
    position -= right * deltaTime * speed;
  }

  float FoV = initialFoV;

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);

  // Camera matrix
  ViewMatrix = glm::lookAt(
    position,               // Camera is here
    position + direction,   // and looks here : at the same position, plus "direction"
    up                      // Head is up (set to 0,-1,0 to look upside-down)
  );

  lastTime = currentTime;
}
