#ifndef __REFERENCE_H
#define __REFERENCE_H
/*
 *  reference.h
 *  LORM
 *
 *  Created by Mathias Franck on 27/11/12.
 *  Copyright 2012 Merguez Factory. All rights reserved.
 *  
 *  This class implements a reference to "foreign" object stored in a database 
 *  mapped "table<T>", linked in the context of a "to-one" relationship.
 *  It supports a semantic for "empty" relationship and lazy loading.
 *
 *  It overloads operator(), in order to allow a simple syntax for set/get e.g:
 *  john.address(anAddress); 
 *  john.address().street="...";
 *  
 */
#include <memory>
#include "column.h"


template < class FOREIGN_CLASS > 
class reference : public column<int> {
public:  

  reference<FOREIGN_CLASS>& operator=(const column<int>& col) {
    column<int>::operator=(col);
    return *this;
  }
  
  bool exists() {
    return value!=NULL;
  }
  
  FOREIGN_CLASS& operator() (bool force_reload=false) {
    return get_role(force_reload);
  }
  
  void operator() (const FOREIGN_CLASS &that_role) {
     set_role(that_role);
  }
    
private:
  std::tr1::shared_ptr<FOREIGN_CLASS> role;
  
  FOREIGN_CLASS &get_role (bool force_reload=false) {
    if (!role.get() || force_reload) {
      role.reset(new FOREIGN_CLASS());
      *role=FOREIGN_CLASS::search_by_id(value);
    }
    return *role;
  }
  
  void set_role (const FOREIGN_CLASS &that_role) {
    if (NULL==that_role.id.value) {
      throw "Identity not set !!";
    }
    *(column<int> *)this = *(that_role.id.value);
    FOREIGN_CLASS *p=new FOREIGN_CLASS(that_role);
    role.reset(p);
  }
  
};
#endif
