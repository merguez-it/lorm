#include "dbi/mysql.h"
#include "uri.h"
#include "types.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <assert.h>

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

    types_[SQL_STRING] = "TEXT";
    types_[SQL_INTEGER] = "INTEGER";
    types_[SQL_NUMERIC] = "REAL";
    types_[SQL_DATETIME] = "DATETIME";
    types_[SQL_BOOLEAN] = "BOOL";

  }

  void mysql::close() {
    if(is_open_) {
      mysql_close(db_);
      is_open_ = false;
    }
  }

  long mysql::execute(const std::string &query) {
    if(0 != mysql_query(db_, query.c_str())) {
      // throw mysql_error(db_);
      const char  * msg = mysql_error(db_);
      throw msg;
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
      } else if(it->second.has_default && (it->second.type !=  SQL_STRING)) {
        query << " DEFAULT ";
        switch(it->second.type) {
          case SQL_DATETIME:
            query << "'" << any_cast<datetime>(it->second.default_value) << "'";
            break;
          case SQL_INTEGER:
            query << any_cast<int>(it->second.default_value);
            break;
          case SQL_BOOLEAN:
            query << any_cast<bool>(it->second.default_value);
            break;
          case SQL_NUMERIC:
            query << any_cast<double>(it->second.default_value);
            break;
          default: 
            throw 1; //TODO
        }
      } else if(!it->second.nullable) {
        query << " NOT NULL";
      }
      sep = ", ";
    }

    query << ");";
    execute(query.str());
  }
	
	// "Select" abstract interface for mySQL
	
	struct mysql_raw_iterator {
		mysql_raw_iterator() : mysql_result(NULL), mysql_row(NULL) {};
		bool initialized() {return mysql_result!=NULL;}
		bool fetchable() {return initialized() && mysql_row !=NULL;}
		MYSQL_RES *mysql_result;
	  MYSQL_ROW mysql_row;
	};
	
	row_iterator mysql::select_start(const std::string & query,  int bind) {
    std::string tmpq = query;
    if (bind != NO_BIND) { // TODO: Implement binding for mySQL parameters
      size_t pos=0;
      pos=tmpq.find("?");
      if (pos!=std::string::npos) {
          tmpq.replace(pos,1,util::to_string<int>(bind));
      }
    }
		if(0 != mysql_query(db_, tmpq.c_str())) {
      throw mysql_error(db_);
    }
		MYSQL_RES *mysql_result =  mysql_use_result(db_);
		mysql_raw_iterator *dbi_result = new mysql_raw_iterator();
		dbi_result->mysql_result = mysql_result ;
		return (row_iterator)(dbi_result);
	}
	
	bool mysql::select_next(row_iterator& row) {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->initialized());
		MYSQL_ROW mysql_row = mysql_fetch_row(mysql_it->mysql_result);
		if (mysql_row) {
			mysql_it->mysql_row = mysql_row;
		} else if (!std::string(mysql_error(db_)).empty()) {
			throw mysql_error(db_); // TODO: Exception + catchable
		}
		return mysql_row != NULL;
	}
	
	void mysql::select_end(row_iterator row)  {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->initialized());
		mysql_free_result(mysql_it->mysql_result);
	};
	
	bool mysql::col_is_null(row_iterator row, int iCol) {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->fetchable());
		return mysql_it->mysql_row[iCol] == NULL;
	};
	
	const char *mysql::col_name(row_iterator row, int iCol) {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->initialized());
		const char *fieldname =mysql_fetch_field_direct(mysql_it->mysql_result,iCol)->name;
		assert(fieldname);
		return fieldname;
	}

	int mysql::col_count(row_iterator row) {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->fetchable());
		return mysql_num_fields(mysql_it->mysql_result) ;
	}
	
	int mysql::get_int_col(row_iterator row, int iCol) {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->fetchable() && mysql_it->mysql_row[iCol]!=NULL);
		return util::from_string<int>(mysql_it->mysql_row[iCol]);
	};
	
	std::string mysql::get_string_col(row_iterator row, int iCol) {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->fetchable() && mysql_it->mysql_row[iCol]!=NULL);
		return mysql_it->mysql_row[iCol];
	};
	
	double mysql::get_double_col(row_iterator row, int iCol) {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->fetchable() && mysql_it->mysql_row[iCol]!=NULL);
		return util::from_string<double>(mysql_it->mysql_row[iCol]);
	};
	
	datetime mysql::get_datetime_col(row_iterator row, int iCol) {
		mysql_raw_iterator *mysql_it =  (mysql_raw_iterator *)row;
		assert(mysql_it && mysql_it->fetchable() && mysql_it->mysql_row[iCol]!=NULL);
		return datetime(mysql_it->mysql_row[iCol]);
	};
}
