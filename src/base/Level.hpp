#ifndef BASE_LEVEL
#define BASE_LEVEL

#include "base/GameObject.hpp"
#include <SDL.h>
#include <memory>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>
#include <iostream>

//! \brief A level in the game.  Essentially manages a collection of game
//! objects, and does some collision detection.
class Level {
public:

	/**
	 * Constructor
	 * @param int w: width
	 * @param int h: height
	 * @param bool mode: mode
	 */
  Level(int w, int h, bool mode);

  /**
   * Destructor
   */
  virtual ~Level();

  /**
   * Initializes the level
   * @param SDL_Renderer* renderer: pointer to SDL_Renderer to render to
   */
  virtual void initialize(SDL_Renderer* renderer) = 0;

  /**
   * Makes an object in the level, based on tag and position
   * @param int tag: tag of the object
   * @param std::pair<int, int> position: the position to place the object at
   */
  virtual void makeObject(int tag, std::pair<int, int> position) = 0;

  /**
   * Finalizes the level
   */
  void finalize();

  /**
   * Return the width
   */
  inline int w() const { return mW; }

  /**
   * Return the height
   */
  inline int h() const { return mH; }

  /**
   * Set an object to be added
   * @param std::shared_ptr<GameObject> object: the object to be added
   */
  void addObject(std::shared_ptr<GameObject> object);

  /**
   * Set an object to be removed
   * @param std::shared_ptr<GameObject> object: the object to be removed
   */
  void removeObject(std::shared_ptr<GameObject> object);
  
  /**
   * Sets a player object
   */
  void setPlayer(std::shared_ptr<GameObject> player);

  /**
   * Sets a goal object
   */
  void setGoal(std::shared_ptr<GameObject> goal);

  /**
   * Moves the player
   */
  void movePlayer(std::pair<int, int> position, float size);

  /**
   * Moves the goal
   */
  void moveGoal(std::pair<int, int> position, float size);

  /**
   * Returns the object at the specified position
   */
  std::shared_ptr<GameObject> getObjectAtPosition(std::pair<int, int> mousePosition, float size);

  /**
   * Removes the object where the mouse is pointed
   */
  void removeObjectAtMouse(std::pair<int, int> mousePosition, float size);

  /**
   * Update objects in level
   */
  void update();

  /**
   * Render the level
   */
  void render(SDL_Renderer * renderer);

  /**
   * Return score for display purposes
   */
  int getScore();

  /**
   * Export level as a string
   * @param float size: the size
   * @param std::string filename: the name of the file
   */
  std::string exportLevel(float size, std::string filename);

  /**
   * Returns if the player has won the level
   */
  bool isWin();

  /**
   * Returns if the player has died
   */
  bool getDie();

private:

  Level(const Level &) = delete;
  void operator=(Level const&) = delete;

  int mW, mH;
  std::vector<std::shared_ptr<GameObject>> mObjects;
  int score = 0;
  std::vector<std::shared_ptr<GameObject>> mObjectsToAdd;
  std::vector<std::shared_ptr<GameObject>> mObjectsToRemove;
  bool win = false;
  bool die = false;
  bool editingMode;

protected:
  std::shared_ptr<GameObject> mPlayer;
  std::shared_ptr<GameObject> mGoal;
};

#endif
