/*
 * particle.h
 *
 *  @since	Apr 6, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#ifndef PARTICLE_H_
#define PARTICLE_H_

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "_utilheader.h"

/**
 * Base class for a component of a particle system. Pure virutal.
 */
class Particle {
protected:
	Point position;	// The position of this particle
	Point velocity; // The velocity of this particle
	int width;	// The width of this particle
	int height;	// The height of this particle
	SDL_Color color;	// The color of this particle
	int remainingLife;	// The number of frames left until this particle dies

public:
	// Destructor.
	virtual ~Particle() {
	}
	;

	// Update function.
	virtual bool update() = 0;

	// Render function.
	virtual void render(SDL_Renderer* ren, int scale) = 0;

};

class Blood : public Particle {
public:
	// Constructor.
	Blood(double x, double y);

	// Update function.
	bool update();

	// Render function.
	void render(SDL_Renderer* ren, int scale);
};

#endif /* PARTICLE_H_ */
