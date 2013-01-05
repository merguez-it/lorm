//
//  sqlite_statement.cc
//  LORM
//
//  Created by Mathias Franck on 01/01/13.
//
//

#include "dbi/sqlite_statement.h"
#include "dbi/sqlite.h"

std::map<std::string, sqlite3_stmt * > sqlite_statement::prepared_stmts_;

sqlite_statement::sqlite_statement(sqlite3 *db, const std::string &query, int bind_id) : stmt_(NULL) {
  int rc = SQLITE_OK;
  std::map<std::string, sqlite3_stmt * >::iterator stmt_it = prepared_stmts_.find(query);
  if (stmt_it == prepared_stmts_.end()) {
    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt_, 0);
    if (bind_id != NO_BIND) {
      prepared_stmts_[query]=stmt_;
    }
  } else {
    stmt_=stmt_it->second;
  }
  if(rc != SQLITE_OK || !stmt_) {
    throw 1; //TODO
  }
  if (bind_id != NO_BIND) {
    sqlite3_reset(stmt_);
    sqlite3_bind_int(stmt_,1,bind_id);
  }
}

sqlite_statement::~sqlite_statement() {
  std::map<std::string, sqlite3_stmt * >::iterator stmt_it = prepared_stmts_.find(sqlite3_sql(stmt_));
  if (stmt_it == prepared_stmts_.end()) { // free if not cached
    sqlite3_finalize(stmt_);
  }
}

void sqlite_statement::reset_cache() {
  std::map<std::string, sqlite3_stmt * >::iterator stmt_it;
  for (stmt_it=prepared_stmts_.begin();stmt_it!=prepared_stmts_.end();stmt_it++) {
    sqlite3_finalize(stmt_it->second);
  }
  prepared_stmts_.clear();
}
