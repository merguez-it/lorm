#ifndef __DBI_SQLITE_H
#define __DBI_SQLITE_H

#include <sqlite3.h>
#include "dbi/dbi.h"
#include <tr1/memory>


namespace lorm {
  class sqlite : public lorm::dbi {
		
    public:
      sqlite(std::string cs);
      void close();
      long execute(const std::string &query);
      void create_table(const std::string & name, columns_desc columns);
		
			row_iterator select_start(const std::string & query, int bind = NO_BIND);
			bool select_next(row_iterator& row);
		  void select_end(row_iterator row);

			int col_count(row_iterator row);
			bool col_is_null(row_iterator row, int iCol);
			const char *col_name(row_iterator row, int iCol);

			int get_int_col(row_iterator row, int iCol);
			double get_double_col(row_iterator row, int iCol);
			datetime get_datetime_col(row_iterator row, int iCol);
			std::string get_string_col(row_iterator row, int iCol);
		
			void execute_with_callback(const std::string &query, void *result_collection, sqlite_callback func);

    private:
      sqlite3 *db_;
      bool is_open_;
      std::map<dbi_column_type, std::string> types_;
  };
}

#endif // __DBI_SQLITE_H

