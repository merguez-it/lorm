#include "gtest/gtest.h"
#include "lorm.h"

class Address : public table<Address> {
public:
  TABLE_INIT(Address);
  column<int> id;
  column<std::string> street;
  column<int> zip;
  column<std::string> city;
};

REGISTER_TABLE(Address) {
  identity("id", &Address::id); // ID
  field("zip", &Address::zip,0,false); // NOT_NULL
  field("street", &Address::street, false);
  field("city", &Address::city,false);
}

class Person : public table<Person> {
  public:
    TABLE_INIT(Person);
    column<int> id;
    column<int> num;
    column<std::string> name;
    column<std::string> desc;
    column<double> age;
    column<datetime> birthday;
    reference<Address> address;
    reference<Address> office;
};

REGISTER_TABLE(Person) {
  identity("id", &Person::id); // ID
  field("num", &Person::num, 123, false); // NOT_NULL
  field("name", &Person::name, false);
  field("desc", &Person::desc, std::string("guest"));
  field("age", &Person::age, 1.2);
  field("birthday", &Person::birthday, datetime("1967-06-26 00:00:00"));
  has_one("address_id", &Person::address);
  has_one("bureauId", &Person::office);
}

TEST(To_one, create_retrieve_two_relationships) {
  Lorm::connect("sqlite://:memory:");
  Person::create();
  Address::create();
  // -- create
  Address john_home;
  john_home.street="14 Rue de la Vieille Aventure";
  john_home.zip=59000;
  john_home.city="Lille";
  john_home=john_home.save();
  ASSERT_TRUE(!john_home.id.is_null());
  
  Address john_office;
  john_office.street="21 Rue Camille-Desmoulins";
  john_office.zip=92789;
  john_office.city="Issy-Les-Moulineaux";
  john_office=john_office.save();
  ASSERT_TRUE(!john_office.id.is_null());

  Person john;
  john.name = "john";
  john.address(john_home);
  john.office(john_office);
  john = john.save();
    
  Person checkForJohnAddress;
  checkForJohnAddress=Person::search_by_id(john.id);
  ASSERT_TRUE(checkForJohnAddress.id==john.id);
  EXPECT_EQ(john_home.street,checkForJohnAddress.address().street);
  EXPECT_EQ(john_home.zip,checkForJohnAddress.address().zip);
  EXPECT_EQ(john_home.city,checkForJohnAddress.address().city);
  EXPECT_EQ(john_office.street,checkForJohnAddress.office().street);
  EXPECT_EQ(john_office.zip,checkForJohnAddress.office().zip);
  EXPECT_EQ(john_office.city,checkForJohnAddress.office().city);
  
  Lorm::disconnect();
  
}

TEST(To_one, attempt_retrieve_null_relationships) {
  Lorm::connect("sqlite://:memory:");
  Person::create();
  
  Address::create();  
  Lorm::disconnect();
}

