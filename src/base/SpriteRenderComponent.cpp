#include "base/SpriteRenderComponent.hpp"
#include "base/GameObject.hpp"
#include <SDL.h>
#include <vector>

SpriteRenderComponent::SpriteRenderComponent(GameObject &gameObject,
		std::vector<SDL_Texture*> textures) :
		RenderComponent(gameObject), playerTextures(textures) {
}

void SpriteRenderComponent::render(SDL_Renderer *renderer) const {
	const GameObject &gameObject = getGameObject();
	SDL_Rect SrcR;
	SDL_Rect DestR;

	SrcR.x = 0;
	SrcR.y = 0;
	SrcR.w = 100;
	SrcR.h = 100;

	DestR.x = gameObject.x();
	DestR.y = gameObject.y();
	DestR.w = gameObject.w();
	DestR.h = gameObject.h();

	SDL_RenderCopy(renderer, playerTextures[spriteToRender], &SrcR, &DestR);
}

void SpriteRenderComponent::setSprite(int newSprite) {
	spriteToRender = newSprite;
}

int SpriteRenderComponent::getSprite() {
	return spriteToRender;
}
