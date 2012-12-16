#include "dbi/mysql.h"
#include "uri.h"
#include <vector>
#include <sstream>
#include <iostream>

namespace lorm {
  // mysql://host:port/database?username=<user>&password=<password>:q
  mysql::mysql(std::string cs) : is_open_(false) {
    URI conn = URI::parse(cs);

    db_ = mysql_init(NULL);
    is_open_ = (NULL != mysql_real_connect(
          db_,
          conn.host().c_str(),
          conn.query()["username"].c_str(),
          conn.query()["password"].c_str(),
          std::string(conn.path().begin()+1, conn.path().end()).c_str(),
          conn.port() ? conn.port() : 3306,
          NULL,
          0
          ));
    if(!is_open_) {
      throw 1; // TODO
    }

    types_[SQL_STRING] = "VARCHAR(2048)";
    types_[SQL_INTEGER] = "INTEGER";
    types_[SQL_NUMERIC] = "REAL";
    types_[SQL_DATETIME] = "DATETIME";
  }

  void mysql::close() {
    if(is_open_) {
      mysql_close(db_);
      is_open_ = false;
    }
  }

  long mysql::execute(const std::string &query) {
    if(0 != mysql_query(db_, query.c_str())) {
      throw mysql_error(db_);
    }
    return mysql_insert_id(db_);
  }

  void mysql::create_table(const std::string & name, columns_desc columns) {
    std::stringstream query;
    query << "CREATE TABLE IF NOT EXISTS " << name << " (";

    std::string sep = "";
    columns_desc::iterator it;
    for(it = columns.begin(); it != columns.end(); it++) {
      query << sep << "`" << it->first << "` " << types_[it->second.type];
      if(it->second.is_id) {
        query << " AUTO_INCREMENT PRIMARY KEY";
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
	
	row_iterator mysql::select_start(const std::string & query) {
		return NULL; //TODO : DO ! 
	}
	
	bool mysql::select_next(row_iterator& row) {
		return NULL; //TODO : DO ! 
	};
	
	bool mysql::col_is_null(row_iterator row, int iCol) {
		return true; //TODO : DO ! 
	};
	
	const char *mysql::col_name(row_iterator row, int iCol) {
		return ""; //TODO : DO !
	}

	int mysql::col_count(row_iterator row) {
		return 0; //TODO : DO !
	}
	
	int mysql::get_int_col(row_iterator row, int iCol) {
		return 0; //TODO : DO !
	};
	
	std::string mysql::get_string_col(row_iterator row, int iCol) {
		return ""; //TODO : DO !
	};
	
	double mysql::get_double_col(row_iterator row, int iCol) {
		return 0.0; //TODO : DO !
	};
	
	datetime mysql::get_datetime_col(row_iterator row, int iCol) {
		return datetime(); //TODO : DO !
	};
	
//  void mysql::select(const std::string & query, collection<T>& data ) {
//    MYSQL_RES *result;
//    MYSQL_ROW row;
//    MYSQL_FIELD *field;
//
//    std::vector<std::string> header_;
//    int num_fields;
//    int i;
//
//    execute(query);
//    result = mysql_store_result(db_);
//    num_fields = mysql_num_fields(result);
//
//    while(field = mysql_fetch_field(result)) {
//      header_.push_back(field->name);
//    }
//
//    while ((row = mysql_fetch_row(result))) {
//      std::map<std::string, std::string> row_;
//      for(i = 0; i < num_fields; i++) {
//        if(row[i]) {
//          row_[header_[i]] = row[i];
//        }
//      }
//      data.push_back(row_);
//    }
//    mysql_free_result(result);
//  }

}
