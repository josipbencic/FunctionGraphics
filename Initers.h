#pragma once

#include <iostream>
#include <cstdlib>

#include <GL/glew.h>


#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

using u = unsigned long long;
using fl = float;
using vf = std::vector<fl>;
#define pb push_back


std::ostream& operator <<(std::ostream& stream, glm::vec3 out) {
  stream << "(" << out.x << ", " << out.y << ", " << out.z << ")";
  return stream;
}

std::ostream& operator <<(std::ostream& stream, glm::quat out) {
  stream << "(" << out.x << ", " << out.y << ", " << out.z << ", " << out.w << ")";
  return stream;
}

struct SDL_Initer {

  SDL_Window *mainwindow;
  SDL_GLContext maincontext;

  void sdldie(const char *msg)
  {
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  void checkSDLError(int line = -1)
  {
#ifndef NDEBUG
    const char *error = SDL_GetError();
    if (*error != '\0')
    {
      printf("SDL Error: %s\n", error);
      if (line != -1)
        printf(" + line: %i\n", line);
      SDL_ClearError();
    }
#endif
  }

  SDL_Initer(std::string ProgramName, int width, int height) {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
      sdldie("Unable to initialize SDL");

    mainwindow = SDL_CreateWindow(ProgramName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (!mainwindow)
      sdldie("Unable to create window");

    checkSDLError(__LINE__);
    maincontext = SDL_GL_CreateContext(mainwindow);
    checkSDLError(__LINE__);
  }

  ~SDL_Initer() {
    SDL_GL_DeleteContext(maincontext);
    SDL_DestroyWindow(mainwindow);
    SDL_Quit();
  }
};

struct GL_Initer {
  GL_Initer() {
    glewExperimental = true;
    glewInit();

    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  }
};

