#include "Display.h"


#include "Platform.h"

#include <iostream>
#include <cstdlib>


Display::Display(std::string programName, int width, int height) {
  InitSDL(programName, width, height);
  InitGL();
}

Display::~Display() {

  QuitSDL();
}

void Display::InitSDL(std::string programName, int width, int height) {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    KillSDL("Unable to initialize SDL");
  }

  mainWindow = SDL_CreateWindow(
    programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

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
