// Support Code written by Michael D. Shah
// Updated by Seth Cooper
// Last Updated: Spring 2020
// Please do not redistribute without asking permission.

#include "SDLGraphicsProgram.hpp"
#include "InputManager.hpp"
#include "PhysicsManager.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include <sstream>
#include <time.h>

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
float avgFPS;

LTimer fpsTimer;
LTimer capTimer;

bool win = false;
bool gameOver = false;

std::vector<std::string> textVector;

// Initialization function
SDLGraphicsProgram::SDLGraphicsProgram(
		std::vector<std::shared_ptr<Level>> levels) :
		mLevel(levels[0]) {
	gameLevels = levels;
	// Initialize random number generation.
	srand(time(nullptr));
	// Initialization flag
	bool success = true;
	// String to hold any errors that occur.
	std::stringstream errorStream;
	// The window we'll be rendering to
	mWindow = nullptr;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError()
				<< "\n";
		success = false;
	} else {
		//Create window
		mWindow = SDL_CreateWindow("Lab", 100, 100, mLevel->w(), mLevel->h(),
				SDL_WINDOW_SHOWN);

		// Check if Window did not create.
		if (mWindow == nullptr) {
			errorStream << "Window could not be created! SDL Error: "
					<< SDL_GetError() << "\n";
			success = false;
		}

		//Create a Renderer to draw on
		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
		// Check if Renderer did not create.
		if (mRenderer == nullptr) {
			errorStream << "Renderer could not be created! SDL Error: "
					<< SDL_GetError() << "\n";
			success = false;
		}
	}

	if (TTF_Init() != 0) {
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		success = false;
	}

	InputManager::getInstance().startUp();
	PhysicsManager::getInstance().startUp();
	// If initialization did not work, then print out a list of errors in the constructor.
	if (!success) {
		errorStream << "Failed to initialize!\n";
		std::string errors = errorStream.str();
		std::cout << errors << "\n";
	} else {
		std::cout << "No SDL errors Detected in during init\n\n";
	}
}

// Proper shutdown and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram() {
	PhysicsManager::getInstance().shutDown();
	InputManager::getInstance().shutDown();

	// Destroy Renderer
	SDL_DestroyRenderer(mRenderer);
	mRenderer = nullptr;

	//Destroy window
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;

	//Quit SDL subsystems
	SDL_Quit();
}

// Update OpenGL
void SDLGraphicsProgram::update() {
	mLevel->update();
}

// Log an error
void SDLGraphicsProgram::logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

// Load display text
void SDLGraphicsProgram::loadText() {
	ResourceManager::getInstance().loadText("Text/english.txt");
	textVector = ResourceManager::getInstance().englishText;
}

// Render
// The render function gets called once per loop
void SDLGraphicsProgram::render() {
	SDL_SetRenderDrawColor(mRenderer, 0x73, 0xCE, 0xD8, 0xFF);
	SDL_RenderClear(mRenderer);

	mLevel->render(mRenderer);
	std::stringstream timeText;
	timeText.str("");
	timeText << "FPS: " << avgFPS;
	std::stringstream scoreText;
	scoreText.str("");
	scoreText << textVector[0] << mLevel->getScore();

	std::stringstream finTextRender;
	finTextRender.str("");
	finTextRender << scoreText.str() << "                      "
			<< timeText.str();

	if (gameOver) {
		finTextRender.str("");
		finTextRender << textVector[3];
	}
	if (win) {
		finTextRender.str("");
		finTextRender << textVector[2];
	}

	SDL_Color color = { 255, 255, 255, 255 };
	std::string path = "../../res/Fonts/712_serif.ttf";
	SDL_Texture *textImage = renderText(finTextRender.str().c_str(),
			path, color, 36, mRenderer) == 0 ? renderText(finTextRender.str().c_str(),
			"res/Fonts/712_serif.ttf", color, 36, mRenderer) : renderText(finTextRender.str().c_str(),
			path, color, 36, mRenderer);
	renderTexture(textImage, mRenderer, 0, 0);

	SDL_DestroyTexture(textImage);

	SDL_RenderPresent(mRenderer);
}

// Render text
SDL_Texture* SDLGraphicsProgram::renderText(const std::string &message,
		const std::string &fontFile, SDL_Color color, int fontSize,
		SDL_Renderer *renderer) {
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr) {
		logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderUTF8_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

// Renders a texture with specified width and height
void SDLGraphicsProgram::renderTexture(SDL_Texture *tex, SDL_Renderer *ren,
		int x, int y, int w, int h) {
	// Set up the destination rectangle to be at the given position with the given dimensions
	SDL_Rect dst { x, y, w, h };
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

// Renders texture using texture's own width and height
void SDLGraphicsProgram::renderTexture(SDL_Texture *tex, SDL_Renderer *ren,
		int x, int y) {
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

//Loops forever!
void SDLGraphicsProgram::loop() {
	// Main loop flag
	// If this is quit = 'true' then the program terminates.
	bool quit = false;
	// Event handler that handles various events in SDL
	// that are related to input and output
	SDL_Event e;

	mLevel->initialize(mRenderer);
	Mix_PlayMusic(ResourceManager::getInstance().bgm, -1);
	loadText();
	int countedFrames = 0;
	fpsTimer.start();
	// While application is running
	while (!quit) {
		InputManager::getInstance().resetForFrame();
		capTimer.start();
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_q) {
					quit = true;
				}
				if (e.key.keysym.sym == SDLK_r) {
					gameOver = false;
					if (win) {
						win = false;
						mLevelNum = 0;
						mLevel->finalize();
						mLevel = gameLevels[mLevelNum];
					}
					mLevel->initialize(mRenderer);
					Mix_PlayMusic(ResourceManager::getInstance().bgm, -1);
				}
				if (e.key.keysym.sym == SDLK_n) {
					gameOver = false;
					win = false;
					if (mLevelNum == 2) {
						mLevelNum = 0;
					} else {
						mLevelNum++;
					}
					mLevel->finalize();
					mLevel = gameLevels[mLevelNum];
					mLevel->initialize(mRenderer);
					Mix_PlayMusic(ResourceManager::getInstance().bgm, -1);
				}
			}
			InputManager::getInstance().handleEvent(e);
		}
		if (mLevel->isWin()) {
			if (mLevelNum == 2) {
				win = true;
				Mix_HaltMusic();
			} else {
				mLevelNum++;
			}
			mLevel->finalize();
			mLevel = gameLevels[mLevelNum];
			mLevel->initialize(mRenderer);
		}
		if (mLevel->getDie()) {
			Mix_HaltMusic();
			gameOver = true;
		}
		avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000) {
			avgFPS = 0;
		}
		// update
		if (!win && !gameOver) {
			update();
		}

		// render
		render();

		++countedFrames;
		int frameTicks = capTimer.getTicks();
		if (frameTicks < SCREEN_TICKS_PER_FRAME) {
			//Wait remaining time
			SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
		}
	}
	mLevel->finalize();
}

/**
 * Get Pointer to Renderer
 */
SDL_Renderer* SDLGraphicsProgram::getSDLRenderer() {
	return mRenderer;
}
