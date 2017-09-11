#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL.h>
#include <glm/glm.hpp>

void computeMatricesFromInputs(SDL_Window* windows);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif
