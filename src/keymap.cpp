/*
 * keys.cpp
 *
 *  @since	Apr 2, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#include <iostream>
#include "keymap.h"

/**
 * Constructor.
 */
KeyMap::KeyMap() {
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p1up, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p1down, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p1left, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p1right, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p1shortwarp, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p1longwarp, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p1attack, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p1jump, IDLE));

	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p2up, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p2down, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p2left, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p2right, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p2shortwarp, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p2longwarp, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p2attack, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_p2jump, IDLE));

	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_escape, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_cancel, IDLE));
	keymap.insert(std::pair<SDL_Keycode, KeyState>(CONFIG_confirm, IDLE));
}

/**
 * Update function. Updates any held or idle keys, and iterates through all
 * incoming SDL_Event s and updates the map accordingly.
 *
 * @return A bool indicating if any SDL_QUIT events were recieved.
 */
bool KeyMap::updateInputs() {

	// update keys that were pressed or released LAST frame
	for (std::pair<SDL_Keycode, KeyState> k : keymap) {
		if (k.second == RELEASED) {
			keymap.at(k.first) = IDLE;
		} else if (k.second == PRESSED) {
			keymap.at(k.first) = HELD;
		}
	}

	// get new keyup and keydown events
	SDL_Event e;
	while (SDL_PollEvent(&e)) {

		// return false if any SDL_QUIT events were recieved
		if (e.type == SDL_QUIT) {
			return false;
		}

		if (e.key.type == SDL_KEYDOWN && e.key.repeat == 0) {
			try {
				keymap.at(e.key.keysym.sym) = PRESSED;

			} catch (const std::out_of_range& oor) {
				// do nothing; key just isn't mapped
			}
		}

		else if (e.key.type == SDL_KEYUP) {
			try {
				keymap.at(e.key.keysym.sym) = RELEASED;
			} catch (const std::out_of_range& oor) {
				// do nothing; key just isn't mapped
			}
		}
	}
	return true;
}

