#ifndef __LORM_UTIL_H
#define __LORM_UTIL_H

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

std::string lower(const std::string & data);
std::string pluralize(std::string singular);
std::vector<std::string> explode(std::string s, std::string e);

enum cut_keep {
  KEEP_LEFT,
  KEEP_RIGHT,
  KEEP_BOOTH,
  KEEP_NONE
};
std::pair<std::string, std::string> cut(std::string data, const std::string & sep, cut_keep keep = KEEP_NONE);

#endif // __LORM_UTIL_H

