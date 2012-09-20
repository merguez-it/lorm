#ifndef __LORM_UTIL_H
#define __LORM_UTIL_H

#include <string>
#include <vector>
#include <algorithm>

std::string lower(const std::string & data);
std::string pluralize(std::string singular);
std::vector<std::string> explode(std::string s, std::string e);

#endif // __LORM_UTIL_H

