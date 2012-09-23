#ifndef __LORM_COLUMN_H
#define __LORM_COLUMN_H

#include <sstream>
#include <iostream>

#include "types.h"

class abstract_column  {
public:
  virtual std::string as_sql_litteral() = 0;
  virtual std::string where(const std::string & name, const std::string & sep)=0;
  virtual std::string sql_delimiter() =0;
};

template <class T> class column : public abstract_column {
  public:
    virtual std::string as_sql_litteral() {throw 1;}
    virtual std::string sql_delimiter() {return "";}
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

    virtual std::string where(const std::string & name, const std::string & sep) {
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

// Specializations of abstract "as_sql_litteral()", type by type
template <> std::string column<int>::as_sql_litteral() {
  if (!is_null()) {
    return sql_delimiter() + util::to_string<int>(*value) + sql_delimiter();
  }
  return sql_delimiter()+sql_delimiter();
}

template <> std::string column<std::string>::sql_delimiter() {
  return "'";
}

template <> std::string column<std::string>::as_sql_litteral() {
  if (!is_null()) {
    return sql_delimiter()+(*value)+sql_delimiter();
  }
  return sql_delimiter()+sql_delimiter();
}

template <> std::string column<double>::as_sql_litteral() {
  if (!is_null()) {
    return sql_delimiter()+util::to_string<double>(*value);
  }
  return sql_delimiter()+sql_delimiter();
}

#endif // __LORM_COLUMN_H

