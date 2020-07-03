// Support Code written by Michael D. Shah
// Updated by Seth Cooper
// Last Updated: Spring 2020
// Please do not redistribute without asking permission.

#include "base/InputManager.hpp"
#include "base/GameObject.hpp"
#include "base/Level.hpp"
#include "base/GenericComponent.hpp"
#include "base/RemoveOnCollideComponent.hpp"
#include "base/RectRenderComponent.hpp"
#include "base/SpriteRenderComponent.hpp"
#include "base/PhysicsComponent.hpp"
#include "base/PatrolComponent.hpp"
#include "base/PhysicsManager.hpp"
#include "base/SDLGraphicsProgram.hpp"
#include "base/ResourceManager.hpp"
#include <SDL.h>
#include <SDL_mixer.h>
#include <Box2D/Box2D.h>
#include <memory>

static const int TAG_PLAYER = 1;
static const int TAG_GOAL = 2;
static const int TAG_BLOCK = 3;
static const int TAG_ENEMY = 4;
static const int TAG_COLLECTIBLE = 5;

int channel;

class JmpInputComponent: public GenericComponent {
public:

	JmpInputComponent(GameObject &gameObject, float speed, float jump,
			float gravity, Mix_Chunk *jumpSound) :
			GenericComponent(gameObject), mSpeed(speed), mJump(jump), mGravity(
					gravity) {
		jSound = jumpSound;
	}

	virtual void update(Level &level) override
	{
		bool left = InputManager::getInstance().isKeyDown(SDLK_LEFT);
		bool right = InputManager::getInstance().isKeyDown(SDLK_RIGHT);
		bool jump = InputManager::getInstance().isKeyPressed(SDLK_UP);

		GameObject &gameObject = getGameObject();
		std::shared_ptr<PhysicsComponent> pc = gameObject.physicsComponent();
		b2Body *body = pc->getBody();

		std::shared_ptr<RenderComponent> rc = gameObject.renderComponent();
		std::shared_ptr<SpriteRenderComponent> spriteComponent =
				std::static_pointer_cast < SpriteRenderComponent > (rc);
		pc->addFy(mGravity);

		if (left && !right) {
			pc->setVx(-mSpeed);
			// flip sprite
			if (spriteComponent->getSprite() == 0) {
				spriteComponent->setSprite(1);
			} else if (spriteComponent->getSprite() == 2) {
				spriteComponent->setSprite(3);
			}
		} else if (!left && right) {
			pc->setVx(mSpeed);
			// flip sprite
			if (spriteComponent->getSprite() == 1) {
				spriteComponent->setSprite(0);
			} else if (spriteComponent->getSprite() == 3) {
				spriteComponent->setSprite(2);
			}
		} else {
			pc->setVx(0.0f);
		}

		// check if velocity is low enough to switch to grounded sprite
		if (std::abs(body->GetLinearVelocity().y) < 0.1) {
			if (spriteComponent->getSprite() > 1) {
				spriteComponent->setSprite(spriteComponent->getSprite() - 2);
			}
		}

		if (jump) {
			bool onGround = false;
			std::vector < std::shared_ptr < GameObject >> objects;
			if (PhysicsManager::getInstance().getCollisions(gameObject.x() + 1,
					gameObject.y() + gameObject.h(), gameObject.w() - 2, 2.0f,
					objects)) {
				for (auto obj : objects) {
					if (obj->tag() == TAG_BLOCK) {
						onGround = true;
					}
				}
			}

			if (onGround) {
				pc->setVy(-mJump);
				//switch to jumping sprite
				if (spriteComponent->getSprite() < 2) {
					spriteComponent->setSprite(
							spriteComponent->getSprite() + 2);
				}
				channel = Mix_PlayChannel(-1, jSound, 0);
			}
		}
	}

private:

	float mSpeed;
	float mJump;
	float mGravity;
	Mix_Chunk *jSound;
};

const float SIZE = 40.0f;

class JmpPlayer: public GameObject {
public:
	JmpPlayer(Level &level, float x, float y,
			std::vector<SDL_Texture*> playerTextures, Mix_Chunk *jumpSound,
			Mix_Chunk *goalSound, Mix_Chunk *collectibleSound) :
			GameObject(level, x, y, SIZE, SIZE, TAG_PLAYER) {
		addGenericComponent(
				std::make_shared < JmpInputComponent
						> (*this, 500.0f, 5000.0f, 50000.0f, jumpSound));
		addGenericComponent(
				std::make_shared < RemoveOnCollideComponent
						> (*this, TAG_GOAL, goalSound, channel));
		addGenericComponent(
				std::make_shared < RemoveOnCollideComponent
						> (*this, TAG_COLLECTIBLE, collectibleSound, channel));
		setPhysicsComponent(
				std::make_shared < PhysicsComponent
						> (*this, PhysicsComponent::Type::DYNAMIC_SOLID));
		setRenderComponent(
				std::make_shared < SpriteRenderComponent
						> (*this, playerTextures));
	}
};

class JmpGoal: public GameObject {
public:
	JmpGoal(Level &level, float x, float y) :
			GameObject(level, x, y, SIZE, SIZE, TAG_GOAL) {
		setPhysicsComponent(
				std::make_shared < PhysicsComponent
						> (*this, PhysicsComponent::Type::STATIC_SENSOR));
		setRenderComponent(
				std::make_shared < RectRenderComponent
						> (*this, 0xff, 0x99, 0x00));
	}
};

class Collectible: public GameObject {
public:
	Collectible(Level &level, float x, float y,
			std::vector<SDL_Texture*> collectibleTextures) :
			GameObject(level, x, y, SIZE, SIZE, TAG_COLLECTIBLE) {
		setPhysicsComponent(
				std::make_shared < PhysicsComponent
						> (*this, PhysicsComponent::Type::STATIC_SENSOR));
		setRenderComponent(
				std::make_shared < SpriteRenderComponent
						> (*this, collectibleTextures));
	}
};

class JmpBlock: public GameObject {
public:
	JmpBlock(Level &level, float x, float y,
			std::vector<SDL_Texture*> blockTextures) :
			GameObject(level, x, y, SIZE, SIZE, TAG_BLOCK) {
		setPhysicsComponent(
				std::make_shared < PhysicsComponent
						> (*this, PhysicsComponent::Type::STATIC_SOLID));
		setRenderComponent(
				std::make_shared < SpriteRenderComponent
						> (*this, blockTextures));
	}
};

class PatrolEnemy: public GameObject {
public:
	PatrolEnemy(Level &level, float x, float y, float distX, float distY,
			Mix_Chunk *deathSound) :
			GameObject(level, x, y, SIZE, SIZE, TAG_ENEMY) {
		addGenericComponent(
				std::make_shared < RemoveOnCollideComponent
						> (*this, TAG_PLAYER, deathSound, channel));
		setPhysicsComponent(
				std::make_shared < PhysicsComponent
						> (*this, PhysicsComponent::Type::DYNAMIC_SOLID));
		setRenderComponent(
				std::make_shared < RectRenderComponent
						> (*this, 0xff, 0x00, 0x00));
		addGenericComponent(
				std::make_shared < PatrolComponent
						> (*this, x + distX, y + distY, SIZE * 2));
	}
};

class JmpLevel: public Level {
public:
	JmpLevel(std::vector<std::string> layout,
			std::vector<SDL_Surface*> surfaces, std::vector<Mix_Chunk*> sounds) :
			Level(20 * SIZE, 20 * SIZE, false) {
		levelLayout = layout;
		levelSurfaces = surfaces;
		levelSounds = sounds;
	}

	void makeObject(int tag, std::pair<int, int> position) override {
		switch (tag) {
		case TAG_PLAYER: {
			auto player =
					std::make_shared < JmpPlayer
							> (*this, position.first * SIZE, position.second
									* SIZE, playerTextures, levelSounds[0], levelSounds[3], levelSounds[2]);
			addObject(player);
		}
			break;

		case TAG_GOAL: {
			auto goal = std::make_shared < JmpGoal
					> (*this, position.first * SIZE, position.second * SIZE);
			addObject(goal);
			break;
		}

		case TAG_BLOCK: {
			addObject(
					std::make_shared < JmpBlock
							> (*this, position.first * SIZE, position.second
									* SIZE, blockTextures));
			break;
		}

		case TAG_ENEMY: {
			addObject(
					std::make_shared < PatrolEnemy
							> (*this, position.first * SIZE, position.second
									* SIZE, SIZE * 2, 0, levelSounds[1]));
			break;
		}

		case TAG_COLLECTIBLE: {
			addObject(
					std::make_shared < Collectible
							> (*this, position.first * SIZE, position.second
									* SIZE, collectibleTextures));
		}
		}
	}

	void initialize(SDL_Renderer *renderer) override
	{
		finalize();
		for (size_t i = 0; i < 4; i++) {
			SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer,
					levelSurfaces[i]);
			if (newTexture == NULL) {
				SDL_Log("Failed to create texture");
			} else {
				SDL_Log("Loaded texture");
			}
			playerTextures.push_back(newTexture);
		}

		SDL_Texture *block = SDL_CreateTextureFromSurface(renderer,
				levelSurfaces[4]);
		if (block == NULL) {
			SDL_Log("Failed to create texture");
		} else {
			SDL_Log("Loaded texture");
		}
		blockTextures.push_back(block);

		SDL_Texture *collectibleTexture = SDL_CreateTextureFromSurface(renderer,
				levelSurfaces[5]);
		if (block == NULL) {
			SDL_Log("Failed to create texture");
		} else {
			SDL_Log("Loaded texture");
		}
		collectibleTextures.push_back(collectibleTexture);

		float xPos = 0;
		float yPos = 0;
		for (std::size_t i = 0; i < levelLayout.size(); i++) {
			for (std::size_t j = 0; j < levelLayout[i].size(); j++) {
				if (levelLayout[i][j] == 'O') {
					makeObject(TAG_BLOCK, std::make_pair(xPos, yPos));
				}

				if (levelLayout[i][j] == 'P') {
					makeObject(TAG_PLAYER, std::make_pair(xPos, yPos));
				}
				if (levelLayout[i][j] == 'G') {
					makeObject(TAG_GOAL, std::make_pair(xPos, yPos));
				}
				if (levelLayout[i][j] == 'E') {
					makeObject(TAG_ENEMY, std::make_pair(xPos, yPos));
				}
				if (levelLayout[i][j] == 'C') {
					makeObject(TAG_COLLECTIBLE, std::make_pair(xPos, yPos));
				}
				xPos++;
			}
			xPos = 0;
			yPos++;
		}
	}

private:
	std::vector<std::string> levelLayout;
	std::vector<SDL_Surface*> levelSurfaces;
	std::vector<SDL_Texture*> playerTextures;
	std::vector<SDL_Texture*> blockTextures;
	std::vector<SDL_Texture*> collectibleTextures;
	std::vector<Mix_Chunk*> levelSounds;
}
;

int main(int argc, char **argv) {
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n",
				Mix_GetError());
	}
	//int channel;
	ResourceManager::getInstance().startUp();
	ResourceManager::getInstance().loadLevel("/Levels/level1.txt");
	ResourceManager::getInstance().loadLevel("/Levels/level2.txt");
	ResourceManager::getInstance().loadLevel("/Levels/level3.txt");
	ResourceManager::getInstance().loadSurface("Sprites/slime.png");
	ResourceManager::getInstance().loadSurface("Sprites/slimeleft.png");
	ResourceManager::getInstance().loadSurface("Sprites/slimejump.png");
	ResourceManager::getInstance().loadSurface("Sprites/slimejumpleft.png");
	ResourceManager::getInstance().loadSurface("Sprites/tile.png");
	ResourceManager::getInstance().loadSurface("Sprites/collectible.png");
	ResourceManager::getInstance().loadJumpEffect("Sounds/Jump.wav");
	ResourceManager::getInstance().loadMissEffect("Sounds/Death.wav");
	ResourceManager::getInstance().loadBGM("Sounds/BGM.wav");

	ResourceManager::getInstance().loadCollectEffect("Sounds/Collectible.wav");
	ResourceManager::getInstance().loadGoalEffect("Sounds/Goal.wav");

	std::vector<Mix_Chunk*> soundVector;
	soundVector.push_back(ResourceManager::getInstance().jumpEff);
	soundVector.push_back(ResourceManager::getInstance().missEff);
	soundVector.push_back(ResourceManager::getInstance().collectEff);
	soundVector.push_back(ResourceManager::getInstance().goalEff);

	std::vector<SDL_Surface*> surfaces =
			ResourceManager::getInstance().getSurfaces();
	std::vector < std::string > levelOneFile =
			ResourceManager::getInstance().levelVector[0];
	std::vector < std::string > levelTwoFile =
			ResourceManager::getInstance().levelVector[1];
	std::vector < std::string > levelThreeFile =
			ResourceManager::getInstance().levelVector[2];
	std::shared_ptr<JmpLevel> firstLevel = std::make_shared < JmpLevel
			> (levelOneFile, surfaces, soundVector);
	std::shared_ptr<JmpLevel> secondLevel = std::make_shared < JmpLevel
			> (levelTwoFile, surfaces, soundVector);
	std::shared_ptr<JmpLevel> thirdLevel = std::make_shared < JmpLevel
			> (levelThreeFile, surfaces, soundVector);
	std::vector < std::shared_ptr < Level >> levels;
	levels.push_back(firstLevel);
	levels.push_back(secondLevel);
	levels.push_back(thirdLevel);
	SDLGraphicsProgram mySDLGraphicsProgram(levels);
	mySDLGraphicsProgram.loop();
	ResourceManager::getInstance().shutDown();
	return 0;
}
