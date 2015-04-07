/*
 * player.h
 *
 *  @since	Apr 2, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "animation.h"

class Game;
class KeyMap;

/**
 * Class for holding information on a player character
 */
class Player {
public:

	Polygon hitbox; // Hitbox for collision detection
	Point position; // Current 2D position of this player

	int dimX;			// the dimensions of a single cel for this Character
	int dimY;

	// the Animation cycle that this Player is in.
	// correlates to the y axis on the sprite sheet.
	int animationNo;
	// the current frame of the Animation that the Player is on.
	int frameNo;
	// the direction this Player is facing (true = right, false = left)
	bool flip;
	// Container for animation cycle information for this Player.
	std::vector<Animation> anims;

	SDL_Texture* tex; // Texture data for the Player
	SDL_Texture* attacktex;	// Texture data for the attack animation

	int kills;	// K/D tracking for scores
	int deaths;

private:
	int playerNo; 	//The identifier for this player
	Point velocity;	// The 2D velocity of this player

	// The number of remaining frames that this player is
	// invincible; if 0, player can be killed
	int iFrames;
	// The number of frames that this is player is warping;
	// if 0, player is not warping!!
	int warpFrames;
	// The number of frames that this is player is attacking;
	// if 0, player is not warping!!
	int attackFrames;
	// The number of frames that this is player is dead;
	// if 0, player is not warping!!
	int deadFrames;

	//int health;	// CONCERN unused
	//int bullets; 	// CONCERN unused

	// state variables
	bool s_running;
	bool s_attacking;
	bool s_jumping;
	bool s_warping;
	bool s_dead;

	// Keycodes for each available player action
	SDL_Keycode key_up;
	SDL_Keycode key_down;
	SDL_Keycode key_right;
	SDL_Keycode key_left;
	SDL_Keycode key_attack;
	SDL_Keycode key_jump;
	SDL_Keycode key_shortwarp;
	SDL_Keycode key_longwarp;

	/** ~~~*&*^ MAGIC NUMBERS ^*&*~~~ **/
	static constexpr float shortwarpDistance = 35; // Pixels traveled by a short warp
	static constexpr float longwarpDistance = 65; // Pixels traveled by a long warp

public:

	// Constructor for a Player TODO expand on this
	Player(int playerNo_, std::string respath, SDL_Renderer* ren);

	// Updates the animation cycle for this player
	void animate();

	// Checks if the player is on the ground
	bool isOnGround(Game* game);

	// Warps the player or updates their state based on their warp
	bool updateWarp(Game* game);

	// Updates the invincibility frames for this player
	void updateIFrames();

	// Checks if the player should be killed from warping into geometry
	void telefrag(Game* game);

	// Kills the player and sets them up for a respawn
	void killPlayer(Game* game, CauseOfDeath cod);

	// Checks if a player is dead
	bool isDead();

	// Special update function for dead players
	void updateDead();

	// Updates the player's velocity
	void updateVelo(Game* game);

	// Adjusts player velocity by clipping it with the terrain collision
	bool clipVeloWithCollision(Game* game);

	// Updates player position based on velocity
	void updatePos();

	// Renders this player
	void renderPlayer(SDL_Renderer* ren, int scale);

	// Checks if a player is attacking or trying to attack and updates
	// accordingly; also handles killing the other player.
	void checkAttack(Game* game, Player* opponent);

	// Sets an animation for this player and resets their frame number
	void setAnimation(int animationNo_);

	// Checks if this player can be killed
	bool isKillable();
};

#endif /* PLAYER_H_ */
