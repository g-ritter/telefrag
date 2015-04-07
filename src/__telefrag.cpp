/*
 * telefrag.cpp
 *
 *  @since	Apr 1, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "_utilheader.h"
#include "_render_shorthand.h"
#include "game.h"
using namespace std;

// don't touch these
// CONCERN maybe change this to be frame rate agnostic
const static unsigned int TARGET_FRAME_RATE = 30; // runs correctly at 30;
const static unsigned int TARGET_FRAME_DUR = 1000 / TARGET_FRAME_RATE;
static SDL_Renderer* ren;
static SDL_Window* win = _sdl::init(ren, 1, "TELEFRAG");

/**
 * The main loop
 */
int main() {
	// set up simple RNG CONCERN make this better?
	// probably not necessary for my purposes
	srand(time(NULL));
	cout << rand() << endl;

	// set up frame rate timers
	Uint32 timeAtStartOfFrame;
	Uint32 timeElapsed;

	// get resource path
	respath = _sdl::getResourcePath("");

	// initialize game containers
	Game game = Game("", respath, ren);

	// main game loop
	while (true) {
		// set up frame rate timer and clear screen
		timeAtStartOfFrame = SDL_GetTicks();
		SDL_RenderClear(ren);

		// update game logic; break out of loop if game is over
		if (!game.update()) {
			break;
		}

		// render things
		game.render(ren, WINDOW_SCALE);
		SDL_RenderPresent(ren);

		// frame rate limiting
		// delay some amount if there's still time left over for this frame
		timeElapsed = SDL_GetTicks() - timeAtStartOfFrame;
		if (timeElapsed < TARGET_FRAME_DUR) {
			SDL_Delay(TARGET_FRAME_DUR - timeElapsed);
		}
	}
	cout << "FINAL TALLY: " << endl;
	cout << "PLAYER 1: " << game.player1.kills << " KILLS, " << game.player1.deaths << " DEATHS" << endl;
	cout << "PLAYER 2: " << game.player2.kills << " KILLS, " << game.player2.deaths << " DEATHS" << endl;
	// rough cleanup
	SDL_DestroyTexture(game.player1.tex);
	SDL_DestroyTexture(game.player2.tex);
	SDL_DestroyTexture(game.player1.attacktex);
	SDL_DestroyTexture(game.player2.attacktex);
	for (Particle* p : game.particles) {
		delete p;
	}
	SDL_DestroyWindow(win);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
	return 0;
}
