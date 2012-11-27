#ifndef __PERSON_H
#define __PERSON_H

#include "macro_utils.h"
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
    HAS_ONE(Address,address)
    HAS_ONE(Address,bureau,office_id)
};

#endif // __PERSON_H
