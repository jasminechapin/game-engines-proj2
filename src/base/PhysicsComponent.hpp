#ifndef BASE_PHYSICS_COMPONENT
#define BASE_PHYSICS_COMPONENT

#include "base/Component.hpp"
#include <Box2D/Box2D.h>

class Level;

//! \brief A component for handling physics.
class PhysicsComponent: public Component {
public:

  enum class Type {
    DYNAMIC_SOLID,
    STATIC_SOLID,
    STATIC_SENSOR
  };
  
  PhysicsComponent(GameObject & gameObject, Type type);
  virtual ~PhysicsComponent();

  void setVx(float vx); //!< set x velocity
  void setVy(float vy); //!< set y velocity

  void addFx(float fx); //!< add force in x direction
  void addFy(float fy); //!< add force in y direction

  void postStep(); //!< Called after physics step.
  b2Body* getBody();
private:

  b2Body *mBody;

};

#endif
