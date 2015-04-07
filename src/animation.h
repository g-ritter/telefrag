/*
 * Animation.h
 *
 *  @since	Nov 8, 2014
 *  @author	g ritter <info@rabbitgirl.me>
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <vector>

/*
 * A container for handling spritesheets and animation loops. Each Animation
 * corresponds to a row of sprites and has handling for how they loop. Does
 * not actually contain the information about where in the Animation an object
 * currently is.
 *
 * Animations act on two time systems - frames, which is how many times the
 * program renders a scene, and cels, which is the point where an object is in
 * an Animation. A single cel corresponds to one sprite, and may take up one
 * frame or one million (or more, I guess).
 */
class Animation {
public:
	bool loop; // Does this Animation loop?
	int loopPoint; // Which frame does the Animation loop back to?
	int length;	// How many frames long is this Animation's loop?s
	std::vector<int> celFrames; // the frame that each cel occurs at. if there
								// are new frames on the third, seventh, and
								// eleventh frames, this vector would be
								// [2, 6, 10]

	// Constructor.
	Animation(bool loop_, int loopPoint_, int length_,
			std::vector<int> celFrames_);

	// Prints this animation.
	void printAnimation();
};

#endif /* ANIMATION_H_ */
