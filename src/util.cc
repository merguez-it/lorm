#include "util.h"

std::string lower(const std::string & data) {
  std::string s(data);
  std::transform(s.begin(), s.end(), s.begin(), tolower);
  return s;
}

#define MAP(suffix, plural) \
  if (singular.length() >= std::string(suffix).length()) { \
    if (singular.substr(singular.length() - std::string(suffix).length(), std::string(suffix).length()) == std::string(suffix)) { \
      return singular.substr(0, singular.length() - std::string(suffix).length()) + plural; \
    } \
  }

/* NOTE: We assume all lower case! */
std::string pluralize(std::string singular) {
  /* Irregular */
  MAP("person", "people");
  MAP("man", "men");
  MAP("child", "children");
  MAP("sex", "sexes");
  MAP("move", "moves");
  MAP("cow", "kine");
  MAP("zombie", "zombies");

  /* Mappings */
  MAP("quiz", "quizzes");
  if (singular == "oxen") {
    return "oxen";
  }
  if (singular == "ox") {
    return "oxen";
  }
  MAP("mice", "mice");
  MAP("lice", "lice");
  MAP("mouse", "mice");
  MAP("louse", "lice");
  MAP("matrix", "matrices");
  MAP("vertix", "vertices");
  MAP("indix", "indices");
  MAP("matrex", "matrices");
  MAP("vertex", "vertices");
  MAP("index", "indices");
  MAP("x", "xes");
  MAP("ch", "ches");
  MAP("ss", "sses");
  MAP("sh", "shes");
  if (singular.length() >= 2) {
    if (singular.substr(singular.length() - 1, 1) == "y") {
      if (singular.substr(singular.length() - 2, 1) != "a" &&
          singular.substr(singular.length() - 2, 1) != "e" &&
          singular.substr(singular.length() - 2, 1) != "i" &&
          singular.substr(singular.length() - 2, 1) != "o" &&
          singular.substr(singular.length() - 2, 1) != "u" &&
          singular.substr(singular.length() - 2, 1) != "y") {
        return singular.substr(0, singular.length() - 1) + "ies";
      }
    }
  }
  MAP("quy", "quies");
  MAP("hive", "hives");
  if (singular.length() >= 3) {
    if (singular.substr(singular.length() - 2, 2) == "fe") {
      if (singular.substr(singular.length() - 2, 1) != "f") {
        return singular.substr(0, singular.length() - 2) + "ves";
      }
    }
  }
  MAP("lf", "lves");
  MAP("rf", "rves");
  MAP("sis", "ses");
  MAP("ta", "ta");
  MAP("ia", "ia");
  MAP("tum", "ta");
  MAP("ium", "ia");
  MAP("buffalo", "buffaloes");
  MAP("tomato", "tomatoes");
  MAP("bus", "buses");
  MAP("alias", "aliases");
  MAP("status", "statuses");
  MAP("octopi", "octopii");
  MAP("viri", "virii");
  MAP("octopus", "octopuses");
  MAP("virus", "viruses");
  MAP("axis", "axes");
  MAP("testis", "testes");
  MAP("s", "s");
  return singular + "s";
}

std::vector<std::string> explode(std::string s, std::string e) {
  std::vector<std::string> ret;
  int iPos = s.find(e, 0);
  int iPit = e.length();
  while(iPos>-1) {
    if(iPos!=0)
      ret.push_back(s.substr(0,iPos));
    s.erase(0,iPos+iPit);
    iPos = s.find(e, 0);
  }
  if(s!="")
    ret.push_back(s);
  return ret;
};

std::pair<std::string, std::string> cut(std::string data, const std::string & sep, cut_keep keep) {
  std::string first_;
  std::string second_;
  int keep_left_ = 0;
  int keep_right_ = sep.size();

  switch(keep) {
    case KEEP_LEFT:
      keep_left_ = sep.size();
      keep_right_ = sep.size();
      break;
    case KEEP_RIGHT:
      keep_left_ = 0;
      keep_right_ = 0;
      break;
    case KEEP_BOOTH:
      keep_left_ = sep.size();
      keep_right_ = 0;
      break;
    case KEEP_NONE:
    default:
      keep_left_ = 0;
      keep_right_ = sep.size();
      break;
  }

  std::string::iterator pos_ = std::search(data.begin(), data.end(), sep.begin(), sep.end());
  int dis_ = std::distance(data.begin(), pos_);
  if(dis_ >= data.size()) {
    first_ = data;
    second_ = "";
  } else {
    first_ = std::string(data.begin(), data.begin()+dis_+keep_left_);
    second_ = std::string(data.begin()+dis_+keep_right_, data.end());
  }

  return std::make_pair(first_, second_);
}
