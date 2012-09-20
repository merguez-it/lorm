#ifndef __PERSON_H
#define __PERSON_H

#include "lorm.h"

class Person : public table<Person> {
  public:
    TABLE_INIT(Person);

    column<int> id;
    column<int> num;
    column<std::string> name;
    column<std::string> desc;
    column<double> age;

//    group<Person> friends; 
//    column<Address> address;
//    group<Parent> parents;
};

#endif // __PERSON_H
