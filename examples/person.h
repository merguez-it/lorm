#ifndef __PERSON_H
#define __PERSON_H

#include "lorm.h"
#include "Address.h"

class Person : public table<Person> {
  public:
    TABLE_INIT(Person);
    column<int> id;
    column<int> num;
    column<std::string> name;
    column<std::string> desc;
    column<double> age;
    column<datetime> birthday;
    REFERENCE(Address, address);
    REFERENCE(Address, bureau);
};

#endif // __PERSON_H
