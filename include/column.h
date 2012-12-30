#ifndef __LORM_COLUMN_H
#define __LORM_COLUMN_H

#include <sstream>
#include <iostream>

#include "types.h"
#include "datetime.h"

class abstract_column  {
public:
  virtual std::string as_sql_litteral() const = 0;
  virtual std::string where(const std::string & name) const = 0;
  virtual std::string sql_delimiter() const = 0;
  virtual bool is_null() const = 0;
};

// Invariant : T is NOT a "pointer" type

template <typename T> class column : public abstract_column {
public:
  
	virtual std::string as_sql_litteral() const {throw "as_sql_litteral() should be defined !!";}
  virtual std::string sql_delimiter() const {return "";}
  
	column<T>(const T& v): value_(v), value_is_null(false), min_is_null(true), max_is_null(true), like_is_null(true) {};
  column<T>(): value_is_null(true), min_is_null(true), max_is_null(true), like_is_null(true) {};
	
  const T& value() const { 
		if (value_is_null) {
			throw "Access to null value";
		}
		return value_;
	}
	
	bool is_null() const {
    return value_is_null;
  }
  
  column<T>& operator=(const T& v) {
    value_=v;
		value_is_null=false;
    return *this;
  }
    
  void operator<(const T& v) {
    max_=v;
		max_is_null=false;
  }
  
  void operator>(const T& v) {
    min_=v;
		min_is_null=false;
  }
  
  bool operator==(const T& v) const {
    if(!value_is_null) {
      return value_ == v;
    }
    return false;
  }
  
  bool operator!=(const T& v) const {
    return !(*this==v);
  }
  
  bool operator==(const column<T>& v) const {
    if(!value_is_null) {
      return value_ == v.value_;
    }
    return v.value_is_null;
  }
  
  void between(const T& m, const T& n) { // m and n bounds are excluded from interval...bof.
    min_=m;
		min_is_null=false;
    max_=n;
		max_is_null=false;
  }
  
  void like(const T& v) {
    like_=v;
		like_is_null=false;
  }
  
  virtual std::string where(const std::string & name) const {
    std::stringstream where;
    
    if(!value_is_null) {
      where << name << " = " << sql_delimiter() << value_ << sql_delimiter();
    } else if(!like_is_null) {
      where << name << " LIKE " << sql_delimiter() << like_ << sql_delimiter();
    } else {
      std::string ac = "";
      if(!min_is_null) {
        where << name << " > " << sql_delimiter() << min_ << sql_delimiter();
        ac = " AND ";
      }
      if(!max_is_null) {
        where << ac << name << " < " << sql_delimiter() << max_ << sql_delimiter();
      }
    }
    
    return where.str();
  }
  
  void reset() {
    value_is_null=true;
    min_is_null=true;
    max_is_null=true;
    like_is_null=true;
  }

protected:
	T value_;
  T min_;
  T max_;
  T like_;
	bool value_is_null:1;
	bool min_is_null:1;
	bool max_is_null:1;
	bool like_is_null:1;
};

template <class T> std::ostream& operator<< (std::ostream& os, const column<T>& obj) {
  if(!obj.is_null()) {
    os << obj.value();
  } else {
    os << "NULL";
  }
  return os;
}

//template <typename T> class column : public abstract_column {
//public:
//  virtual std::string as_sql_litteral() const {throw "as_sql_litteral() should be defined !!";}
//  virtual std::string sql_delimiter() const {return "";}
//  column<T>(T* v): value(v), min_(NULL), max_(NULL), like_(NULL) {};
//  column<T>(): value(NULL), min_(NULL), max_(NULL), like_(NULL) {};
//  
//  bool is_null() const {
//    return NULL == value;
//  }
//  
//  column<T>& operator=(const T& v) {
//    clean_new(value,v);
//    return *this;
//  }
//  
//  column<T>& operator= (const column<T>& v) {
//    smart_copy(value,v.value);
//    smart_copy(min_,v.min_);
//    smart_copy(max_,v.max_);
//    smart_copy(like_, v.like_); 
//    return *this;
//  }
//  
//  
//  void operator<(const T& v) {
//    clean_new(max_,v);
//  }
//  
//  void operator>(const T& v) {
//    clean_new(min_,v);
//  }
//  
//  bool operator==(const T& v) const {
//    if(NULL != value) {
//      return *value == v;
//    }
//    return false;
//  }
//  
//  bool operator!=(const T& v) const {
//    return !(*this==v);
//  }
//  
//  bool operator==(const column<T>& v) const {
//    if(NULL != value) {
//      return *value == *(v.value);
//    }
//    return NULL!=v.value;
//  }
//  
//  void between(const T& m, const T& n) {
//    clean_new(min_,m);
//    clean_new(max_,n);
//  }
//  
//  void like(const T& v) {
//    clean_new(like_,v);
//  }
//  
//  virtual std::string where(const std::string & name) const {
//    std::stringstream where;
//    
//    if(NULL != value) {
//      where << name << " = " << sql_delimiter() << *value << sql_delimiter();
//    } else if(NULL != like_) {
//      where << name << " LIKE " << sql_delimiter() << *like_ << sql_delimiter();
//    } else {
//      std::string ac = "";
//      if(NULL != min_) {
//        where << name << " > " << sql_delimiter() << *min_ << sql_delimiter();
//        ac = " AND ";
//      }
//      if(NULL != max_) {
//        where << ac << name << " < " << sql_delimiter() << *max_ << sql_delimiter();
//      }
//    }
//    
//    return where.str();
//  }
//  
//  void reset() {
//    safe_delete(value);
//    safe_delete(min_);
//    safe_delete(max_);
//    safe_delete(like_);
//  }
//  
//public:
//  T *value;
//  
//protected:
//  T *min_;
//  T *max_;
//  T *like_; 
//    
//  inline void clean_new(T*& member, const T& v) {
//    if (member) delete member;
//    member = new T(v);
//  }
//  
//  inline void safe_delete(T* &member) {
//    if (member) {
//      delete member;
//      member = NULL;
//    }
//  }
//                          
//  inline void smart_copy(T* &member, const T* that_member) {
//    if (that_member) {
//      clean_new(member,*that_member);
//    } else{
//      safe_delete(member);
//    }
//  }
//  
//};
//
//template <class T> std::ostream& operator<< (std::ostream& os, const column<T>& obj) {
//  if(NULL != obj.value) {
//    os << *obj.value;
//  } else {
//    os << "NULL";
//  }
//  return os;
//}

// Specializations of abstract entry-points, type by type
template <> extern std::string column<int>::as_sql_litteral() const;
template <> extern std::string column<std::string>::sql_delimiter() const;
template <> extern std::string column<datetime>::sql_delimiter() const;
template <> extern std::string column<std::string>::as_sql_litteral() const;
template <> extern std::string column<double>::as_sql_litteral() const;
template <> extern std::string column<datetime>::as_sql_litteral () const;
#endif // __LORM_COLUMN_H

