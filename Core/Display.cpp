#include "Display.h"


#include "Platform.h"

#include <iostream>
#include <cstdlib>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Shapes/Manifold.h"


Display::Display(std::string programName, int width, int height) {
  InitSDL(programName, width, height);
  InitGL();
}

Display::~Display() {

  QuitSDL();
}


void Display::BeginFrame() {

  //  Clear the screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Compute time difference between current and last frame
  static double lastTime = double(SDL_GetTicks()) / 1000.0;
  double currentTime = double(SDL_GetTicks()) / 1000.0;
  float deltaTime = float(currentTime - lastTime);

  UpdateOrientation();
  UpdateMovement(deltaTime);

  lastTime = currentTime;
}

bool Display::FinishFrame() {

  SDL_GL_SwapWindow(mainWindow);
  SDL_PollEvent(&e);

  if (e.type == SDL_QUIT) {
    return false;
  }

  if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
    return false;
  }

  return true;
}

void Display::InitSDL(std::string programName, int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    KillSDL("Unable to initialize SDL");
  }

  mainWindow = SDL_CreateWindow(
    programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
  );

  if (mainWindow == nullptr) {
    KillSDL("Unable to create window");
  }

  CheckSDLError(__LINE__);
  mainContext = SDL_GL_CreateContext(mainWindow);
  CheckSDLError(__LINE__);
}

void Display::InitGL() {
  glewExperimental = true;
  glewInit();

  //  Use OpenGL 3.1 core
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
}

void Display::CheckSDLError(unsigned line) {
#ifndef NDEBUG
  const char *error = SDL_GetError();
  if (*error != '\0') {
    printf("SDL Error: %s\n", error);
    if (line != -1) {
      printf(" + line: %i\n", line);
    }
    SDL_ClearError();
  }
#endif
}

void Display::KillSDL(const char* msg) {
  printf("%s: %s\n", msg, SDL_GetError());
  SDL_Quit();
  exit(1);
}

void Display::QuitSDL() {
  SDL_GL_DeleteContext(mainContext);
  SDL_DestroyWindow(mainWindow);
  SDL_Quit();
}

void Display::UpdateOrientation() {
  SDL_ShowCursor(SDL_DISABLE);

  // Get mouse position
  int xpos, ypos;
  SDL_GetMouseState(&xpos, &ypos);

  // Reset mouse position for next frame
  int w, h;
  SDL_GetWindowSize(mainWindow, &w, &h);
  SDL_WarpMouseInWindow(mainWindow, w / 2, h / 2);

  // Compute new orientation
  horizontalAngle += mouseSpeed * float(w / 2 - xpos);
  verticalAngle += mouseSpeed * float(h / 2 - ypos);
}

void Display::Move(float deltaTime, glm::vec3 direction, glm::vec3 right) {

  SDL_PumpEvents();
  const Uint8* keys = SDL_GetKeyboardState(NULL);

  // Move forward
  if (keys[SDL_SCANCODE_W]) {
    position += direction * deltaTime * speed;
  }
  // Move backward
  if (keys[SDL_SCANCODE_S]) {
    position -= direction * deltaTime * speed;
  }
  // Strafe right
  if (keys[SDL_SCANCODE_D]) {
    position += right * deltaTime * speed;
  }
  // Strafe left
  if (keys[SDL_SCANCODE_A]) {
    position -= right * deltaTime * speed;
  }
}

void Display::UpdateMovement(float deltaTime) {

  // Direction : Spherical coordinates to Cartesian coordinates conversion
  glm::vec3 direction(
    cos(verticalAngle) * sin(horizontalAngle),
    sin(verticalAngle),
    cos(verticalAngle) * cos(horizontalAngle)
  );
  glm::vec3 right = glm::vec3(
    sin(horizontalAngle - 3.14f / 2.0f),
    0,
    cos(horizontalAngle - 3.14f / 2.0f)
  );
  glm::vec3 up = glm::cross(right, direction);

  Move(deltaTime, direction, right);

  // Projection : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  projection = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);

  // Camera matrix
  view = glm::lookAt(
    position,               // Camera is here
    position + direction,   // and looks here : at the same position, plus "direction"
    up                      // Head is up (set to 0,-1,0 to look upside-down)
  );
}

const Display& Display::operator<<(const Manifold& m) const {

  m.mesh.Render(m.ComputeModelMatrix(), view, projection);
  return *this;
}

