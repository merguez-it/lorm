/*
 *  address.cpp
 *  LORM
 *
 *  Created by Mathias Franck on 06/10/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "address.h"
#include "person.h"

REGISTER_TABLE(Address) {
  register_identity("id", &Address::id); // ID
  register_field("zip", &Address::zip,0,false); // NOT_NULL
  register_field("street", &Address::street, false);
  register_field("city", &Address::city,false);
}

has_many(Address,Person,residents,address)