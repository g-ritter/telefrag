/*
 * keys.h
 *
 *  @since	Apr 2, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#ifndef KEYMAP_H_
#define KEYMAP_H_

#include <exception>
#include <map>
#include <SDL2/SDL.h>
#include "_utilheader.h"

// CONCERN privately nest in game.cpp?
/**
 * A container for the current state of every key the player can or
 * has pressed. Maps SDL_Keycode s to values of a KeyState enum.
 */
class KeyMap {
public:
	// Represents the potential states each key can be in.
	enum KeyState {
		IDLE = 0,
		PRESSED = 1,
		HELD = 2,
		RELEASED = 3
	};

	// The actual container for all the KeyState s
	std::map<SDL_Keycode, KeyState> keymap;

	// Constructor
	KeyMap();

	// Gets all incoming SDL_Events and updates the map accordingly
	bool updateInputs();
};

#endif /* KEYMAP_H_ */
