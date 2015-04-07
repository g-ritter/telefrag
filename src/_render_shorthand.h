/*
 * _render_shorthand.cpp
 *
 * Most of these functions are written by Will Usher, and taken from their SDL
 * 2.0 tutorials available here: http://www.willusher.io/pages/sdl2/
 *
 * Some of these functions have been modified to better fit my uses, but they
 * are mostly just copied straight over.
 *
 *  @since	Nov 9, 2014
 *  @author Will Usher (http://www.willusher.io/pages/sdl2/)
 *  @author	g ritter <info@rabbitgirl.me>
 */

#ifndef RENDER_SHORTHAND_H_
#define RENDER_SHORTHAND_H_

#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace _sdl {
/**
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message to
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg);

/*
 * Initializes a the SDL_Window and SDL_Renderer that are required for display
 * stuff. Returns an initialized SDL_Window and modifies the SDL_Renderer
 * passed as an argument.
 */
SDL_Window* init(SDL_Renderer* &ren, int scale, std::string title);

/**
 * Render the message we want to display to a texture for drawing
 * @param message The message we want to display
 * @param fontFile The font we want to use to render the text
 * @param color The color we want the text to be
 * @param fontSize The size we want the font to be
 * @param renderer The renderer to load the texture in
 * @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
 */
void renderText(SDL_Renderer* ren, SDL_Texture* font, int x, int y, int scale,
		const std::string message);

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, scaling the
 * texture's width and height by a given value
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 * @param scale The degree to scale the resulting texture by
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
		int scale);

/*
 * Get the resource path for resources located in res/subDir
 * It's assumed the project directory is structured like:
 * bin/
 *  the executable
 * res/
 *  Lesson1/
 *  Lesson2/
 *
 * Paths returned will be Project_Root/res/subDir
 */
std::string getResourcePath(const std::string &subDir = "");
}

#endif /* RENDER_SHORTHAND_H_ */
