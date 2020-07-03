#include "base/RectRenderComponent.hpp"
#include "base/GameObject.hpp"

RectRenderComponent::RectRenderComponent(GameObject & gameObject, Uint8 r, Uint8 g, Uint8 b):
  RenderComponent(gameObject),
  mR(r),
  mG(g),
  mB(b)
{
}

void
RectRenderComponent::render(SDL_Renderer * renderer) const
{
  const GameObject & gameObject = getGameObject();
  SDL_Rect fillRect = { int(gameObject.x()), int(gameObject.y()), int(gameObject.w()), int(gameObject.h()) };
  SDL_SetRenderDrawColor(renderer, mR, mG, mB, 0xFF);
  SDL_RenderFillRect(renderer, &fillRect);
}
