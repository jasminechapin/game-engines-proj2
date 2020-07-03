#include "ResourceManager.hpp"
#include "res_path.hpp"
#include <fstream>
#include <iostream>

/**
 * Implementation of the ResourceManager.hpp header.
 * Allows access to resources and makes sure there is
 * only one copy of each resource loaded at a time.
 */

// Initializing instance
ResourceManager *ResourceManager::instance = 0;

// Empty constructor
ResourceManager::ResourceManager() {
}

// Returns instance of ResourceManager
ResourceManager& ResourceManager::getInstance() {
	if (!instance) {
		instance = new ResourceManager();
	}
	return *instance;
}

// Starts ResourceManager
int ResourceManager::startUp() {
	std::cout << "Started" << std::endl;
	return 0;
}

// Shuts down ResourceManager
int ResourceManager::shutDown() {
	//Free the surface
	SDL_FreeSurface(surface);
	surface = NULL;
	//Free the texture
	//SDL_DestroyTexture(texture);
	//sprites.clear();
	levelVector.clear();
	//Free the sound effects
	Mix_FreeChunk(jumpEff);
	Mix_FreeChunk(missEff);
	jumpEff = NULL;
	missEff = NULL;

	//Free the music
	Mix_FreeMusic(bgm);
	bgm = NULL;
	return 0;
}

// Loads in text to display from text file
int ResourceManager::loadText(std::string filename) {
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;
	std::ifstream inFile(filePath);
	std::string line;

	if (inFile.is_open()) {
		SDL_Log("Loaded text");
		while (getline(inFile, line)) {
			if (filename.compare("Text/russian.txt") == 0) {
				russianText.push_back(line);
			} else if (filename.compare("Text/english.txt") == 0) {
				englishText.push_back(line);
			}
		}
	} else {
		SDL_Log("Failed to open text");
		return 1;
	}

	inFile.close();
	return 0;
}

// Loads in level from text file
int ResourceManager::loadLevel(std::string filename) {
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;
	std::vector < std::string > level;

	std::ifstream inFile(filePath);
	std::string line;
	if (inFile.is_open()) {
		SDL_Log("Loaded level");
		while (getline(inFile, line)) {
			level.push_back(line);
		}
	} else {
		SDL_Log("Failed to open level");
		return 1;
	}

	inFile.close();
	levelVector.push_back(level);
	return 0;
}

// Saves level to text file
int ResourceManager::saveLevel(std::string filename, std::string output) {
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;

	std::ofstream outFile(filePath);
	if (outFile.is_open()) {
		outFile << output;
		outFile.close();
	} else
		std::cout << "Unable to open file";
	return 0;
}

//Load Surfaces
int ResourceManager::loadSurface(std::string filename) {
	//Load image at specified path
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;
	SDL_Surface *loadedSurface = IMG_Load(filePath.c_str());
	if (loadedSurface == NULL) {
		SDL_Log("Failed to load image");
		return 1;
	} else {
		SDL_Log("Loaded image");
	}
	surfaces.push_back(loadedSurface);
	return 0;
}

//Load BGM
int ResourceManager::loadBGM(std::string filename) {
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;
	bgm = Mix_LoadMUS(filePath.c_str());
	if (bgm == NULL) {
		SDL_Log("Failed to allocate background music");
		return 1;
	} else {
		SDL_Log("Loaded background music once");
	}
	return 0;
}

//Load Effects
int ResourceManager::loadJumpEffect(std::string filename) {
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;
	jumpEff = Mix_LoadWAV(filePath.c_str());
	if (jumpEff == NULL) {
		SDL_Log("Failed to allocate jump sound effect");
		return 1;
	} else {
		SDL_Log("Loaded jump sound once");
	}
	return 0;
}

int ResourceManager::loadMissEffect(std::string filename) {
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;
	missEff = Mix_LoadWAV(filePath.c_str());
	if (missEff == NULL) {
		SDL_Log("Failed to allocate background music");
		return 1;
	} else {
		SDL_Log("Loaded death sound once");
	}
	return 0;
}

int ResourceManager::loadCollectEffect(std::string filename) {
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;
	collectEff = Mix_LoadWAV(filePath.c_str());
	if (collectEff == NULL) {
		SDL_Log("Failed to allocate collectible sound effect");
		return 1;
	} else {
		SDL_Log("Loaded goal sound once");
	}
	return 0;
}

int ResourceManager::loadGoalEffect(std::string filename) {
	std::string resPath = getResourcePath();
	std::string filePath = resPath + filename;
	goalEff = Mix_LoadWAV(filePath.c_str());
	if (goalEff == NULL) {
		SDL_Log("Failed to allocate collectible sound effect");
		return 1;
	} else {
		SDL_Log("Loaded collectible sound once");
	}
	return 0;
}

std::vector<SDL_Surface*> ResourceManager::getSurfaces() {
	return surfaces;
}

