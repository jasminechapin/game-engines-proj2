#include "base/InputManager.hpp"
#include "base/GameObject.hpp"
#include "base/Level.hpp"
#include "base/GenericComponent.hpp"
#include "base/PatrolComponent.hpp"
#include "base/RemoveOnCollideComponent.hpp"
#include "base/RectRenderComponent.hpp"
#include "base/SDLGraphicsProgram.hpp"
#include "base/ResourceManager.hpp"
#include "base/SpriteRenderComponent.hpp"
#include <memory>
#include <ctype.h>
#include <stdio.h>
#include "SDL.h"
#include <iostream>
#include <string>

static const int TAG_PLAYER = 1;
static const int TAG_GOAL = 2;
static const int TAG_BLOCK = 3;
static const int TAG_ENEMY = 4;
static const int TAG_COLLECTIBLE = 5;

std::string filename;

const float SIZE = 40.0f;

/**
 * Input component to manipulate movement in two directions
 * 
 * @param GameObject gameObject - gameObject to be manipulated by input
 * @param float speed - speed at which the gameObject moves through input
 */
class MouseControlComponent : public GenericComponent
{
public:
	MouseControlComponent(GameObject &gameObject) : GenericComponent(gameObject)
	{
	}

	virtual void update(Level &level) override
	{
		//GameObject & gameObject = getGameObject();

		bool playerKey = InputManager::getInstance().isKeyDown(SDLK_p);
		bool enemyKey = InputManager::getInstance().isKeyDown(SDLK_e);
		bool goalKey = InputManager::getInstance().isKeyDown(SDLK_g);
		bool blockKey = InputManager::getInstance().isKeyDown(SDLK_o);
		bool deleteKey = InputManager::getInstance().isKeyDown(SDLK_SPACE);
		bool collectibleKey = InputManager::getInstance().isKeyDown(SDLK_c);
		bool saveKey = InputManager::getInstance().isKeyDown(SDLK_s);

		std::pair<int, int> mousePosn =
			InputManager::getInstance().getMouseGridPosition(SIZE);

		if (playerKey)
		{
			level.makeObject(TAG_PLAYER, mousePosn);
		}
		else if (enemyKey)
		{
			level.makeObject(TAG_ENEMY, mousePosn);
		}
		else if (goalKey)
		{
			level.makeObject(TAG_GOAL, mousePosn);
		}
		else if (blockKey)
		{
			level.makeObject(TAG_BLOCK, mousePosn);
		}
		else if (deleteKey)
		{
			level.removeObjectAtMouse(mousePosn, SIZE);
		}
		else if (collectibleKey)
		{
			level.makeObject(TAG_COLLECTIBLE, mousePosn);
		}
		else if (saveKey)
		{
			std::string output = level.exportLevel(SIZE, filename);
			ResourceManager::getInstance().saveLevel(filename, output);
		}
	}

private:
};

/**
 * Player object with dual axis input. Includes an AdvInput component.
 * @param float x - initial position x
 * @param float y - initial position y
 * 
 */
class EditorPlayer : public GameObject
{
public:
	EditorPlayer(Level &level, float x, float y,
				 std::vector<SDL_Texture *> playerTextures) : GameObject(level, x, y, SIZE, SIZE, TAG_PLAYER)
	{
		setRenderComponent(
			std::make_shared<SpriteRenderComponent>(*this, playerTextures));
	}
};

/**
 * Goal Object within the Adventure game mode, position which the character goes through a win state
 * 
 * @param float x - initial position x
 * @param float y - initial position y
 */
class EditorGoal : public GameObject
{
public:
	EditorGoal(Level &level, float x, float y) : GameObject(level, x, y, SIZE, SIZE, TAG_GOAL)
	{
		setRenderComponent(
			std::make_shared<RectRenderComponent>(*this, 0xff, 0xff, 0x00));
	}
};

//Block allows the component to have collision
class EditorBlock : public GameObject
{
public:
	EditorBlock(Level &level, float x, float y,
				std::vector<SDL_Texture *> blockTextures) : GameObject(level, x, y, SIZE, SIZE, TAG_BLOCK)
	{
		setRenderComponent(
			std::make_shared<SpriteRenderComponent>(*this, blockTextures));
	}
};

/**
 * Enemy object with dual axis movement
 * @param float x - initial position x
 * @param float y - initial position y
 */
class EditorEnemy : public GameObject
{
public:
	EditorEnemy(Level &level, float x, float y) : GameObject(level, x, y, SIZE, SIZE, TAG_ENEMY)
	{
		setRenderComponent(
			std::make_shared<RectRenderComponent>(*this, 0xff, 0x00, 0x00));
	}
};

/**
 * Collectible object 
 * @param float x - initial position x
 * @param float y - initial position y
 */
class EditorCollectible : public GameObject
{
public:
	EditorCollectible(Level &level, float x, float y,
					  std::vector<SDL_Texture *> collectibleTextures) : GameObject(level, x, y, SIZE, SIZE, TAG_COLLECTIBLE)
	{
		setRenderComponent(
			std::make_shared<SpriteRenderComponent>(*this, collectibleTextures));
	}
};

/**
 * Generic editor object from a GameObject with mouse control input 
 * @param level - Current level of the object
 */
class EditorObject : public GameObject
{
public:
	EditorObject(Level &level) : GameObject(level, 0.0f, 0.0f, 0.0f, 0.0f, 0)
	{
		addGenericComponent(std::make_shared<MouseControlComponent>(*this));
	}
};

class EditorLevel : public Level
{
public:
	EditorLevel(std::vector<std::string> layout,
				std::vector<SDL_Surface *> surfaces) : Level(20 * SIZE, 20 * SIZE, true)
	{
		levelLayout = layout;
		levelSurfaces = surfaces;
	}

	/**
		 * Creates an object within the current Editor level and adds it to a vector of items to be constructed in the scene
		 * 
		 * @param tag The type of object to be added to the scene
		 * @param position The current position of the object to write to.
		 */
	void makeObject(int tag, std::pair<int, int> position) override
	{
		if (getObjectAtPosition(std::make_pair(position.first, position.second), SIZE) != 0)
		{
			return;
		}

		switch (tag)
		{
		case TAG_PLAYER: // A player object to be created in the scene
		{ 
			if (mPlayer)
			{
				movePlayer(position, SIZE);
			}
			else
			{
				auto player =
					std::make_shared<EditorPlayer>(*this, position.first * SIZE, position.second * SIZE, playerTextures);
				setPlayer(player);
				addObject(player);
			}
			break;
		}

		case TAG_GOAL: // A goal object to be created in the scene
		{
			if (mGoal)
			{
				moveGoal(position, SIZE);
			}
			else
			{
				auto goal =
					std::make_shared<EditorGoal>(*this, position.first * SIZE, position.second * SIZE);
				setGoal(goal);
				addObject(goal);
			}
			break;
		}

		case TAG_BLOCK: // A block object to be created in the scene
		{
			addObject(std::make_shared<EditorBlock>(*this, position.first * SIZE, position.second * SIZE, blockTextures));
			break;
		}

		case TAG_ENEMY: // An enemy object to be created in the scene
		{
			addObject(
				std::make_shared<EditorEnemy>(*this, position.first * SIZE, position.second * SIZE));
			break;
		}
		case TAG_COLLECTIBLE: // A collectible object to be created in the scene
		{
			addObject(
				std::make_shared<EditorCollectible>(*this, position.first * SIZE, position.second * SIZE, collectibleTextures));
			break;
		}
		}
	}

	void initialize(SDL_Renderer *renderer) override
	{
		finalize();
		for (size_t i = 0; i < 4; i++)
		{
			SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer,
																   levelSurfaces[i]);
			if (newTexture == NULL)
			{
				SDL_Log("Failed to create texture");
			}
			else
			{
				SDL_Log("Loaded texture");
			}
			//Get rid of old loaded surface
			//SDL_FreeSurface(levelSurfaces[i]);
			playerTextures.push_back(newTexture);
		}

		SDL_Texture *block = SDL_CreateTextureFromSurface(renderer,
														  levelSurfaces[4]);
		if (block == NULL)
		{
			SDL_Log("Failed to create texture");
		}
		else
		{
			SDL_Log("Loaded texture");
		}
		//Get rid of old loaded surface
		//SDL_FreeSurface(levelSurfaces[4]);
		blockTextures.push_back(block);

		SDL_Texture *collectibleTexture = SDL_CreateTextureFromSurface(renderer,
																	   levelSurfaces[5]);
		if (block == NULL)
		{
			SDL_Log("Failed to create texture");
		}
		else
		{
			SDL_Log("Loaded texture");
		}
		collectibleTextures.push_back(collectibleTexture);

		float xPos = 0;
		float yPos = 0;
		for (std::size_t i = 0; i < levelLayout.size(); i++)
		{
			for (std::size_t j = 0; j < levelLayout[i].size(); j++)
			{
				if (levelLayout[i][j] == 'O')
				{
					makeObject(TAG_BLOCK, std::make_pair(xPos, yPos));
				}
				if (levelLayout[i][j] == 'P')
				{
					makeObject(TAG_PLAYER, std::make_pair(xPos, yPos));
				}
				if (levelLayout[i][j] == 'G')
				{
					makeObject(TAG_GOAL, std::make_pair(xPos, yPos));
				}
				if (levelLayout[i][j] == 'E')
				{
					makeObject(TAG_ENEMY, std::make_pair(xPos, yPos));
				}
				if (levelLayout[i][j] == 'C')
				{
					makeObject(TAG_COLLECTIBLE, std::make_pair(xPos, yPos));
				}

				addObject(std::make_shared<EditorObject>(*this));
				xPos++;
			}
			xPos = 0;
			yPos++;
		}
	}

private:
	std::vector<std::string> levelLayout;
	std::vector<SDL_Surface *> levelSurfaces;
	std::vector<SDL_Texture *> playerTextures;
	std::vector<SDL_Texture *> blockTextures;
	std::vector<SDL_Texture *> collectibleTextures;
};

void loadResources()
{
	ResourceManager::getInstance().startUp();
	ResourceManager::getInstance().loadLevel(filename);
	ResourceManager::getInstance().loadSurface("Sprites/slime.png");
	ResourceManager::getInstance().loadSurface("Sprites/slimeleft.png");
	ResourceManager::getInstance().loadSurface("Sprites/slimejump.png");
	ResourceManager::getInstance().loadSurface("Sprites/slimejumpleft.png");
	ResourceManager::getInstance().loadSurface("Sprites/tile.png");
	ResourceManager::getInstance().loadSurface("Sprites/collectible.png");
}

int main(int argc, char **argv)
{
	std::cout << "Welcome to the level editor! \n"
			  << "Move your mouse cursor over a tile and press one of the following keys to make changes.\n"
			  << "Press SPACE to delete an object.\n"
			  << "Press P to place the player.\n"
			  << "Press E to place an enemy.\n"
			  << "Press G to place the goal.\n"
			  << "Press O to place a platform.\n"
			  << "Press C to place a collectible.\n"
			  << "Press S to save your level.\n\n"
			  << "Which level would you like to edit?\n"
			  << "(1) Level 1\n"
			  << "(2) Level 2\n"
			  << "(3) Level 3\n";

	int levelChoice;
	std::cin >> levelChoice;
	if (levelChoice == 1)
	{
		filename = "/Levels/level1.txt";
	}
	else if (levelChoice == 2)
	{
		filename = "/Levels/level2.txt";
	}
	else if (levelChoice == 3)
	{
		filename = "/Levels/level3.txt";
	}
	loadResources();
	std::vector<SDL_Surface *> surfaces =
		ResourceManager::getInstance().getSurfaces();
	std::vector<std::string> levelOneFile =
		ResourceManager::getInstance().levelVector[0];
	std::shared_ptr<EditorLevel> firstLevel = std::make_shared<EditorLevel>(levelOneFile, surfaces);
	std::vector<std::shared_ptr<Level>> levels;
	levels.push_back(firstLevel);
	SDLGraphicsProgram mySDLGraphicsProgram(levels);
	mySDLGraphicsProgram.loop();
	ResourceManager::getInstance().shutDown();
	return 0;
}
