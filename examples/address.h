/*
 *  address.h
 *  LORM
 *
 *  Created by Mathias Franck on 06/10/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __ADDRESS_H
#define __ADDRESS_H

#include "lorm.h"

class Address : public table<Address> {
public:
  TABLE_INIT(Address);
  column<int> id;
  column<std::string> street;
  column<int> zip;
  column<std::string> city;
};

#endif // __PERSON_H