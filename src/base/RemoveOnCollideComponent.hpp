#ifndef BASE_REMOVE_ON_COLLIDE_COMPONENT
#define BASE_REMOVE_ON_COLLIDE_COMPONENT

#include "base/GenericComponent.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

//! \brief A component that removes a game object (of a given tag) on collision.
class RemoveOnCollideComponent: public GenericComponent {
public:

  RemoveOnCollideComponent(GameObject & gameObject, int tag, Mix_Chunk* collideSound, int soundChannel);
  
  virtual void collision(Level & level, std::shared_ptr<GameObject> obj) override;

private:

  int mTag;
  Mix_Chunk* collideSound;
  int channel;

};

#endif
