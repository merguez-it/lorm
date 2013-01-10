/*
 *  column.cpp
 *  LORM
 *
 *  Created by Mathias Franck on 26/11/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "column.h"

// Specializations of abstract entry-points, type by type
template <> std::string column<bool>::as_sql_litteral() const {
  if (!is_null()) {
    return sql_delimiter() + util::to_string<bool>(value_) + sql_delimiter();
  }
  return sql_delimiter()+sql_delimiter();
}

template <> std::string column<int>::as_sql_litteral() const {
  if (!is_null()) {
    return sql_delimiter() + util::to_string<int>(value_) + sql_delimiter();
  }
  return sql_delimiter()+sql_delimiter();
}

template <> std::string column<std::string>::sql_delimiter() const {
  return "'";
}

template <> std::string column<datetime>::sql_delimiter() const {
  return "'";
}

template <> std::string column<std::string>::as_sql_litteral() const {
  if (!is_null()) {
    return sql_delimiter()+value_+sql_delimiter();
  }
  return sql_delimiter()+sql_delimiter();
}

template <> std::string column<double>::as_sql_litteral() const{
  if (!is_null()) {
    return sql_delimiter()+util::to_string<double>(value_)+sql_delimiter();
  }
  return sql_delimiter()+sql_delimiter();
}

template <> std::string column<datetime>::as_sql_litteral() const {
  if (!is_null()) {
    std::stringstream litteral;
    litteral << sql_delimiter();
    litteral << value_;
    litteral << sql_delimiter();
    
    return litteral.str();
  }
  return sql_delimiter()+sql_delimiter();
}