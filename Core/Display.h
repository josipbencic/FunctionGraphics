#ifndef INCLUDED_DISPLAY_H
#define INCLUDED_DISPLAY_H

#include <string>

#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

class Display {

  SDL_Window    *mainWindow;
  SDL_GLContext  mainContext;

public:
  Display(std::string programName, int width = 1920, int height = 1080);

  inline SDL_Window* Window() const {
    return mainWindow;
  }

  ~Display();

private:

  void InitSDL(std::string programName, int width, int height);

  /*  This functions sets GL version and similar attributes. */
  void InitGL();

  /*  Reads errors from SDL error buffer. */
  void CheckSDLError(unsigned debugLine);

  /*  Force kill, used if initialization fails. Exits whole program with value 1. */
  void KillSDL(const char* msg);

  /*  Graceful exit; called on dtor. */
  void QuitSDL();
};



#endif
