/*
 * particle.cpp
 *
 *  @since	Apr 6, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#include <cstdlib>
#include "particle.h"

/**
 * Constructor for a particle of blood with slightly random color and velocity.
 */
Blood::Blood(double x, double y) {
	position = Point(x, y);
	double vx = ((rand() % 6000) - 3000) / 1000.0;
	double vy = (rand() % 10000) / 1000.0;
	velocity = Point(vx, -(vy));
	width = 2;
	height = 2;

	color = SDL_Color();
	color.r = 81 + (rand() % 80);
	color.g = 27 + (rand() % 40);
	color.b = rand() % 30;
	color.a = 255;

	remainingLife = 90;
}

/**
 * Update function. Moves the particle, adds gravity, changes opacity, and
 * determines if the particle is still alive or not.
 *
 * @return A bool indicating if this particle is still alive or not.
 */
bool Blood::update() {

	// kill the particle
	if (remainingLife-- <= 0) {
		return false;
	}

	// update velocity
	velocity.y(velocity.y() + (GRAVITY / 3));
	velocity.x(velocity.x() * .99);

	// apply velocity
	position.x(position.x() + velocity.x());
	position.y(position.y() + velocity.y());

	// fade out particle if it's over half dead
	if (remainingLife < 45) {
		color.a = (remainingLife / 45.0) * 255;
	}

	return true;
}

/**
 * Renders the game data onto the screen. Like magic!
 *
 * @param ren The SDL_Renderer to draw to
 * @param scale The factor to scale the game by
 * 				CONCERN maybe just change all refs to WINDOW_SCALE?
 */
void Blood::render(SDL_Renderer* ren, int scale) {
	SDL_Rect r;
	r.x = (position.x() - (width / 2)) * scale;
	r.y = (position.y() - (height / 2)) * scale;
	r.w = width * scale;
	r.h = height * scale;
	SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(ren, &r);
}

