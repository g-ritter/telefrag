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

#include <SDL2/SDL.h>
#include <iostream>
#include <string>

#include "_utilheader.h"

namespace _sdl {

/**
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message to
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " error: " << SDL_GetError() << std::endl;
}

/*
 * Initializes a the SDL_Window and SDL_Renderer that are required for display
 * stuff. Returns an initialized SDL_Window and modifies the SDL_Renderer
 * passed as an argument.
 */
SDL_Window* init(SDL_Renderer* &ren, int scale, std::string title) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return nullptr;
	}
	SDL_Window*win = SDL_CreateWindow(title.c_str(), 100, 100, GAME_WIDTH * WINDOW_SCALE,
			GAME_HEIGHT * WINDOW_SCALE, SDL_WINDOW_SHOWN);
	//Make sure creating our window went ok
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}
	ren = SDL_CreateRenderer(win, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError()
				<< std::endl;
		SDL_Quit();
		return nullptr;
	}
	return win;
}

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
		const std::string message) {
	int len = message.length();
	int cwidth;
	int cheight;
	SDL_QueryTexture(font, NULL, NULL, &cwidth, &cheight);
	cwidth = (cwidth / 8);
	cheight = (cheight / 14);
	for (int i = 0; i < len; i++) {
		char c = message.at(i);

		int col = -1;
		int row = -1;
		if (c >= 'a' && c <= 'z') {
			col = (c - 'a') % 8;
			row = floor((c - 'a') / 8);
		} else if (c >= 'A' && c <= 'Z') {
			col = (c - 'A') % 8;
			row = floor((c - 'A') / 8) + 4;
		} else if (c >= '0' && c <= '9') {
			col = (c - '0') % 8;
			row = floor((c - '0') / 8) + 8;
		} else if (c == ' ') {
			col = 7;
			row = 3;
		} else {
			const char other[30] = {'.', ',', '!', '?', '<', '>', '\"', '\'',
					'(', ')', '[', ']', '{', '}', ':', ';', '@', '#', '$', '~',
					'%', '^', '&', '*', '-', '+', '_', '=', '/', '\\', };
			for (int i2 = 0; i2 < 30; i2++) {
				if (c == other[i2]) {
					col = i2 % 8;
					row = floor(i2 / 8) + 10;
					break;
				}
			}
		}

		if (col < 0 || row < 0) {
			break;
			std::string err = "Rendered text contains undefined characters: ";
			err += message;
			logSDLError(std::cout, err);
		}

		SDL_Rect src;
		src.h = cheight;
		src.w = cwidth;
		src.x = col * cwidth;
		src.y = row * cheight;

		SDL_Rect dst;
		dst.h = cheight * scale;
		dst.w = cwidth * scale;
		dst.x = (x + (i * (cwidth + 1))) * scale;
		dst.y = y * scale;

		//std::cout << c << ": " << src.x << ", " << src.y << "\n";
		SDL_RenderCopy(ren, font, &src, &dst);

	}
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

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
		int scale) {
//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x * scale;
	dst.y = y * scale;
//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	dst.w *= scale;
	dst.h *= scale;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

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
std::string getResourcePath(const std::string &subDir = "") {
#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif
	static std::string baseRes;
	if (baseRes.empty()) {
		char* basePath = SDL_GetBasePath();
		if (basePath) {
			baseRes = basePath;
			SDL_free(basePath);
		} else {
			std::cerr << "Error getting resource path: " << SDL_GetError()
					<< std::endl;
			return "";
		}
		//We replace the last bin/ with res/ to get the the resource path
		size_t pos = baseRes.rfind("bin");
		baseRes = baseRes.substr(0, pos) + "res" + PATH_SEP;
	}
	return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}
}
