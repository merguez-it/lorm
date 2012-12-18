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
	Lorm::getInstance()->execute("DROP TABLE IF EXISTS people");
	
	Person should_fail;
	EXPECT_ANY_THROW(should_fail.save());

  Person::create();

  // -- 
  Person bob;
  bob.name = "bob";
  bob = bob.save();

  EXPECT_TRUE(bob.name == "bob");
  EXPECT_TRUE(bob.description == "guest");
  EXPECT_TRUE(bob.num == 123);
  EXPECT_TRUE(bob.age == 1.2);
  EXPECT_TRUE(bob.birthday == datetime("1967-06-26 00:00:00"));

  // -- 
  Person john;
  john.name = "john";
  john.num = 666;
  john.age = 18.6;
  datetime dt = datetime::now();
  john.birthday = dt;
  john = john.save();

  EXPECT_TRUE(john.name == "john");
  EXPECT_TRUE(john.description == "guest");
  EXPECT_TRUE(john.num == 666);
  EXPECT_TRUE(john.age == 18.6);
  EXPECT_TRUE(john.birthday == dt);

  // -- 
  Person clark;
  clark.name = "clark";
  clark.description = "admin";
  clark.birthday = datetime("1955-04-22");
  clark = clark.save();

  EXPECT_TRUE(clark.name == "clark");
  EXPECT_TRUE(clark.description == "admin");
  EXPECT_TRUE(clark.num == 123);
  EXPECT_TRUE(clark.age == 1.2);
  EXPECT_TRUE(clark.birthday == datetime("1955-04-22"));

  // -- 
  Person bob_2 = Person::search_by_id(bob.id);

  EXPECT_TRUE(bob_2.name == bob.name);
  EXPECT_TRUE(bob_2.id == bob.id);

  // --
  Person jupd;
  jupd.num = 456;
  jupd.name = "johnup";
  john = john.update(jupd);
  EXPECT_TRUE(john.name == "johnup");
  EXPECT_TRUE(john.description == "guest");
  EXPECT_TRUE(john.num == 456);
  EXPECT_TRUE(john.age == 18.6);
  EXPECT_TRUE(john.birthday == dt);

  // --
  Person someone;
  someone.name = "clark";
  someone.id < 10;
  someone.id > 0;
  collection<Person> *people = someone.find();
  EXPECT_EQ(1U, people->count());
  people->remove();
  EXPECT_EQ(0U, people->count());

  Person all;
  EXPECT_EQ(2, all.count());
  all.remove();
  EXPECT_EQ(0, all.count());
  
  Lorm::disconnect();

}
