/*
 * Animation.cpp
 *
 *  @since	Nov 8, 2014
 *  @author	g ritter <info@rabbitgirl.me>
 */

#include "animation.h"

#include <iostream>
#include <vector>

/**
 * Constructor for an animation.
 */
Animation::Animation(bool loop_, int loopPoint_, int length_,
		std::vector<int> celFrames_) {
	loop = loop_;
	loopPoint = loopPoint_;
	length = length_;
	celFrames = celFrames_;
}

/**
 * Prints the contents of this Animation for testing.
 */
void Animation::printAnimation() {
	std::cout << "loop: " << loop << '\n';
	std::cout << "loopPoint: " << loopPoint << '\n';
	std::cout << "length: " << length << '\n';
	std::cout << "celFrames:\n";
	for (int i : celFrames) {

		std::cout << i << ", ";
	}

}
