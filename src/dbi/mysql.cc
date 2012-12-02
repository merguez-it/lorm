#include "dbi/mysql.h"
#include "uri.h"
#include <vector>
#include <sstream>
#include <iostream>

namespace lorm {
  // mysql://host:port/database?username=<user>&password=<password>:q
  mysql::mysql(std::string cs) : is_open_(false) {
    URI conn = URI::parse(cs);

    std::cout << "MYSQL : " << conn.host() << " : " << conn.port() << " DB: " << std::string(conn.path().begin()+1, conn.path().end()) << std::endl;
    std::cout << "user = " << conn.query()["username"] << std::endl;
    std::cout << "pass = " << conn.query()["password"] << std::endl;

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
    std::cout << query << std::endl;
    if(0 != mysql_query(db_, query.c_str())) {
      std::cout << mysql_error(db_) << std::endl;
      throw mysql_error(db_);
    }
    return mysql_insert_id(db_);
  }

  void mysql::create_table(const std::string & name, std::vector<lorm::column_t> columns) {
    std::stringstream query;
    query << "CREATE TABLE IF NOT EXISTS " << name << " (";

    std::string sep = "";
    std::vector<lorm::column_t>::iterator it;
    for(it = columns.begin(); it != columns.end(); it++) {
      query << sep << "`" << (*it).name << "` " << types_[(*it).type];
      if((*it).is_id) {
        query << " AUTO_INCREMENT PRIMARY KEY";
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

  std::vector<std::map<std::string, std::string> > mysql::select(const std::string & query) {
    std::vector<std::map<std::string, std::string> > data;

    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *field;

    std::vector<std::string> header_;
    int num_fields;
    int i;

    execute(query);
    result = mysql_store_result(db_);
    num_fields = mysql_num_fields(result);

    while(field = mysql_fetch_field(result)) {
      header_.push_back(field->name);
    }

    while ((row = mysql_fetch_row(result))) {
      std::map<std::string, std::string> row_;
      for(i = 0; i < num_fields; i++) {
        if(row[i]) {
          row_[header_[i]] = row[i];
        }
      }
      data.push_back(row_);
    }
    mysql_free_result(result);

    return data;
  }
}
