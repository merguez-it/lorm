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

class Person;

class Book : public table<Book> {
public:
  TABLE_INIT(Book);
  column<int> id;
  column<std::string> title;
  COLLECTION(Person, borrowers);
};

REGISTER_TABLE(Book) {
  identity("book_id", &Book::id); // ID
  field("title", &Book::title);
}
has_and_belongs_to_many(Book,Person,borrowers,"borrows","borrowed_book_id", "borrower_id");


class Person : public table<Person> {
public:
  TABLE_INIT(Person);
  column<int> id;
  column<std::string> name;
  COLLECTION(Book, borrowed_books);
};

REGISTER_TABLE(Person) {
  identity("person_id", &Person::id); // ID
  field("name", &Person::name, false);
}
has_and_belongs_to_many(Person,Book,borrowed_books,"borrows","borrower_id","borrowed_book_id");


#define BUILD_BOOK(book)\
  Book book;\
  book.title=#book;\
  book=book.save();\

#define BUILD_PERSON(person)\
  Person person;\
  person.name=#person;\
  person=person.save();\

#define BORROWS(book,person) Lorm::getInstance()->execute("INSERT INTO borrows (borrowed_book_id, borrower_id) VALUES ("\
                + book.id.as_sql_litteral() + "," + person.id.as_sql_litteral() + ")");\

TEST(Many_to_many, retrieve_bidirectional_manies) {
  
  // Preparing
  Lorm::connect("sqlite://:memory:");
  Person::create();
  Book::create();
  Lorm::getInstance()->
  execute("CREATE TABLE borrows (\
                   borrowed_book_id integer NOT NULL,\
                   borrower_id integer NOT NULL\
                )");
  
  BUILD_BOOK(effectiveCipleussepleusse)
  BUILD_BOOK(designPatterns)
  BUILD_BOOK(principlesOfCompilerDesign)

  BUILD_PERSON(Greg)
  BUILD_PERSON(JeanPaul)
  BUILD_PERSON(Mathias)
  
  // borrowing - TODO: write accessors for many-to-many - as thou shall not write macros as they're evil
  BORROWS(principlesOfCompilerDesign,Greg)
  BORROWS(principlesOfCompilerDesign,JeanPaul)
  BORROWS(principlesOfCompilerDesign,Mathias)
  BORROWS(effectiveCipleussepleusse,Greg)
  BORROWS(effectiveCipleussepleusse,JeanPaul)
  BORROWS(designPatterns,JeanPaul)
    
  // books to people
  EXPECT_EQ(1U,designPatterns.borrowers().size());
  EXPECT_EQ(2U,effectiveCipleussepleusse.borrowers().size());
  EXPECT_EQ(3U,principlesOfCompilerDesign.borrowers().size());
  
  // people to books
  EXPECT_EQ(1U,Mathias.borrowed_books().size());
  EXPECT_EQ(2U,Greg.borrowed_books().size());
	collection< Book > emprunts_de_JeanPaul;
	emprunts_de_JeanPaul=JeanPaul.borrowed_books();
  EXPECT_EQ(3U,emprunts_de_JeanPaul.size());
//	EXPECT_EQ((void *)NULL, emprunts_de_JeanPaul.at(1).title.value);
	EXPECT_EQ(emprunts_de_JeanPaul[1].title,"effectiveCipleussepleusse");

  Lorm::disconnect();
  
}