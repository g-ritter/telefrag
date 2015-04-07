/*
 * player.cpp
 *
 *  @since	Apr 1, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#include <cmath>
#include <iostream>
#include <utility>
#include <vector>
#include <SDL2_image/SDL_image.h>

#include "_utilheader.h"
#include "game.h"
#include "keymap.h"
#include "player.h"

/**
 * Constructor TODO expand
 */
Player::Player(int playerNo_, std::string respath, SDL_Renderer* ren) {

	playerNo = playerNo_;

	// initialize positional information
	velocity = Point(0, 0); // first is x, second is y
	if (playerNo == 1) {
		position = Point((GAME_WIDTH * .15) - 5, 41);
	} else {
		position = Point((GAME_WIDTH * .85) - 5, 41);
	}

	// build hitbox off of positional information
	hitbox.clear();
	boost::geometry::append(hitbox, Point(position.x(), position.y()));
	boost::geometry::append(hitbox, Point(position.x() + 11, position.y()));
	boost::geometry::append(hitbox,
			Point(position.x() + 11, position.y() + 11));
	boost::geometry::append(hitbox, Point(position.x(), position.y() + 11));
	boost::geometry::append(hitbox, Point(position.x(), position.y()));

	// init state
	iFrames = 0;
	warpFrames = 0;
	attackFrames = 0;
	deadFrames = 0;
	kills = 0;
	deaths = 0;
	s_running = false;
	s_attacking = false;
	s_jumping = false;
	s_warping = false;
	s_dead = false;

	// init graphical info
	dimX = 12;
	dimY = 11;
	animationNo = 0;
	frameNo = 0;

	// load textures
	if (playerNo == 1) {
		flip = false;
		tex = IMG_LoadTexture(ren, (respath + "/player.png").c_str());
	} else {
		flip = true;
		tex = IMG_LoadTexture(ren, (respath + "/player2.png").c_str());
	}
	attacktex = IMG_LoadTexture(ren, (respath + "/attack.png").c_str());

	// build animations
	anims.push_back(
			Animation(true, 0, 128, std::vector<int> {0, 125, 126, 127}));
	anims.push_back(
			Animation(true, 0, 40, std::vector<int> {0, 8, 16, 24, 32}));
	anims.push_back(Animation(false, 0, 5, std::vector<int> {0, 1, 2, 3, 4}));

	// assign input keycodes to values defined in _utilheader.h
	if (playerNo == 1) {
		key_up = CONFIG_p1up;
		key_down = CONFIG_p1down;
		key_right = CONFIG_p1right;
		key_left = CONFIG_p1left;
		key_attack = CONFIG_p1attack;
		key_jump = CONFIG_p1jump;
		key_shortwarp = CONFIG_p1shortwarp;
		key_longwarp = CONFIG_p1longwarp;
	} else {
		key_up = CONFIG_p2up;
		key_down = CONFIG_p2down;
		key_right = CONFIG_p2right;
		key_left = CONFIG_p2left;
		key_attack = CONFIG_p2attack;
		key_jump = CONFIG_p2jump;
		key_shortwarp = CONFIG_p2shortwarp;
		key_longwarp = CONFIG_p2longwarp;
	}
}
;

/**
 * Advances the animation cycle for this character. Handles looping, animation
 * delays, etc.
 */
void Player::animate() {
	frameNo++; // advance frame counter
	Animation temp = anims.at(animationNo); // grabs the current animation

	// loop back to the loopPoint if the frameNo has reached its end
	if (frameNo >= temp.length) {

		if (temp.loop) {
			frameNo = temp.loopPoint;
		}
	}
}

/**
 * Sets a player's animation and resets the frame counter.
 */
void Player::setAnimation(int animationNo_) {
	animationNo = animationNo_;
	frameNo = -1; // set to -1 so animate() starts it on the first frame
}

/**
 * Checks if the player is standing on the ground or not. A player is defined as
 * "being on the ground" if there is a part of level geometry some distance
 * (defined in VECTOR_CHECK_LENGTH) below at least one part of the player's hitbox.
 *
 * @return A bool indicating if the player is on the ground or not
 */
bool Player::isOnGround(Game* game) {

	const double VECTOR_CHECK_LENGTH = 0.99; // defines how far below the player to check for ground

	// iterate through every point in the player's hitbox
	for (Point p : hitbox.outer()) {

		// get "destination" and the segment connecting the position and destination
		Point p2 = Point(p.x(), p.y() + VECTOR_CHECK_LENGTH);
		std::vector<Point> segment {p, p2};
		std::vector<Point> out;

		// iterate through terrain
		for (Polygon poly : game->terrain) {
			boost::geometry::intersection(
					linestring(segment.begin(), segment.end()),
					linestring(poly.outer().begin(), poly.outer().end()), out);
			// if there's at least one collision point, player is "on ground"
			if (!out.empty()) {
				return true;
			}
		}
	}
	// no collision points found
	return false;
}

/**
 * Updates the state and position of this player if they're warping or
 * have chosen to warp.
 *
 * @param k The current state of the inputs
 * @param terrain The terrain collision of the game world; passed to warpKill()
 * @return A bool indicating true if the player is warping or has chosen to warp,
 * 		   false if the player is not
 */
bool Player::updateWarp(Game* game) {

	const int NUM_OF_WARP_FRAMES = 5;

	// if the player is already warping:
	if (s_warping) {

		// decrement warp frames
		warpFrames--;

		// switch for player position in the warp process
		switch (warpFrames) {

		case 4:
			// do nothing, player is warping out
			break;

		case 3: {
			// player is still warping, but has disappeared: update
			// position and hitbox
			position.x(position.x() + velocity.x());
			position.y(position.y() + velocity.y());
			Polygon newHitbox;
			for (Point p : hitbox.outer()) {
				boost::geometry::append(newHitbox,
						Point(p.x() + velocity.x(), p.y() + velocity.y()));
			}
			boost::geometry::assign(hitbox, newHitbox);
			break;
		}

		case 2:
		case 1:
			// player is warping in, do nothing
			break;

		case 0:
			// player is done warping; reset velocity and check for TELEFRAG
			// (state changes take place in telefrag)
			velocity.x(0), velocity.y(0);
			telefrag(game);
			break;
		}
		return true;
	}

	// warp the player if they aren't attacking, dead, and have pressed
	// one of the warp buttons
	else if ((game->keys.keymap.at(key_shortwarp) == game->keys.PRESSED
			|| game->keys.keymap.at(key_longwarp) == game->keys.PRESSED)
			&& (!s_attacking && !s_dead)) {

		warpFrames = NUM_OF_WARP_FRAMES; // start up warp frames
		s_warping = true;	// set state
		setAnimation(2);	// set animation to warp animation

		// determines desired magnitude of warp vector. root of 2 is a magic number;
		// just shorthand for magnitude calculation
		double desiredWarpMag;
		if (game->keys.keymap.at(key_shortwarp) == game->keys.PRESSED) {
			desiredWarpMag = shortwarpDistance * std::sqrt(2.0);
		} else {
			desiredWarpMag = longwarpDistance * std::sqrt(2.0);
		}

		// initialize velocity CONCERN maintain velocity through warps? for jumps?
		velocity.x(0);
		velocity.y(0);

		// set up warp directions
		if (game->keys.keymap.at(key_right) == game->keys.PRESSED
				|| game->keys.keymap.at(key_right) == game->keys.HELD) {
			velocity.x(1);
		}
		if (game->keys.keymap.at(key_left) == game->keys.PRESSED
				|| game->keys.keymap.at(key_left) == game->keys.HELD) {
			velocity.x(-1);
		}
		if (game->keys.keymap.at(key_up) == game->keys.PRESSED
				|| game->keys.keymap.at(key_up) == game->keys.HELD) {
			velocity.y(-1);
		}
		if (game->keys.keymap.at(key_down) == game->keys.PRESSED
				|| game->keys.keymap.at(key_down) == game->keys.HELD) {
			velocity.y(1);
		}

		// determine magnitude of CURRENT warp velocity
		double warpMag = std::sqrt(
				std::pow(velocity.x(), 2) + std::pow(velocity.y(), 2));

		// scale warp velocity to desired magnitude
		if (warpMag != 0) {
			double scale = desiredWarpMag / warpMag;
			velocity.x(velocity.x() * scale);
			velocity.y(velocity.y() * scale);
		}
		return true;
	} else {
		return false;
	}
}

/**
 * Checks if the player has telefragged themselves by warping into level geometry
 *
 * @param terrain The level geometry being checked against
 */
void Player::telefrag(Game* game) {

	// iterates through the player's hitbox
	for (Point point : hitbox.outer()) {

		// iterates through the terrain
		for (Polygon poly : game->terrain) {

			// kills the player if any part of them is inside of the level geometry
			if (boost::geometry::within(point, poly)) {
				killPlayer(game, WARP_INTO_WALL);
				return;
			}
		}
	}

	// disables warping if the player is still alive
	s_warping = false;
	setAnimation(0);
}

/**
 * Decrements the player's invincibility frames.
 */
void Player::updateIFrames() {
	iFrames--;
}

/**
 * Checks if the player is dead.
 *
 * @return A bool indicating if this player is alive or not
 */
bool Player::isDead() {
	if (s_dead) {
		return true;
	} else
		return false;
}

/**
 * A seperate update loop for dead players; decrements deadFrames and brings
 * players back to life.
 */
void Player::updateDead() {

	// brings the player back to life if their deadFrames have run out
	if (deadFrames-- <= 0) {
		iFrames = 45;	// gives the player invincibility frames
		s_dead = false;
		setAnimation(0);
	}
}

/**
 * Kills this player, resets their position, and prints a message about how
 * they died.
 *
 * @param game A reference to the current game data
 * @param cod An enum describing how the player died
 */
void Player::killPlayer(Game* game, CauseOfDeath cod) {

	const int NUM_OF_DEADFRAMES = 30;// how many frames the player spends DEAD

	// Prints a death message saying what player died, how they died,
	// and some random flavor text.
	std::cout << "PLAYER " << std::to_string(playerNo) << " DIED "
			<< codMsg.at(cod) << " " << deathMsg.at(rand() % deathMsg.size())
			<< std::endl;

	// Adds some blood where the player died
	game->addBlood(position.x(), position.y());

	// Resets player position, velocity, and hitboxes
	velocity = Point(0, 0);
	if (playerNo == 1) { // TODO determine spawn points in level data
		position = Point((GAME_WIDTH * .15) - 5, 41);
		flip = false;
	} else {
		position = Point((GAME_WIDTH * .85) - 5, 41);
		flip = true;
	}
	hitbox.clear();
	boost::geometry::append(hitbox, Point(position.x(), position.y()));
	boost::geometry::append(hitbox, Point(position.x() + 11, position.y()));
	boost::geometry::append(hitbox,
			Point(position.x() + 11, position.y() + 11));
	boost::geometry::append(hitbox, Point(position.x(), position.y() + 11));
	boost::geometry::append(hitbox, Point(position.x(), position.y()));
	iFrames = 0;
	warpFrames = 0;
	attackFrames = 0;
	deaths++;	// increment number of deaths
	deadFrames = NUM_OF_DEADFRAMES;
	s_dead = true; // SET DEAD TO TRUE, ALL ELSE TO FALSE
	s_warping = false;
	s_jumping = false;
	s_running = false;
	s_attacking = false;
}

/**
 * Updates the player velocity based on the player and input state
 *
 * @param game A reference to the current game data
 */
void Player::updateVelo(Game* game) {

	// Cap for how fast a player can move themselves
	const double VELO_CAP = 5;
	// How much faster a player moves per-frame
	const double VELO_INCREMENTOR = 1;
	// The rate at which a player slows down after stopping inputs
	const double VELO_DIVISOR = 1.7;
	// The lower bound on movement speed
	const double VELO_LOW_BOUND = 0.25;

	bool playerMoved = false;	// Tracks if the player has moved this frame

	// skip x velocity updates if the player is warping, attacking, or DEAD
	if (!s_attacking && !s_warping && !s_dead) {

		// move right if the correct keys are pressed or held
		if (game->keys.keymap.at(key_right) == game->keys.PRESSED
				|| game->keys.keymap.at(key_right) == game->keys.HELD) {

			// make the player face right
			flip = false;
			playerMoved = true;

			// update velocity
			if (velocity.x() < VELO_CAP) {
				velocity.x(velocity.x() + VELO_INCREMENTOR);
			}
		}

		// move left if the correct keys are pressed or held
		else if (game->keys.keymap.at(key_left) == game->keys.PRESSED
				|| game->keys.keymap.at(key_left) == game->keys.HELD) {

			// make the player face left
			flip = true;
			playerMoved = true;

			// update velocity
			if (velocity.x() > -(VELO_CAP)) {
				velocity.x(velocity.x() - VELO_INCREMENTOR);
			}
		}
	}

	// decrease velocity if the player is not trying to move
	if ((!playerMoved && !(s_jumping && s_attacking) && !(s_running && s_attacking))) {
		if (std::abs(velocity.x()) <= VELO_LOW_BOUND) {
			velocity.x(0);
		} else {
			velocity.x(velocity.x() / VELO_DIVISOR);
		}
	}

	// allow jumping if the player is on the ground
	if (isOnGround(game)) {

		// update state since the player isn't in the air
		if (s_jumping) {
			s_jumping = false;
		}

		// let the player jump if they've hit the jump button and aren't busy
		if (game->keys.keymap.at(key_jump) == game->keys.PRESSED
				&& (!s_warping && !s_attacking && !s_dead)) {
			s_jumping = true;
			setAnimation(0);
			velocity.y(JUMP_HEIGHT);
		} else {
			// if on the ground and not jumping, set y velocity to 0
			velocity.y(0);
		}
	} else {
		// apply gravity if player is in the air
		velocity.y(velocity.y() + GRAVITY);
	}

	// turn off running and jumping if player is still
	if (velocity.y() == 0 && velocity.x() == 0) {
		s_jumping = false, s_running = false;
		setAnimation(0);
	} else if (!s_jumping) {
		// turn on running if player is moving and not jumping
		s_running = true;
		setAnimation(1);
	}
}

/**
 * Clips and readjusts player velocity based on collision detection using
 * speculative contacts.
 * TODO add collision for other players
 * FIXME
 * @param p The other player - used for collision
 * @param terrain The level geometry to be checked against
 * @return A bool indicating if there was any collision on this frame
 * TODO return a bool indicating if player is standing?
 */
bool Player::clipVeloWithCollision(Game* game) {

	const double BUFFER = .0001;

	bool collision = false;

	// calculate magnitude of original velocity
	double veloMagnitude = std::sqrt(
			std::pow(velocity.x(), 2) + std::pow(velocity.y(), 2));

	// iterate through every collision point in this box
	for (Point p : hitbox.outer()) {

		// get destination and segment connecting two points
		Point p2 = Point(p.x() + velocity.x(), p.y() + velocity.y());
		std::vector<Point> segment {p, p2};
		std::vector<Point> out;

		// iterate through terrain
		for (Polygon poly : game->terrain) {

			// check if segment collides with this terrain
			if (boost::geometry::intersection(
					linestring(segment.begin(), segment.end()),
					linestring(poly.outer().begin(), poly.outer().end()),
					out)) {

				// iterate through collision points
				for (Point p_out : out) {

					double newMagnitude = std::sqrt(
							std::pow(p_out.x() - p.x(), 2)
									+ std::pow(p_out.y() - p.y(), 2));

					// if this collision point is closer, rescale velocity to adjust
					if (newMagnitude < veloMagnitude) {

						double scale = newMagnitude / veloMagnitude;

						velocity.x(velocity.x() * scale);
						velocity.y(velocity.y() * scale);
						veloMagnitude = newMagnitude;
						p2 = Point(p.x() + velocity.x(), p.y() + velocity.y());
						segment.clear();
						segment.push_back(p);
						segment.push_back(p2);
						collision = true;
					}
				}
			}
		}
	}

	if (collision) {

		if (velocity.x() > 0) {
			velocity.x(velocity.x() - BUFFER);
		} else if (velocity.x() < 0) {
			velocity.x(velocity.x() + BUFFER);
		}
		if (velocity.y() > 0) {
			velocity.y(velocity.y() - BUFFER);
		} else if (velocity.y() < 0) {
			velocity.y(velocity.y() + BUFFER);
		}
	}
	game->analytics.append("\n");
	return collision;
}

/**
 * Updates player position based on velocity
 */
void Player::updatePos() {
	// add velocity to position
	position.x(position.x() + velocity.x());
	position.y(position.y() + velocity.y());

	// build hitbox with new position
	Polygon newHitbox;
	for (Point p : hitbox.outer()) {
		boost::geometry::append(newHitbox,
				Point(p.x() + velocity.x(), p.y() + velocity.y()));
	}
	boost::geometry::assign(hitbox, newHitbox);
}

/**
 * Updates player attacks, allows players to attack if they aren't already,
 * and checks for attack collision with other players.
 *
 * @param game A reference to the current game data.
 * @param opponent A reference to the other player in the game.
 */
void Player::checkAttack(Game* game, Player* opponent) {

	// turns off attacking if the attackFrames have run out
	if (attackFrames-- <= 0) {
		s_attacking = false;
	}

	// turns on attacking if the player is able to attack and presses the key
	if (game->keys.keymap.at(key_attack) == game->keys.PRESSED
			&& !s_attacking) {
		attackFrames = 5;
		s_attacking = true;
	}

	// runs collision detection if the players opponent can be killed and the
	// player is attacking
	if (s_attacking && opponent->isKillable()) {

		// build hitbox for attack
		Point p;
		Polygon poly;
		if (flip) {
			p.x((position.x() - 13));
		} else {
			p.x(position.x() + dimX);
		}
		p.y(position.y());
		boost::geometry::append(poly, p);
		boost::geometry::append(poly, Point(p.x() + 13, p.y()));
		boost::geometry::append(poly, Point(p.x() + 13, p.y() + 11));
		boost::geometry::append(poly, Point(p.x(), p.y() + 11));
		boost::geometry::append(poly, p);

		// check if any part of the opponents hitbox is in the attack hitbox
		for (Point p2 : opponent->hitbox.outer()) {
			if (boost::geometry::covered_by(p2, poly)) {
				kills++;
				opponent->killPlayer(game, CUT_BY_PLAYER);
				return;
			}
		}
	}
}

/**
 * Checks if this player is killable. Players are killable if they aren't
 * dead, warping, or invincible.
 *
 * @return A bool indicating if this player can be killed.
 */
bool Player::isKillable() {
	return !(s_dead || s_warping || iFrames > 0);
}

/**
 * Renders the game data onto the screen. Like magic!
 *
 * @param ren The SDL_Renderer to draw to
 * @param scale The factor to scale the game by
 * 				CONCERN maybe just change all refs to WINDOW_SCALE?
 */
void Player::renderPlayer(SDL_Renderer* ren, int scale) {

	// Don't render the player if they're dead
	if (s_dead) {
		return;
	}

	// "strobe" the player's transparency if they're invincible
	if (iFrames > 0) {
		Uint8 alpha = (std::sin(((M_PI / 7.5) * iFrames) + (.5 * M_PI)) + 1)
				* 127;
		SDL_SetTextureAlphaMod(tex, alpha);
	}

	// determine whether to flip the texture based on the player
	SDL_RendererFlip flipval;
	if (flip) {
		flipval = SDL_FLIP_HORIZONTAL;
	} else {
		flipval = SDL_FLIP_NONE;
	}

	// instantiate rendering positions
	SDL_Rect src;
	SDL_Rect dst;

	// find the x-position of the current cel
	bool foundCel = false;
	// iterate through time spent on each cell in the current animation
	for (unsigned int i = 0; i < anims.at(animationNo).celFrames.size(); i++) {
		// check if the celFrames of this animation step is greater than the
		// currently elapsed number of frames
		if (anims.at(animationNo).celFrames.at(i) > frameNo && !foundCel) {
			// if yes, set that cel number as the x-source
			src.x = (i - 1) * dimX;
			foundCel = true;
		}
	}
	// if no frames are found, default to the last animation
	if (!foundCel) {
		src.x = (anims.at(animationNo).celFrames.size() - 1) * dimX;
	}

	// get spritesheet positions
	src.y = animationNo * dimY;
	src.w = dimX;
	src.h = dimY;

	// get on-screen draw positions
	dst.x = position.x() * scale;
	dst.y = position.y() * scale;
	dst.w = dimX * scale;
	dst.h = dimY * scale;

	// render player
	SDL_RenderCopyEx(ren, tex, &src, &dst, 0, NULL, flipval);
	SDL_SetTextureAlphaMod(tex, 255); // reset alpha drawing

	// draw the "sword" if the player is attacking
	if (attackFrames > 0) {

		// get spritesheet positions
		src.w = 13;
		src.h = 11;
		src.x = ((5 - attackFrames) * src.w); // make attackFrames count "forwards"
		src.y = 0;

		// determine where to draw the sword based on the
		// direction the player is facing
		if (flip) {
			dst.x = (position.x() - 13) * scale;
		} else {
			dst.x = (position.x() + dimX) * scale;
		}

		// fill out on-screen draw positions
		dst.y = position.y() * scale;
		dst.w = 13 * scale;
		dst.h = 11 * scale;

		// draw "sword"
		SDL_RenderCopyEx(ren, attacktex, &src, &dst, 0, NULL, flipval);
	}
}

