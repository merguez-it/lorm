//
//  sqlite_statement.h
//  LORM
//
//  Created by Mathias Franck on 01/01/13.
//  Implementation for caching, finding, and binding SQLite prepared statements.
//  => keeping a compiled version of commonly used parameterized statements improves
//     significantly performances of "looped" selects.
//  WARNING: In the current implementation , only statments that contains 1 question mark are kept cached.
//  eg: searchById => "select * from people where person_id = ? "
//  eg: join many to many => "select xx_id from xx_yy  where yy_id = ?"
//  eg: join one to many => "select * from yy  where yy.xx_id = ?"


#ifndef __LORM__sqlite_statement__
#define __LORM__sqlite_statement__

#include <iostream>
#include <map>
#include <sqlite3.h>

#include "dbi/dbi.h"

class sqlite_statement /* TODO: public: sql_statement */ {
public:
  // Creates a prepared statement, bound to its "bind_id" argument if needed.
  // In this latter case, statement may also be cached for subsequent uses.
  sqlite_statement(sqlite3 *db, const std::string &query, int bind_id = NO_BIND);

  // Free resources as required.
  ~sqlite_statement();
  
  //Finalizes and frees any stored prepared statements.
  static void reset_cache();
  
  sqlite3_stmt *statement() {return stmt_;}
  
protected:
  sqlite3_stmt *stmt_ ;
  std::string query_;
  static std::map<std::string, sqlite3_stmt * > prepared_stmts_;
  
};
#endif /* defined(__LORM__sqlite_statement__) */
