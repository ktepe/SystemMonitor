#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // DONE: See src/format.cpp
std::vector<std::string> Split (const std::string &s, char delim); // ADDED.
}
#endif