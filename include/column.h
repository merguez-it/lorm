#ifndef __LORM_COLUMN_H
#define __LORM_COLUMN_H

#include <sstream>
#include <iostream>

template <class T> class column {
  public:
    column<T>(T* v): value(v), min_(NULL), max_(NULL), like_(NULL) {};
    column<T>(): value(NULL), min_(NULL), max_(NULL), like_(NULL) {};

    bool is_null() const {
      return NULL == value;
    }

    void operator=(T v) {
      value = new T(v);
    }

    void operator<(T v) {
      max_ = new T(v);
    }

    void operator>(T v) {
      min_ = new T(v);
    }

    void between(T m, T n) {
      min_ = new T(m);
      max_ = new T(n);
    }

    void like(T v) {
      like_ = new T(v);
    }

    std::string where(const std::string & name, const std::string & sep) {
      std::stringstream where;

      if(NULL != value) {
        where << name << " = " << sep << *value << sep;
      } else if(NULL != like_) {
        where << name << " LIKE " << sep << *like_ << sep;
      } else {
        std::string ac = "";
        if(NULL != min_) {
          where << name << " > " << sep << *min_ << sep;
          ac = " AND ";
        }
        if(NULL != max_) {
          where << ac << name << " < " << sep << *max_ << sep;
        }
      }

      return where.str();
    }

    void reset() {
      value = NULL;
      min_ = NULL;
      max_ = NULL;
      like_ = NULL;
    }

  public:
    T *value;

  private:
    T *min_;
    T *max_;
    T *like_;
};

template <class T> std::ostream& operator<< (std::ostream& os, const column<T>& obj) {
  if(NULL != obj.value) {
    os << *obj.value;
  } else {
    os << "NULL";
  }
  return os;
}

#endif // __LORM_COLUMN_H

