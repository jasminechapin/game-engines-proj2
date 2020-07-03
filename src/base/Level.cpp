#include "base/Level.hpp"
#include "base/PhysicsManager.hpp"
#include <string>
#include <algorithm>
#include <iostream>

// Construct a level
Level::Level(int w, int h, bool mode) :
		mW(w), mH(h), editingMode(mode) {
}

// Destroy the level
Level::~Level() {
}

// Finalize the new level for use
void Level::finalize() {
	mObjects.clear();
	mObjectsToAdd.clear();
	mObjectsToRemove.clear();
	mPlayer = 0;
	mGoal = 0;
	score = 0;
	win = false;
	die = false;
}

// Add an object to the list of objects to add
void Level::addObject(std::shared_ptr<GameObject> object) {
	mObjectsToAdd.push_back(object);
}

// Add an object to the list of objects to remove
void Level::removeObject(std::shared_ptr<GameObject> object) {
	mObjectsToRemove.push_back(object);
	if (object->tag() == 5) {
		score++;
	}
}

// Set the player to the given object
void Level::setPlayer(std::shared_ptr<GameObject> player) {
	mPlayer = player;
}

// Set the goal to the given object
void Level::setGoal(std::shared_ptr<GameObject> goal) {
	mGoal = goal;
}

// Move the player if there's nothing where it is trying to move
void Level::movePlayer(std::pair<int, int> position, float size) {
	if (getObjectAtPosition(position, size) != 0) {
		return;
	}
	mPlayer->setX(position.first * size);
	mPlayer->setY(position.second * size);
}

// Move the goal to the specified position if available
void Level::moveGoal(std::pair<int, int> position, float size) {
	if (getObjectAtPosition(position, size) != 0) {
		return;
	}
	mGoal->setX(position.first * size);
	mGoal->setY(position.second * size);
}

// Update the level
void Level::update() {
	for (auto obj : mObjectsToAdd) {
		mObjects.push_back(obj);
	}
	mObjectsToAdd.clear();

	for (auto gameObject : mObjects) {
		gameObject->update();
	}

	PhysicsManager::getInstance().step();
	for (auto gameObject : mObjects) {
		gameObject->postStep();
	}

	for (auto obj : mObjectsToRemove) {
		auto elem = std::find(mObjects.begin(), mObjects.end(), obj);

		if (elem != mObjects.end()) {
			if (!editingMode) {
				if (obj->tag() == 2) {
					win = true;
				} else if (obj->tag() == 1) {
					die = true;
				}
			}
			mObjects.erase(elem);
		}
	}
	mObjectsToRemove.clear();
}

// Render the level
void Level::render(SDL_Renderer *renderer) {
	for (auto gameObject : mObjects) {
		gameObject->render(renderer);
	}
}

// Return the object at the specified position
std::shared_ptr<GameObject> Level::getObjectAtPosition(
		std::pair<int, int> mousePosition, float size) {
	auto elem = std::find_if(mObjects.begin(), mObjects.end(),
			[mousePosition, size](std::shared_ptr<GameObject> obj) {
				return (trunc(obj->x() / size) == mousePosition.first
						&& trunc(obj->y() / size) == mousePosition.second);
			});

	if (elem != mObjects.end()) {
		return *elem;
	}

	elem = std::find_if(mObjectsToAdd.begin(), mObjectsToAdd.end(),
			[mousePosition, size](std::shared_ptr<GameObject> obj) {
				return (trunc(obj->x() / size) == mousePosition.first
						&& trunc(obj->y() / size) == mousePosition.second);
			});

	if (elem != mObjectsToAdd.end()) {
		return *elem;
	}

	return NULL;
}

// Remove the object at the mouse position
void Level::removeObjectAtMouse(std::pair<int, int> mousePosition, float size) {
	auto obj = getObjectAtPosition(mousePosition, size);
	if (obj == NULL) {
		return;
	}

	if (obj == mPlayer) {
		mPlayer = 0;
	} else if (obj == mGoal) {
		mGoal = 0;
	}

	mObjectsToRemove.push_back(obj);
}

// Export the level
std::string Level::exportLevel(float size, std::string filename) {
	std::string output;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			auto posn = std::make_pair(j, i);
			std::shared_ptr<GameObject> obj = getObjectAtPosition(posn, size);

			if (obj != NULL) {
				int tag = obj->tag();
				switch (tag) {
				case 0: {
					output += "_";
					break;
				}
				case 1: {
					output += "P";
					break;
				}
				case 2: {
					output += "G";
					break;
				}
				case 3: {
					output += "O";
					break;
				}
				case 4: {
					output += "E";
					break;
				}
				case 5: {
					output += "C";
					break;
				}
				}
			} else {
				output += "_";
			}
		}
		output += "\n";
	}
	return output;
}

// Return the score
int Level::getScore() {
	return score;
}

// Return if the level has been won
bool Level::isWin() {
	return win;
}

// Return if the player has died
bool Level::getDie() {
	return die;
}
