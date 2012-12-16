#ifndef __LORM_DBI_H
#define __LORM_DBI_H

#include "any.h"
#include "datetime.h"

#include <string>
#include <map>
#include <vector>

#include "collection.h"

namespace lorm {

  enum dbi_column_type {
    SQL_STRING,
    SQL_INTEGER,
    SQL_NUMERIC,
    SQL_DATETIME
  };

  struct column_t {
		std::string name;
    bool is_id;
    lorm::dbi_column_type type;
    bool nullable;
    bool has_default;
    any default_value;
    unsigned long offset; // Binary offset to the member in the class
  };

	typedef void* row_iterator; // Opaque row cursor on db
	typedef std::map<std::string, lorm::column_t> columns_desc;

	class dbi {
    public:
      virtual void close() = 0;
      virtual long execute(const std::string &query) = 0;
      virtual void create_table(const std::string & name, columns_desc columns) = 0;
		
      virtual row_iterator select_start(const std::string & query) = 0; // immediately returns null if no results.
			virtual bool select_next(row_iterator &row /* in & out */) = 0; // Also does the clean-up when it returns false
			virtual int col_count(row_iterator row) = 0;
		  virtual bool col_is_null(row_iterator row, int iCol) = 0;
			virtual const char *col_name(row_iterator row, int iCol) = 0;		
		
			virtual int get_int_col(row_iterator row, int iCol) = 0;
			virtual double get_double_col(row_iterator row, int iCol) = 0;
			virtual datetime get_datetime_col(row_iterator row, int iCol) = 0;
			virtual std::string get_string_col(row_iterator row, int iCol) = 0;
  };
}

#endif // __LORM_DBI_H

