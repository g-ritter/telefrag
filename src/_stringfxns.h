/*
 * _stringfxns.h
 *
 *  @since	Nov 25, 2014
 *  @author	g ritter <info@rabbitgirl.me>
 */

#ifndef STRINGFXNS_H_
#define STRINGFXNS_H_

#include <vector>

/**
 * A namespace with a set of home-made functions for processing, reading, and
 * manipulating strings.
 */
namespace stringfxns {

// Splits a string according to an input and returns a vector containing each
// of the new strings
std::vector<std::string> splitString(std::string input, std::string split);

// Attempts to read an integer value from a string.
int stringToInt(std::string input);

// Correctly formats a path string to work on unix or windows systems.
std::string setPathDelimiters(std::string input);

}

#endif /* STRINGFXNS_H_ */
