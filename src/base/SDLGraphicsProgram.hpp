// Support Code written by Michael D. Shah
// Updated by Seth Cooper
// Last Updated: Spring 2020
// Please do not redistribute without asking permission.

#ifndef BASE_SDL_GRAPHICS_PROGRAM_HPP
#define BASE_SDL_GRAPHICS_PROGRAM_HPP

#include "base/Level.hpp"
#include <memory.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "LTimer.hpp"

//! \brief This class sets up a full graphics program.
class SDLGraphicsProgram {
public:

  // Constructor
  SDLGraphicsProgram(std::vector<std::shared_ptr<Level>> levels);

  // Desctructor
  ~SDLGraphicsProgram();

  // Per frame update
  void update();

  // Renders shapes to the screen
  void render();

  // loop that runs forever
  void loop();

  /**
   * Get Pointer to Renderer
   */
  SDL_Renderer* getSDLRenderer();

  /**
   * Log an SDL Error
   */
  void logSDLError(std::ostream &os, const std::string &msg);

  /**
   * Loads display text.
   */
  void loadText();

  /**
   * Render the message we want to display to a texture for drawing
   * @param message The message we want to display
   * @param fontFile The font we want to use to render the text
   * @param color The color we want the text to be
   * @param fontSize The size we want the font to be
   * @param renderer The renderer to load the texture in
   * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
   */
  SDL_Texture* renderText(const std::string &message,
  		const std::string &fontFile, SDL_Color color, int fontSize,
  		SDL_Renderer *renderer);

  /**
   * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired
   * width and height
   * @param tex The source texture we want to draw
   * @param ren The renderer we want to draw to
   * @param x The x coordinate to draw to
   * @param y The y coordinate to draw to
   * @param w The width of the texture to draw
   * @param h The height of the texture to draw
   */
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren,
  		int x, int y, int w, int h);

  /**
   * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
   * the texture's width and height
   * @param tex The source texture we want to draw
   * @param ren The renderer we want to draw to
   * @param x The x coordinate to draw to
   * @param y The y coordinate to draw to
   */
  void renderTexture(SDL_Texture *tex, SDL_Renderer *ren,
  		int x, int y);

private:

  // the current level
  std::shared_ptr<Level> mLevel;
  
  // The window we'll be rendering to
  SDL_Window * mWindow = nullptr;

  // SDL Renderer
  SDL_Renderer * mRenderer = nullptr;

  std::vector<std::shared_ptr<Level>> gameLevels;
  int mLevelNum = 0;

};

#endif
