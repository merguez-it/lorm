#include "uri.h"
#include "util.h"
#include <sstream>
URI::URI() {}

URI::URI(const std::string & str) : uri_(str) {}

URI URI::parse(const std::string & str) {
  return URI(str).parse();
}

std::string URI::encode(const std::string & str) {
  return URI(str).encode();
}

std::string URI::decode(const std::string & str) {
  return URI(str).decode();
}

#include <iostream>
URI URI::parse() {
  std::pair<std::string, std::string> scheme_and_rest_ = cut(uri_, "://");
  scheme_ = scheme_and_rest_.first;

  std::pair<std::string, std::string> uhp_and_pq_ = cut(scheme_and_rest_.second, "/", KEEP_RIGHT);

  std::pair<std::string, std::string> user_and_hp_ = cut(uhp_and_pq_.first, "@");
  std::string user_info_ = user_and_hp_.first;
  std::string host_port_ = user_and_hp_.second;
  if(0 == host_port_.size()) {
    user_info_ = "";
    host_port_ = user_and_hp_.first;
  }

  std::pair<std::string, std::string> host_and_port_ = cut(host_port_, ":");
  host_ = host_and_port_.first;
  if(0 < host_and_port_.second.size()) {
    std::stringstream stream_(host_and_port_.second);
    stream_ >> port_;
  } else {
    port_ = 0;
  }

  std::pair<std::string, std::string> user_and_password_ = cut(user_info_, ":");
  username_ = user_and_password_.first;
  password_ = user_and_password_.second;

  std::pair<std::string, std::string> path_and_query_ = cut(uhp_and_pq_.second, "?");
  path_ = path_and_query_.first;
  query_string_ = path_and_query_.second;

  std::vector<std::string> params_ = explode(query_string_, "&");
  std::vector<std::string>::iterator it;
  for(it = params_.begin(); it != params_.end(); it++) {
    std::pair<std::string, std::string> param_ = cut(*it, "=");
    query_[param_.first] = param_.second;
  }

  return *this;
}

const char SAFE[256] = {
  /*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
  /* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,

  /* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
  /* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
  /* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
  /* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,

  /* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

  /* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
  /* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

std::string URI::encode() const {
  const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
  const unsigned char * pSrc = (const unsigned char *)uri_.c_str();
  const int SRC_LEN = uri_.length();
  unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
  unsigned char * pEnd = pStart;
  const unsigned char * const SRC_END = pSrc + SRC_LEN;

  for (; pSrc < SRC_END; ++pSrc) {
    if (SAFE[*pSrc]) { 
      *pEnd++ = *pSrc;
    } else {
      // escape this char
      *pEnd++ = '%';
      *pEnd++ = DEC2HEX[*pSrc >> 4];
      *pEnd++ = DEC2HEX[*pSrc & 0x0F];
    }
  }

  std::string sResult((char *)pStart, (char *)pEnd);
  delete [] pStart;
  return sResult;
}

const char HEX2DEC[256] = {
  /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
  /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

  /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

  /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

  /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
  /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

std::string URI::decode() const {
  const unsigned char * pSrc = (const unsigned char *)uri_.c_str();
  const int SRC_LEN = uri_.length();
  const unsigned char * const SRC_END = pSrc + SRC_LEN;
  const unsigned char * const SRC_LAST_DEC = SRC_END - 2;   // last decodable '%' 

  char * const pStart = new char[SRC_LEN];
  char * pEnd = pStart;

  while (pSrc < SRC_LAST_DEC) {
    if (*pSrc == '%') {
      char dec1, dec2;
      if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)]) && -1 != (dec2 = HEX2DEC[*(pSrc + 2)])) {
        *pEnd++ = (dec1 << 4) + dec2;
        pSrc += 3;
        continue;
      }
    }

    *pEnd++ = *pSrc++;
  }

  // the last 2- chars
  while (pSrc < SRC_END) {
    *pEnd++ = *pSrc++;
  }

  std::string sResult(pStart, pEnd);
  delete [] pStart;
  return sResult;
}

const std::string URI::scheme() const {
  return scheme_;
}

const std::string URI::host() const {
  return host_;
}

const int URI::port() const {
  return port_;
}

const std::string URI::path() const {
  return path_;
}

const std::string URI::query_string() const {
  return query_string_;
}

std::map<std::string, std::string> URI::query() const {
  return query_;
}

const std::string URI::username() const {
  return username_;
}

const std::string URI::password() const {
  return password_;
}


