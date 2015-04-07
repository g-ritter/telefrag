/*
 * _errorlog.cpp
 *
 * a bunch of functions for logging and handling errors
 *
 *  @since	Feb 17, 2015
 *  @author	g ritter <info@rabbitgirl.me>
 */

#include <fstream>
#include <iostream>
#include <string>
#include "_stringfxns.h"

/**
 * A quick and easy function for for outputting an arbitrary error message
 * with the date and time. TODO actually test this lmao
 */
namespace errorlog {

	void log(std::string message) {
		std::ofstream outfile;
		outfile.open("test.txt", std::ios_base::app);
		time_t t = time(0);
		struct tm * now = localtime(&t);

		std::string time = "[";
		time += std::to_string(now->tm_year + 1900) + '/';
		if (now->tm_mon < 9) {
			time += '0' + std::to_string(now->tm_mon + 1) + '/';
		} else {
			time += std::to_string(now->tm_mon + 1) + '/';
		}
		if (now->tm_mday < 10) {
			time += '0' + std::to_string(now->tm_mday) + ':';
		} else {
			time += std::to_string(now->tm_mday) + ':';
		}
		if (now->tm_hour < 10) {
			time += '0' + std::to_string(now->tm_hour) + ':';
		} else {
			time += std::to_string(now->tm_hour) + ':';
		}
		if (now->tm_min < 10) {
			time += '0' + std::to_string(now->tm_min) + ':';
		} else {
			time += std::to_string(now->tm_min) + ':';
		}
		if (now->tm_sec < 10) {
			time += '0' + std::to_string(now->tm_sec) + "] ";
		} else {
			time += std::to_string(now->tm_sec) + "] ";
		}

		outfile << time << message << std::endl;
		outfile.close();
		std::cout << time << message << std::endl;
	}
}
