#include "dbi/sqlite.h"
#include "table.h"
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

  void sqlite::create_table(const std::string & name, columns_desc columns) {
    std::stringstream query;
    query << "CREATE TABLE IF NOT EXISTS " << name << " (";

    std::string sep = "";
    columns_desc::iterator it;
    for(it = columns.begin(); it != columns.end(); it++) {
      query << sep << it->first << " " << types_[it->second.type];
      if(it->second.is_id) {
        query << " PRIMARY KEY AUTOINCREMENT";
      } else if(it->second.has_default) {
        query << " DEFAULT ";
        switch(it->second.type) {
          case SQL_STRING:
            query << "'" << any_cast<std::string>(it->second.default_value) << "'";
            break;
          case SQL_DATETIME:
            query << "'" << any_cast<datetime>(it->second.default_value) << "'";
            break;
          case SQL_INTEGER:
            query << any_cast<int>(it->second.default_value);
            break;
          case SQL_NUMERIC:
            query << any_cast<double>(it->second.default_value);
            break;
        }
      } else if(!it->second.nullable) {
        query << " NOT NULL";
      }
      sep = ", ";
    }

    query << ");";
    execute(query.str());
  }
	
//SELECT interface subset
	row_iterator sqlite::select_start(const std::string & query)	{
		sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, 0);
    if(rc != SQLITE_OK || !stmt) {
			throw 1; //TODO
    }
		return stmt;
	}
						
	bool sqlite::select_next(row_iterator& row) {
		sqlite3_stmt *stmt = (sqlite3_stmt *)row;
		int rc = sqlite3_step(stmt);
		if (SQLITE_DONE == rc) {
			sqlite3_finalize(stmt);
			stmt =NULL;
		} else if (SQLITE_ROW != rc) {
			throw 1; // TODO
		}
		row=stmt;
		return row != NULL;
	};
	
	void sqlite::select_end(row_iterator row)  {
		sqlite3_stmt *stmt = (sqlite3_stmt *)row;
		sqlite3_finalize(stmt); // 	Harmless on a null pointer => safe (even if useless) to be called ,
														//	even after "select_next" has nullified the statement;
	}
	
	int sqlite::col_count(row_iterator row) {
		return sqlite3_column_count((sqlite3_stmt *)row);
	}
	
	bool sqlite::col_is_null(row_iterator row, int iCol) {
		return SQLITE_NULL == sqlite3_column_type((sqlite3_stmt *)row, iCol);
	};

	const char *sqlite::col_name(row_iterator row, int iCol) {
	  return sqlite3_column_name((sqlite3_stmt *)row, iCol);
	}

	int sqlite::get_int_col(row_iterator row, int iCol) {
		return sqlite3_column_int( (sqlite3_stmt *)row , iCol );
	};
	
	std::string sqlite::get_string_col(row_iterator row, int iCol) {
		const unsigned char * s = sqlite3_column_text( (sqlite3_stmt *)row , iCol );
		return std::string( (const char *) s );
	};
	
	double sqlite::get_double_col(row_iterator row, int iCol) {
		return sqlite3_column_double((sqlite3_stmt *)row , iCol );
	};
	
	datetime sqlite::get_datetime_col(row_iterator row, int iCol) {
		const char * s = (const char *)sqlite3_column_text((sqlite3_stmt *)row, iCol);
		return datetime(s);
	};
	
	void sqlite::execute_with_callback(const std::string &query, void *result_collection, sqlite_callback func) {
    char *err; 
    if(SQLITE_OK != sqlite3_exec(db_, query.c_str(), func, result_collection,&err)) {
      throw err; // TODO
    }
	}

}
