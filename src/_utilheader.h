/*
 * _utilheader.h
 *
 * Contains a bunch of global variables, enums, etc. that are used widely
 * throughout the project and mostly SHOULDN'T be modified at runtime.
 *
 *  @since	Apr 1, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#ifndef UTILHEADER_H_
#define UTILHEADER_H_

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <SDL2/SDL.h>

// The actual width and height of the game screen
const static int GAME_WIDTH = 420;
const static int GAME_HEIGHT = 300;
// The factor by which the game window and all graphics inside of it are
// scaled; necessarily overrides the GAME_WIDTH and _HEIGHT variables.
const static int WINDOW_SCALE = 2;
// The value added to every physics based object's downward velocity per-frame.
// May be further modified or offset based on the object.
const static double GRAVITY = 1.7;
// The upwards velocity a player gets when they jump. SHOULD BE NEGATIVE.
const static double JUMP_HEIGHT = -16;

// Enums for different ways a player could potentially die
enum CauseOfDeath {
	WARP_INTO_WALL,
	CUT_BY_PLAYER
};

// A map of CauseOfDeath s to appropriate strings for printing or displaying
// death messages.
const static std::map<CauseOfDeath, std::string> codMsg {std::pair<CauseOfDeath,
		std::string>(WARP_INTO_WALL, "FROM WARPING INTO A WALL."), std::pair<
		CauseOfDeath, std::string>(CUT_BY_PLAYER, "BY RUNNING INTO A SWORD.")};

// A set of random strings to tack onto death messages.
const static std::vector<std::string> deathMsg {"RIP, BUDDY.",
		"BETTER LUCK NEXT TIME.", "HOW'D YOU MANAGE THAT?", "NICE ONE, DORK.",
		"I THINK WE BOTH KNOW YOU CAN DO BETTER.", "WHAT A NERD!"};

// A bunch of typedefs for geometry handling
typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::segment<Point> Segment;
typedef boost::geometry::model::polygon<Point> Polygon;
typedef boost::geometry::model::linestring<Point> linestring;
typedef boost::geometry::model::box<Point> Box;

// The resource path of this project. Set at runtime.
static std::string respath;

// ALL of the keycodes referenced throughout the project. TODO Should maybe be able
// to be modified in the game?
const static SDL_Keycode CONFIG_p1up = SDLK_w;
const static SDL_Keycode CONFIG_p1down = SDLK_s;
const static SDL_Keycode CONFIG_p1left = SDLK_a;
const static SDL_Keycode CONFIG_p1right = SDLK_d;
const static SDL_Keycode CONFIG_p1shortwarp = SDLK_r;
const static SDL_Keycode CONFIG_p1longwarp = SDLK_t;
const static SDL_Keycode CONFIG_p1attack = SDLK_f;
const static SDL_Keycode CONFIG_p1jump = SDLK_g;

const static SDL_Keycode CONFIG_p2up = SDLK_o;
const static SDL_Keycode CONFIG_p2down = SDLK_l;
const static SDL_Keycode CONFIG_p2left = SDLK_k;
const static SDL_Keycode CONFIG_p2right = SDLK_SEMICOLON;
const static SDL_Keycode CONFIG_p2shortwarp = SDLK_LEFTBRACKET;
const static SDL_Keycode CONFIG_p2longwarp = SDLK_RIGHTBRACKET;
const static SDL_Keycode CONFIG_p2attack = SDLK_QUOTE;
const static SDL_Keycode CONFIG_p2jump = SDLK_BACKSLASH;

const static SDL_Keycode CONFIG_escape = SDLK_ESCAPE;
const static SDL_Keycode CONFIG_confirm = SDLK_RETURN;
const static SDL_Keycode CONFIG_cancel = SDLK_s;

#endif /* UTILHEADER_H_ */
