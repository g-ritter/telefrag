/*
 * game.cpp
 *
 *  @since	Apr 2, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "_render_shorthand.h"
#include "_utilheader.h"
#include "game.h"
#include "keymap.h"
#include "player.h"

/**
 * Constructor.
 *
 * TODO levelPaths are unimplemented, but they SHOULD be paths to folders
 * containing bg images and level geometry
 */
Game::Game(std::string levelPath, std::string respath, SDL_Renderer* ren) :
		player1(Player(1, respath, ren)), player2(Player(2, respath, ren)) {
	/**
	 * builds the game terrain and adds it all together.
	 * TODO implement a script reader to build terrain from a text file
	 */
	Polygon poly1;
		Polygon poly2;
		Polygon poly3;
		Polygon poly4;
		Polygon poly5;
		Polygon poly6;
		Polygon poly7;
		Polygon poly8;
		Polygon poly9;
		Polygon poly10;
		// bottom bound
		boost::geometry::append(poly1, Point(-200, GAME_HEIGHT - 20));
		boost::geometry::append(poly1, Point(GAME_WIDTH + 200, GAME_HEIGHT - 20));
		boost::geometry::append(poly1, Point(GAME_WIDTH + 200, GAME_HEIGHT + 200));
		boost::geometry::append(poly1, Point(-200, GAME_HEIGHT + 200));
		boost::geometry::append(poly1, Point(-200, GAME_HEIGHT - 20));
		// left bound
		boost::geometry::append(poly2, Point(-200, -200));
		boost::geometry::append(poly2, Point(20, -200));
		boost::geometry::append(poly2, Point(20, GAME_HEIGHT + 200));
		boost::geometry::append(poly2, Point(-200, GAME_HEIGHT + 200));
		boost::geometry::append(poly2, Point(-200, -200));
		// top bound
		boost::geometry::append(poly3, Point(-200, -200));
		boost::geometry::append(poly3, Point(GAME_WIDTH + 200, -200));
		boost::geometry::append(poly3, Point(GAME_WIDTH + 200, 20));
		boost::geometry::append(poly3, Point(-200, 20));
		boost::geometry::append(poly3, Point(-200, -200));
		// right bound
		boost::geometry::append(poly4, Point(GAME_WIDTH - 20, -200));
		boost::geometry::append(poly4, Point(GAME_WIDTH + 200, -200));
		boost::geometry::append(poly4, Point(GAME_WIDTH + 200, GAME_HEIGHT + 200));
		boost::geometry::append(poly4, Point(GAME_WIDTH - 20, GAME_HEIGHT + 200));
		boost::geometry::append(poly4, Point(GAME_WIDTH - 20, -200));
		// pillar
		boost::geometry::append(poly5, Point(190, 180));
		boost::geometry::append(poly5, Point(230, 180));
		boost::geometry::append(poly5, Point(230, 280));
		boost::geometry::append(poly5, Point(190, 280));
		boost::geometry::append(poly5, Point(190, 180));
		// center platform
		boost::geometry::append(poly6, Point(140, 160));
		boost::geometry::append(poly6, Point(280, 160));
		boost::geometry::append(poly6, Point(280, 180));
		boost::geometry::append(poly6, Point(140, 180));
		boost::geometry::append(poly6, Point(140, 160));
		// left-low platform
		boost::geometry::append(poly7, Point(20, 210));
		boost::geometry::append(poly7, Point(90, 210));
		boost::geometry::append(poly7, Point(90, 230));
		boost::geometry::append(poly7, Point(20, 230));
		boost::geometry::append(poly7, Point(20, 210));
		// left-high platform
		boost::geometry::append(poly8, Point(20, 90));
		boost::geometry::append(poly8, Point(110, 90));
		boost::geometry::append(poly8, Point(110, 110));
		boost::geometry::append(poly8, Point(20, 110));
		boost::geometry::append(poly8, Point(20, 90));
		// right-low platform
		boost::geometry::append(poly9, Point(GAME_WIDTH - 20, 210));
		boost::geometry::append(poly9, Point(GAME_WIDTH - 90, 210));
		boost::geometry::append(poly9, Point(GAME_WIDTH - 90, 230));
		boost::geometry::append(poly9, Point(GAME_WIDTH - 20, 230));
		boost::geometry::append(poly9, Point(GAME_WIDTH - 20, 210));
		// right-high platform
		boost::geometry::append(poly10, Point(GAME_WIDTH - 20, 90));
		boost::geometry::append(poly10, Point(GAME_WIDTH - 110, 90));
		boost::geometry::append(poly10, Point(GAME_WIDTH - 110, 110));
		boost::geometry::append(poly10, Point(GAME_WIDTH - 20, 110));
		boost::geometry::append(poly10, Point(GAME_WIDTH - 20, 90));

	terrain.push_back(poly1);
		terrain.push_back(poly2);
		terrain.push_back(poly3);
		terrain.push_back(poly4);
		terrain.push_back(poly5);
		terrain.push_back(poly6);
		terrain.push_back(poly7);
		terrain.push_back(poly8);
		terrain.push_back(poly9);
		terrain.push_back(poly10);

	std::string analytics = "";
}

/**
 * Main game loop.
 *
 * @return A bool indicating if the game should keep going or not
 */
bool Game::update() {

	// reset analytics
	analytics = "";

	// quits the game if there's an SDL_QUIT event or if escape is pressed
	if (!keys.updateInputs() || keys.keymap.at(CONFIG_escape) == keys.PRESSED) {
		return false;
	}

	// TODO implement player double buffering for attacks

	// update player 1
	// skip other updates if player is already dead
	if (!player1.isDead()) {
		// update invincibility frames
		player1.updateIFrames();
		// check if the player is warping, allow the player to warp, and/or
		// check if the player has died WHILE warping
		if (!player1.updateWarp(this) && !player1.isDead()) {
			// get inputs and update movement, jumping, and gravity
			// CONCERN maintain velocity through clips? a la aria of sorrow
			player1.updateVelo(this);
			// clip current velocity vector with the collision geometry
			player1.clipVeloWithCollision(this);
			// update player position with new velocity vector
			player1.updatePos();
			// check if the player is attacking or wants to attack
			// CONCERN shift into the if/then statement?
			player1.checkAttack(this, &player2);
		}
	} else {
		// update player seperately if they're dead
		player1.updateDead();
	}

	// update player 2 in the same way
	if (!player2.isDead()) {
		player2.updateIFrames();
		if (!player2.updateWarp(this) && !player2.isDead()) {
			player2.updateVelo(this);
			player2.clipVeloWithCollision(this);
			player2.updatePos();
			player2.checkAttack(this, &player1);
		}
	} else {
		player2.updateDead();
	}

	// update all particles and delete any dead ones
	for (unsigned int i = 0; i < particles.size();) {
		if (!particles.at(i)->update()) {
			delete particles.at(i);
			particles.erase(particles.begin() + i);
		} else {
			i++;
		}
	}

	// keep looping if everything's good!
	return true;
}

/**
 * Adds a bunch of blood particles to the particle system emanating from
 * some coordinates.
 *
 * @param x The x-coordinate the blood comes from
 * @param y The y-coordinate the blood comes from
 */
void Game::addBlood(double x, double y) {
	const int NUMBER_OF_BLOOD_PARTICLES = 100;
	for (int i = 0; i < NUMBER_OF_BLOOD_PARTICLES; i++) {
		particles.push_back(new Blood(x, y));
	}
}

/**
 * Renders the game data onto the screen. Like magic!
 *
 * @param ren The SDL_Renderer to draw to
 * @param scale The factor to scale the game by
 * 				CONCERN maybe just change all refs to WINDOW_SCALE?
 */
void Game::render(SDL_Renderer* ren, int scale) {

	// update animations for players
	player1.animate();
	player2.animate();

	// render players
	player1.renderPlayer(ren, scale);
	player2.renderPlayer(ren, scale);

	// render particles
	for (Particle* p : particles) {
		p->render(ren, scale);
	}

	// render terrain
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	for (Polygon p : terrain) {
		for (unsigned int i = 0; i < p.outer().size() - 1; i++) {
			SDL_RenderDrawLine(ren, p.outer()[i].x() * scale,
					p.outer()[i].y() * scale, p.outer()[i + 1].x() * scale,
					p.outer()[i + 1].y() * scale);
		}
	}

	// reset color for screen refreshes
	SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
}

