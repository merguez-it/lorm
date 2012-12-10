/*
 *  many_to_many_unitest.cc
 *  LORM
 *
 *  Created by Mathias Franck on 01/12/12.
 *  Copyleft 2012 merguez-IT. All rights reserved.
 *
 */
#include "gtest/gtest.h"
#include "lorm.h"

// Persistent objects interface

class Person;

class Book : public table<Book> {
public:
  TABLE_INIT(Book);
  column<int> id;
  column<std::string> title;
  reference<Person> owner;
};

class Person : public table<Person> {
public:
  TABLE_INIT(Person);
  column<int> id;
  column<std::string> name;
  COLLECTION(Book, personal_library);
};

// Persistent objects implementation

REGISTER_TABLE(Book) {
  identity("book_id", &Book::id); // ID
  field("title", &Book::title);
  has_one("owner_id",&Book::owner);
}

REGISTER_TABLE(Person) {
  identity("person_id", &Person::id); // ID
  field("name", &Person::name, false);
}
has_many(Person,Book,personal_library,owner);

// Test stuff 
#define BUILD_BOOK(book)\
  Book book;\
  book.title=#book;\
  book=book.save();\

#define BUILD_PERSON(person)\
  Person person;\
  person.name=#person;\
  person=person.save();\

TEST(ONE_TO_MANY, retrieve_unidirectional_manies) {
  // Ready...
  Lorm::connect("sqlite://:memory:");
  Person::create();
  Book::create();
  BUILD_BOOK(effectiveCipleussepleusse)
  BUILD_BOOK(designPatterns)
  BUILD_BOOK(principlesOfCompilerDesign)
  BUILD_BOOK(HarryPotter_IV)
  BUILD_BOOK(HarryPotter_V)
  BUILD_BOOK(HarryPotter_VI)

  BUILD_PERSON(Greg)
  BUILD_PERSON(JeanPaul)
  BUILD_PERSON(Mathias)
  
  //Set...
  effectiveCipleussepleusse.owner(Greg);
  effectiveCipleussepleusse.update();
  
  principlesOfCompilerDesign.owner(JeanPaul);
  principlesOfCompilerDesign.update();
  
  designPatterns.owner(JeanPaul);
  designPatterns.update();
  
  HarryPotter_IV.owner(Mathias);
  HarryPotter_IV.update();
  
  HarryPotter_V.owner(Mathias);
  HarryPotter_V.update();
  
  HarryPotter_VI.owner(Mathias);
  HarryPotter_VI.update();
  
  // Go !
  EXPECT_EQ(3U,Mathias.personal_library().size());
  EXPECT_EQ(2U,JeanPaul.personal_library().size());
  EXPECT_EQ(1U,Greg.personal_library().size());
	EXPECT_EQ(Mathias.personal_library()[2].title,"HarryPotter_VI");

  Lorm::disconnect();
  
}



