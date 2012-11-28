#include "person.h"
#include "macro_utils.h"

REGISTER_TABLE(Person) {
  identity("id", &Person::id); // ID
  field("num", &Person::num, 123, false); // NOT_NULL
  field("name", &Person::name, false);
  field("desc", &Person::desc, std::string("guest"));
  field("age", &Person::age, 1.2);
  field("birthday", &Person::birthday, datetime("1967-06-26 00:00:00"));
  has_one("address_id", &Person::address);
  has_one("office_id", &Person::bureau);
//  has_and_belongs_to_many("friends", &Person::friends); // TODO NYI
//  has_many("parents", &Person::parents); // TODO NYI
}
 

int main() {
//  Lorm::connect("sqlite://:memory:");
  try {
    Lorm::connect("sqlite://person.db"); // Penser à  nettoyer person.db, sinon, les id augmentent et le test change
    Person::create();
    Address::create();
    // -- 
    Person bob;
    bob.name = "bob";
    bob = bob.save();
    std::cout << "INSERT bob ->\n" << bob.to_string() << std::endl;

    // -- 
    Address john_home;
    john_home.street="14 Rue de la Vieille Aventure";
    john_home.zip=59000;
    john_home.city="Lille";
    Address john_home_saved=john_home.save();
    
    Person john;
    john.name = "john";
    john.num=666;
    john.age = 18.6;
    john.birthday = datetime::now();
    john.address(john_home_saved);
    john = john.save();
    std::cout << "INSERT john ->\n" << john.to_string() << std::endl;
    
    Person anotherJohn;
    anotherJohn=Person::search_by_id(john.id);
    std::cout << "John's addresse: " << anotherJohn.address().street << std::endl;
    // -- 
    Person clark;
    clark.name = "clark";
    clark.desc = "admin";
    clark.birthday = datetime("1955-04-22");
    clark.save();
    std::cout << "INSERT clark ->\n" << clark.to_string() << std::endl;
    // -- 
    Person bob_2 = Person::search_by_id(bob.id);
    std::cout << "Person::search_by_id(" << bob.id << ") ->\n" << bob_2.to_string() << std::endl;

    // --
    Person jupd;
    jupd.num = 456;
    jupd.name="johnup";
    john = john.update(jupd);
    std::cout << "Update john -> " << john.to_string() << std::endl;

    // --

    Person someone;
    someone.name = "clark";
    someone.id < 10;
    someone.id > 0;

    collection<Person> people = someone.find();
    std::cout << "Found " << people.size() << " " << someone.name << "(s) : " << std::endl;
    collection<Person>::iterator it;
    for(it = people.begin(); it != people.end(); it++) {
      std::cout << "DELETE " << (*it).name << " #" << (*it).id << std::endl;
    }
    people.remove();

    Person all;
    std::cout << "Delete all: Will remove " << all.count() << " entries"<< std::endl;
    all.remove();
    
    Address godzilla;
    godzilla.remove(); // Godzilla destroys all addresses in the city, mouhahahaha !!!
    
    Lorm::disconnect();
  }
  catch (const char *s) {
    std::cerr << "Exception: " << s << std::endl;
  }
  catch (...) {
    std::cerr << "Exception inconnue " << std::endl;
  }
  return 0;
}
