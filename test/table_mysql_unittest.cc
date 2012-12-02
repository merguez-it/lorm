#include "gtest/gtest.h"
#include "lorm.h"


class Person : public table<Person> {
  public:
    TABLE_INIT(Person);

    column<int> id;
    column<int> num;
    column<std::string> name;
    column<std::string> description;
    column<double> age;
    column<datetime> birthday;
};

REGISTER_TABLE(Person) {
  identity("id", &Person::id); // ID
  field("num", &Person::num, 123, false); // NOT_NULL
  field("name", &Person::name, false);
  field("description", &Person::description, std::string("guest"));
  field("age", &Person::age, 1.2);
  field("birthday", &Person::birthday, datetime("1967-06-26 00:00:00"));
}

TEST(MySQL, tests) {
  Lorm::connect("mysql://localhost/tests?username=root&password=");
  Person::create();

  // -- 
  Person bob;
  bob.name = "bob";
  bob = bob.save();

  ASSERT_TRUE(bob.name == "bob");
  ASSERT_TRUE(bob.description == "guest");
  ASSERT_TRUE(bob.num == 123);
  ASSERT_TRUE(bob.age == 1.2);
  ASSERT_TRUE(bob.birthday == datetime("1967-06-26 00:00:00"));

  // -- 
  Person john;
  john.name = "john";
  john.num = 666;
  john.age = 18.6;
  datetime dt = datetime::now();
  john.birthday = dt;
  john = john.save();

  ASSERT_TRUE(john.name == "john");
  ASSERT_TRUE(john.description == "guest");
  ASSERT_TRUE(john.num == 666);
  ASSERT_TRUE(john.age == 18.6);
  ASSERT_TRUE(john.birthday == dt);

  // -- 
  Person clark;
  clark.name = "clark";
  clark.description = "admin";
  clark.birthday = datetime("1955-04-22");
  clark = clark.save();

  ASSERT_TRUE(clark.name == "clark");
  ASSERT_TRUE(clark.description == "admin");
  ASSERT_TRUE(clark.num == 123);
  ASSERT_TRUE(clark.age == 1.2);
  ASSERT_TRUE(clark.birthday == datetime("1955-04-22"));

  // -- 
  Person bob_2 = Person::search_by_id(bob.id);

  ASSERT_TRUE(bob_2.name == bob.name);
  ASSERT_TRUE(bob_2.id == bob.id);

  // --
  Person jupd;
  jupd.num = 456;
  jupd.name = "johnup";
  john = john.update(jupd);
  ASSERT_TRUE(john.name == "johnup");
  ASSERT_TRUE(john.description == "guest");
  ASSERT_TRUE(john.num == 456);
  ASSERT_TRUE(john.age == 18.6);
  ASSERT_TRUE(john.birthday == dt);

  // --
  Person someone;
  someone.name = "clark";
  someone.id < 10;
  someone.id > 0;
  collection<Person> people = someone.find();
  ASSERT_EQ(1U, people.count());
  people.remove();
  ASSERT_EQ(0U, people.count());

  Person all;
  ASSERT_EQ(2, all.count());
  all.remove();
  ASSERT_EQ(0, all.count());
  
  Lorm::disconnect();

}
