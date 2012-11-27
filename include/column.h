#ifndef __LORM_COLUMN_H
#define __LORM_COLUMN_H

#include <sstream>
#include <iostream>

#include "types.h"
#include "datetime.h"

class abstract_column  {
public:
  virtual std::string as_sql_litteral() = 0;
  virtual std::string where(const std::string & name)= 0;
  virtual std::string sql_delimiter() = 0;
  virtual bool is_null() const = 0;
};

template <class T> class column : public abstract_column {
  public:
    virtual std::string as_sql_litteral() {throw "as_sql_litteral() should be defined !!";}
    virtual std::string sql_delimiter() {return "";}
    column<T>(T* v): value(v), min_(NULL), max_(NULL), like_(NULL) {};
    column<T>(): value(NULL), min_(NULL), max_(NULL), like_(NULL) {};

    bool is_null() const {
      return NULL == value;
    }

    void operator=(const T& v) {
      value = new T(v);
    }

    void operator<(const T& v) {
      max_ = new T(v);
    }

    void operator>(const T& v) {
      min_ = new T(v);
    }

    bool operator==(const T& v) const {
      if(NULL != value) {
        return *value == v;
      }
      return false;
    }

    bool operator!=(const T& v) const {
      return !(*this==v);
    }
  
    bool operator==(const column<T>& v) const {
      if(NULL != value) {
        return *value == *(v.value);
      }
      return false;
    }

    void between(const T& m, const T& n) {
      min_ = new T(m);
      max_ = new T(n);
    }

    void like(const T& v) {
      like_ = new T(v);
    }

    virtual std::string where(const std::string & name) {
      std::stringstream where;

      if(NULL != value) {
        where << name << " = " << sql_delimiter() << *value << sql_delimiter();
      } else if(NULL != like_) {
        where << name << " LIKE " << sql_delimiter() << *like_ << sql_delimiter();
      } else {
        std::string ac = "";
        if(NULL != min_) {
          where << name << " > " << sql_delimiter() << *min_ << sql_delimiter();
          ac = " AND ";
        }
        if(NULL != max_) {
          where << ac << name << " < " << sql_delimiter() << *max_ << sql_delimiter();
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

// Specializations of abstract entry-points, type by type
template <> extern std::string column<int>::as_sql_litteral();
template <> extern std::string column<std::string>::sql_delimiter();
template <> extern std::string column<datetime>::sql_delimiter();
template <> extern std::string column<std::string>::as_sql_litteral();
template <> extern std::string column<double>::as_sql_litteral();
template <> extern std::string column<datetime>::as_sql_litteral();
#endif // __LORM_COLUMN_H

