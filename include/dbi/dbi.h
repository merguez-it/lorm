#ifndef __LORM_DBI_H
#define __LORM_DBI_H

#include "any.h"
#include "datetime.h"

#include <string>
#include <map>
#include <vector>

namespace lorm {
  enum dbi_column_type {
    SQL_STRING,
    SQL_INTEGER,
    SQL_NUMERIC,
    SQL_DATETIME
  };

  struct column_t {
    bool is_id;
    lorm::dbi_column_type type;
    std::string name;
    bool nullable;
    bool has_default;
    any default_value;
    unsigned long offset; // Binary offset to the member in the class
  };

  class dbi {
    public:
      virtual void close() = 0;
      virtual long execute(const std::string &query) = 0;
      virtual void create_table(const std::string & name, std::vector<column_t> columns) = 0;
      virtual std::vector<std::map<std::string, std::string> > select(const std::string & query) = 0;
  };
}

#endif // __LORM_DBI_H

