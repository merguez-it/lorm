#ifndef __UTIL_TYPES_H
#define __UTIL_TYPES_H

#include <sstream>

namespace util {
  template <typename T> inline bool value_of(const std::string & data, T & obj) {
    std::istringstream is(data);
    return (is >> obj).eof();
  }
  template <typename T> inline bool value_of(const std::string & data, std::string & obj) {
    obj = data;
    return true;
  }
  template <typename T> inline T from_string(const std::string & data) {
    T i;
    if(value_of<T>(data, i)) {
      return i;
    }
    return 0; // O_o ??? a 0 string  is "empty", but what about int, float, or bool ? 
  }
  template <typename T> inline std::string to_string(const T & data) {
    std::ostringstream os;
    os << data;
    return os.str();
  }

  template <typename F, typename T> T convert(F data) {
    std::string tmp = to_string<F>(data);
    return from_string<T>(tmp);
  }
}

#endif // __UTIL_TYPES_H

