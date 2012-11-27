#include "dbi/sqlite.h"
#include <vector>
#include <sstream>
#include <iostream>

namespace lorm {
  sqlite::sqlite(std::string cs) : is_open_(false) {
    int rc = sqlite3_open(cs.c_str(), &db_);
    is_open_ = rc == SQLITE_OK;
    if(!is_open_) {
      throw 1; // TODO
    }

    types_[SQL_STRING] = "TEXT";
    types_[SQL_INTEGER] = "INTEGER";
    types_[SQL_NUMERIC] = "REAL";
    types_[SQL_DATETIME] = "DATE";

  }

  void sqlite::close() {
    if(is_open_) {
      is_open_ = !(SQLITE_OK == sqlite3_close(db_));
    }

    if(is_open_) {
      throw 1; // TODO
    }
  }

  long sqlite::execute(const std::string &query) {
    char *err; 
    if(SQLITE_OK != sqlite3_exec(db_, query.c_str(), NULL, 0, &err)) {
      throw err; // TODO
    }
    return sqlite3_last_insert_rowid(db_);
  }

  void sqlite::create_table(const std::string & name, std::vector<lorm::column_t> columns) {
    std::stringstream query;
    query << "CREATE TABLE IF NOT EXISTS " << name << " (";

    std::string sep = "";
    std::vector<lorm::column_t>::iterator it;
    for(it = columns.begin(); it != columns.end(); it++) {
      query << sep << (*it).name << " " << types_[(*it).type];
      if((*it).is_id) {
        query << " PRIMARY KEY AUTOINCREMENT";
      } else if((*it).has_default) {
        query << " DEFAULT ";
        switch((*it).type) {
          case SQL_STRING:
            query << "'" << any_cast<std::string>((*it).default_value) << "'";
            break;
          case SQL_DATETIME:
            query << "'" << any_cast<datetime>((*it).default_value) << "'";
            break;
          case SQL_INTEGER:
            query << any_cast<int>((*it).default_value);
            break;
          case SQL_NUMERIC:
            query << any_cast<double>((*it).default_value);
            break;
        }
      } else if(!(*it).nullable) {
        query << " NOT NULL";
      }
      sep = ", ";
    }

    query << ");";
    execute(query.str());
  }

  std::vector<std::map<std::string, std::string> > sqlite::select(const std::string & query) {
    std::vector<std::map<std::string, std::string> > data;
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, 0);
    if(rc != SQLITE_OK || !stmt) {
      throw 1; // TODO
    }

    int cols = sqlite3_column_count(stmt);

    while(1) {
      rc = sqlite3_step(stmt);
      if(SQLITE_DONE == rc) {
        break;
      } else if(SQLITE_ROW == rc) {
        std::map<std::string, std::string> row;

        for(int i = 0; i < cols; i++) {
          const char *col = sqlite3_column_name(stmt, i);
          const char *txt = (const char*)sqlite3_column_text(stmt, i);

          if(txt) {
            row[col] = txt;
          }
        }

        data.push_back(row);
      } else {
        throw 1; // TODO
      }
    }
    sqlite3_finalize(stmt);

    return data;
  }
}
