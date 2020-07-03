#include "InputManager.hpp"
#include <algorithm>
#include <math.h>
#include <iostream>

InputManager &
InputManager::getInstance()
{
  static InputManager * instance = new InputManager();
  return *instance;
}

void
InputManager::startUp()
{
}

void
InputManager::shutDown()
{
}

void
InputManager::resetForFrame()
{
  mKeysPressed.clear();
}

void
InputManager::handleEvent(const SDL_Event & e)
{
  if (e.type == SDL_KEYDOWN) {
    auto elem = mKeysDown.find(e.key.keysym.sym);
    if (elem == mKeysDown.end()) {
      mKeysPressed.insert(e.key.keysym.sym);
    }
    mKeysDown.insert(e.key.keysym.sym);
  } else if (e.type == SDL_KEYUP) {
    mKeysDown.erase(e.key.keysym.sym);
  }
}

std::pair<int, int>
InputManager::getMouseGridPosition(float size) {
  SDL_GetMouseState( &mMouseCoordinateX, &mMouseCoordinateY );
  return std::make_pair(trunc(mMouseCoordinateX / size), trunc(mMouseCoordinateY / size));
}

bool
InputManager::isKeyDown(SDL_Keycode k) const
{
  return mKeysDown.find(k) != mKeysDown.end();
}

bool
InputManager::isKeyPressed(SDL_Keycode k) const
{
  return mKeysPressed.find(k) != mKeysPressed.end();
}

bool 
InputManager::isHovering(float x, float y, const float size)
{
  bool withinX = mMouseCoordinateX >= x && mMouseCoordinateX < x + size;
  bool withinY = mMouseCoordinateY >= y && mMouseCoordinateY < y + size;
  return withinX && withinY;
}

bool
InputManager::isMouseDown() {
  return mMouseDown;
}

float
InputManager::mouseX()
{
  return mMouseCoordinateX;
}

float
InputManager::mouseY()
{
  return mMouseCoordinateY;
}