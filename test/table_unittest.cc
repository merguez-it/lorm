#include "gtest/gtest.h"
#include "lorm.h"


class Person : public table<Person> {
  public:
    TABLE_INIT(Person);
    column<int> id;
    column<int> num;
    column<std::string> name;
    column<std::string> desc;
    column<double> age;
    column<datetime> birthday;
    column<bool> is_cool;
};

REGISTER_TABLE(Person) {
  register_identity("id", &Person::id); // ID
  register_field("num", &Person::num, 123, false); // NOT_NULL
  register_field("name", &Person::name, false);
  register_field("desc", &Person::desc, std::string("guest"));
  register_field("age", &Person::age, 1.2);
  register_field("birthday", &Person::birthday, datetime("1967-06-26 00:00:00"));
  register_field("is_cool", &Person::is_cool,true,true);
}

TEST(Person, tests) {
  Lorm::connect("sqlite://:memory:");
  Person::create();

  // -- 
  Person bob;
  bob.name = "bob";
  bob = bob.save();

  EXPECT_TRUE(bob.name == "bob");
  EXPECT_TRUE(bob.desc == "guest");
  EXPECT_TRUE(bob.num == 123);
  EXPECT_TRUE(bob.age == 1.2);
  EXPECT_TRUE(bob.birthday == datetime("1967-06-26 00:00:00"));
  EXPECT_TRUE(bob.is_cool == true);

  // -- 
  Person john;
  john.name = "john";
  john.num = 666;
  john.age = 18.6;
  datetime dt = datetime::now();
  john.birthday = dt;
  john.is_cool = false;
  john = john.save();

  EXPECT_TRUE(john.name == "john");
  EXPECT_TRUE(john.desc == "guest");
  EXPECT_TRUE(john.num == 666);
  EXPECT_TRUE(john.age == 18.6);
  EXPECT_TRUE(john.birthday == dt);
  EXPECT_TRUE(john.is_cool == false);

  // -- 
  Person clark;
  clark.name = "clark";
  clark.desc = "admin";
  clark.birthday = datetime("1955-04-22");
  clark = clark.save();

  EXPECT_TRUE(clark.name == "clark");
  EXPECT_TRUE(clark.desc == "admin");
  EXPECT_TRUE(clark.num == 123);
  EXPECT_TRUE(clark.age == 1.2);
  EXPECT_TRUE(clark.birthday == datetime("1955-04-22"));

  // -- 
  Person bob_2 = Person::search_by_id(bob.id);

  EXPECT_TRUE(bob_2.name == bob.name);
  EXPECT_TRUE(bob_2.id == bob.id);

  // -- Finding using bools.
  Person cool_people, not_so_cool;
  cool_people.is_cool=true;
  not_so_cool.is_cool = false;
  EXPECT_TRUE(not_so_cool.find()->size()==1);
  EXPECT_TRUE(cool_people.find()->size()==2);

  // --
  Person jupd;
  jupd.num = 456;
  jupd.name = "johnup";
  jupd.is_cool = true;
  john = john.update(jupd);
  EXPECT_TRUE(john.name == "johnup");
  EXPECT_TRUE(john.desc == "guest");
  EXPECT_TRUE(john.num == 456);
  EXPECT_TRUE(john.age == 18.6);
  EXPECT_TRUE(john.birthday == dt);
  EXPECT_TRUE(john.is_cool == true);
  
  // --
  Person someone;
  someone.name = "clark";
  someone.id < 10;
  someone.id > 0;
  collection<Person> *people = someone.find();
  EXPECT_EQ(1U, people->count());
  people->remove();
  EXPECT_EQ(0U, people->count());
	delete people; // hum, sounds a bit like a slaughter...
  Person all;
  EXPECT_EQ(2, all.count());
  all.remove();
  EXPECT_EQ(0, all.count());
  
  Lorm::disconnect();

}

TEST(Person, should_fail_if_bad_connection_string) {
    EXPECT_ANY_THROW(Lorm::connect("sqlite:///path/to/nowhere"));
    
    EXPECT_ANY_THROW(Lorm::connect("mysql:///path/to/nowhere"));

    EXPECT_ANY_THROW(Lorm::connect("dummy:///path/to/nowhere"));
}

TEST(Person, should_throw_if_no_table) {
  Lorm::connect("sqlite://:memory:");
  Person bob;
  bob.name = "bob";
  EXPECT_ANY_THROW(bob.save());
  EXPECT_ANY_THROW(bob.find());
  Lorm::disconnect();
}

TEST(Person, search_by_id_should_throw_if_not_found) {
  Lorm::connect("sqlite://:memory:");
  Person::create();
  Person nobody;
  EXPECT_ANY_THROW( nobody = Person::search_by_id(1234) );
  Lorm::disconnect();
}



