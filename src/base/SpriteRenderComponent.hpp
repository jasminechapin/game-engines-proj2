#ifndef BASE_SPRITE_RENDER_COMPONENT
#define BASE_SPRITE_RENDER_COMPONENT

#include "base/RenderComponent.hpp"
#include <SDL.h>
#include <vector>

//! \brief Handles rendering a game object with a sprite.
class SpriteRenderComponent: public RenderComponent {
public:

	SpriteRenderComponent(GameObject &gameObject, std::vector<SDL_Texture*> textures);

	virtual void render(SDL_Renderer *renderer) const override;

	void setSprite(int newSprite);

	int getSprite();

private:
	std::vector<SDL_Texture*> playerTextures;
	int spriteToRender { 0 };
};

#endif /* BASE_SPRITE_RENDER_COMPONENT */
