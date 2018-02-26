#ifndef INCLUDED_DISPLAY_H
#define INCLUDED_DISPLAY_H

#include <string>

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/glm.hpp>

class Display {

  // Initial position : on +Y
  const glm::vec3 startingPosition = glm::vec3(0, 5, -5);

  // Initial Field of View
  static constexpr float initialFoV = 70.0f;

  // 5 units / second
  static constexpr float speed = 5.0f;
  static constexpr float mouseSpeed = 0.005f;

  // Initial horizontal angle
  float horizontalAngle = -3.14f / 2.0f;
  // Initial vertical angle
  float verticalAngle = -3.14f / 6.0f;

  //  Current position
  glm::vec3 position = startingPosition;

  SDL_Window    *mainWindow;
  SDL_GLContext  mainContext;

  SDL_Event e;

  glm::mat4 view;
  glm::mat4 projection;

public:
  /*  Initializes SDL and GL. Should be called at the start of the program. */
  Display(std::string programName, int width = 1920, int height = 1080);

  /*  Deinitalizies SDL and GL. Should be called at the end of the program. */
  ~Display();

  void BeginFrame();

  bool FinishFrame();

private:

  /*  Also sets some global attributes for SDL. Configure them inside. */
  void InitSDL(std::string programName, int width, int height);

  /*  This functions sets GL version and similar attributes. */
  void InitGL();

  /*  Reads errors from SDL error buffer. */
  void CheckSDLError(unsigned debugLine);

  /*  Force kill, used if initialization fails. Exits whole program with value 1. */
  void KillSDL(const char* msg);

  /*  Graceful exit; called on dtor. */
  void QuitSDL();

  /*  Converts mouse input to orientation. Must be called before UpdateMovement! */
  void UpdateOrientation();

  /*  Polls keyboard key state and updates position based on what keys are pressed. */
  void Move(float deltaTime, glm::vec3 direction, glm::vec3 rightVector);

  /*  */
  void UpdateMovement(float deltaTime);

  /* Trivial getters. */
public:

  inline glm::mat4 View() const {
    return view;
  }

  inline glm::mat4 Projection() const {
    return projection;
  }

  inline SDL_Window* Window() const {
    return mainWindow;
  }
};



#endif
