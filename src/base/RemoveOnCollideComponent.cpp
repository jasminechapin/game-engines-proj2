#include "base/RemoveOnCollideComponent.hpp"
#include "base/Level.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

RemoveOnCollideComponent::RemoveOnCollideComponent(GameObject &gameObject,
		int tag, Mix_Chunk *sound, int soundChannel) :
		GenericComponent(gameObject), mTag(tag), collideSound(sound), channel(
				soundChannel) {
}

void RemoveOnCollideComponent::collision(Level &level,
		std::shared_ptr<GameObject> obj) {
	if (obj->tag() == mTag) {
		level.removeObject(obj);
		channel = Mix_PlayChannel(-1, collideSound, 0);
	}
}
