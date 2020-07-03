#ifndef BASE_PATROL_COMPONENT
#define BASE_PATROL_COMPONENT

#include "base/GenericComponent.hpp"

//! \brief A component that causes its game object to patrol back and forth.
class PatrolComponent: public GenericComponent {
public:

  PatrolComponent(GameObject & gameObject, float toX, float toY, float speed);
  
  virtual void update(Level & level);

private:

  // TODO: add variables
  float startX, startY, destX, destY, startVx, startVy, speedX, speedY;
};

#endif
