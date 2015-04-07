/*
 * game.h
 *
 *  @since	Apr 2, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#ifndef GAME_H_
#define GAME_H_

#include "_utilheader.h"
#include "keymap.h"
#include "particle.h"
#include "player.h"

/**
 * Class holding all the current game screen data.
 */
class Game {

public:
	// Collection of Polygons describing the game world geometry; used for
	// simple rendering and for collision
	std::vector<Polygon> terrain;
	// Collection of all game Particles
	std::vector<Particle*> particles;
	// Map of all current key states
	KeyMap keys;
	// Players
	Player player1;
	Player player2;
	// string used to dump debug data. currently unused
	std::string analytics;

	// Constructor
	Game(std::string levelPath, std::string respath, SDL_Renderer* ren);

	// Game logic update
	bool update();

	// Adds a good old SPLURT of blood to the particle system
	void addBlood(double x, double y);

	// Renders the current game screen
	void render(SDL_Renderer* ren, int scale);
};

#endif /* GAME_H_ */
