#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <map>
#include <vector>
/**
 * A class for managing game resources.  Allows access to resources
 * and makes sure there is only one copy of each resource loaded at a
 * time.
 */
class ResourceManager {
private:

	ResourceManager();  // Private Singleton
	ResourceManager(ResourceManager const&) = delete; // Avoid copy constructor.
	void operator=(ResourceManager const&) = delete; // Don't allow copy assignment.
	ResourceManager(ResourceManager const&&) = delete; // Avoid move constructor.
	void operator=(ResourceManager const&&) = delete; // Don't allow move assignment.

	// ... // perhaps other private members
	static ResourceManager *instance;

public:

	/**
	 * This gets the instance of the resource manager. If one does not already exist, it creates one.
	 */
	static ResourceManager& getInstance();

	std::vector<std::string> russianText;
	std::vector<std::string> englishText;
	std::vector<SDL_Surface*> surfaces;
	int loadText(std::string filename);

	/**
	 * An allocated surface that holds the different frames an animation
	 */
	SDL_Surface *surface;

	/**
	 * The music to be played
	 */
	Mix_Music *bgm;

	/**
	 * Jump sound effect
	 */
	Mix_Chunk *jumpEff;
	/**
	 * Ball loss sound effect
	 */
	Mix_Chunk *missEff;
	/**
	 * Collect item sound effect
	 */
	Mix_Chunk *collectEff;
	/**
	 * Reach goal sound effect
	 */
	Mix_Chunk *goalEff;
	/**
	 * Stores level layout from files in a vector
	 */
	std::vector<std::vector<std::string>> levelVector;

	/**
	 *  'equivalent' to our constructor
	 *  A method that acts as a constructor. It does nothing as of right now.
	 */
	int startUp();

	/**
	 *'equivalent' to our destructor
	 * Frees memory and sets things to null
	 */
	int shutDown();

	/**
	 * Takes in the name of a file and loads it into a texture
	 * @param std::string filename: the name of the file.
	 */
	int loadSurface(std::string filename);

	/**
	 * Takes in the name of a file and loads it into a music track
	 * @param std::string filename: the name of the file.
	 */
	int loadBGM(std::string filename);

	/**
	 * Takes in the name of a file and loads it into an effect
	 * @param std::string filename: the name of the file.
	 */
	int loadJumpEffect(std::string filename);

	/**
	 * Takes in the name of a file and loads it into an effect
	 * @param std::string filename: the name of the file.
	 */
	int loadMissEffect(std::string filename);

	/**
	 * Takes in the name of a file and loads it into an effect
	 * @param std::string filename: the name of the file
	 */
	int loadCollectEffect(std::string filename);

	/**
	 * Takes in the name of a file and loads it into an effect
	 * @param std::string filename: the name of the file
	 */
	int loadGoalEffect(std::string filename);

	/**
	 * Loads level from .txt. file
	 * @param std::string filename: the name of the file.
	 */
	int loadLevel(std::string filename);

	/**
	 * Saves level to .txt file
	 * @param std::string filename: the name of the file.
	 * @param std::string output: what is being written to the file.
	 */
	int saveLevel(std::string filename, std::string output);

	std::vector<SDL_Surface*> getSurfaces();

};

#endif
