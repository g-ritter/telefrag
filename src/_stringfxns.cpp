/*
 * _stringfxns.cpp
 *
 *  @since	Nov 25, 2014
 *  @author	g ritter <info@rabbitgirl.me>
 */

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * A namespace with a set of home-made functions for processing, reading, and
 * manipulating strings.
 */
namespace stringfxns {

// Splits a string according to an input and returns a vector containing each
// of the new strings
std::vector<std::string> splitString(std::string input, std::string split);

// Attempts to read an integer value from a string
int stringToInt(std::string input);

// Correctly formats a path string to work on unix or windows systems.
std::string setPathDelimiters(std::string input);

}

/**
 * Splits an input string at every occurence of a second input, and collects
 * all the resulting strings into a vector.
 *
 * @param input The string to be split
 * @param split The string that the input is split by
 * @return A vector containing all the split stringss
 */
std::vector<std::string> stringfxns::splitString(std::string input,
		std::string split) {
	if (input == "") {
		return std::vector<std::string>();
	}
	std::vector<std::string> result;
	std::string current;
	bool newString = false;
	for (int pos = 0; pos < input.length(); pos++) {
		if (input[pos] == split[0]) {
			for (int i = 0; i < split.length() && pos + i < input.length();
					i++) {
				if (input[pos + i] == split[i]) {
					newString = true;
				} else {
					current += input[pos];
					newString = false;
					break;
				}
			}
		} else {
			current += input[pos];
			newString = false;
		}
		if (newString) {
			result.push_back(current);
			current = "";
			pos += (split.length() - 1);
		}
	}
	result.push_back(current);
	return result;
}

/**
 * Parses a string and attempts to create an int. Fatally crashes if it gets
 * passed a string containing anything besides numbers and an optional
 * negative sign.
 *
 * @param input The string to be parsed
 * @return An int representative of this string.
 */
int stringfxns::stringToInt(std::string input) {
	int result = 0;
	int decimalPlaces = 0;
	bool negative;
	unsigned int i = 0;
	if (input[0] == '-') {
		negative = true;
		decimalPlaces = input.size() - 1;
		i++;
	} else {
		negative = false;
		decimalPlaces = input.size() - 1;
	}
	for (; i < input.size(); i++) {
		int r = input[i] - '0';
		if (r > 9 || r < 0) {
			throw std::runtime_error(
					"Function stringToInt for input " + input
							+ " contains non-number or - characters, returning 0");
			return 0;
		} else {
			result += r * pow(10, decimalPlaces - i);
		}
	}
	if (negative) {
		return -result;
	}
	return result;
}

/**
 * Correctly formats an string representation of a file path to work on this
 * file system. Currently only works for Windows and Unix systems.
 *
 * CONCERN Add support for other delimiters?
 *
 * @param input The string to be modified.
 * @return A string compatible with this file system.
 */
std::string stringfxns::setPathDelimiters(std::string input) {
#ifdef _WIN32
	const char DELIM = '\\';
	const char NOT_DELIM = '/';
#else
	const char DELIM = '/';
	const char NOT_DELIM = '\\';
#endif
	for (char &c : input) {
		if (c == NOT_DELIM) {
			c = DELIM;
		}
	}
	return input;
}
