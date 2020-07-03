#include "base/PatrolComponent.hpp"
#include "base/GameObject.hpp"
#include "base/Level.hpp"
#include "base/PhysicsComponent.hpp"
#include <cmath>

PatrolComponent::PatrolComponent(GameObject &gameObject, float toX, float toY,
		float speed) :
		GenericComponent(gameObject), startX(gameObject.x()), startY(
				gameObject.y()), destX(toX), destY(toY), startVx(speed), startVy(
				speed), speedX(startVx), speedY(startVy) {

	if (std::abs(startX - destX) < 0.001) {
		startVx = 0;
	} else {
		startVx = speed;
	}

	if (std::abs(startY - destY) < 0.001) {
		startVy = 0;
	} else {
		startVy = speed;
	}

	speedX = startVx;
	speedY = startVy;

	if (destX < startX) {
		speedX *= -1;
	}

	if (destY < startY) {
		speedY *= -1;
	}
}

void PatrolComponent::update(Level &level) {
	// TODO: update position
	GameObject &obj = getGameObject();

	if (std::abs(obj.x() - destX) < 0.001) {
		speedX = 0;
	}

	if (std::abs(obj.y() - destY) < 0.001) {
		speedY = 0;
	}

	if (speedX == 0 && speedY == 0) {
		float tempX = destX;
		float tempY = destY;
		destX = startX;
		destY = startY;
		startX = tempX;
		startY = tempY;

		if (destX > startX) {
			speedX = startVx;
		} else {
			speedX = startVx * -1;
		}

		if (destY > startY) {
			speedY = startVy;
		} else {
			speedY = startVy * -1;
		}
	}

	std::shared_ptr<PhysicsComponent> pc = obj.physicsComponent();
	pc->setVx(speedX);
	pc->setVy(speedY);
}
