#ifndef __DBI_SQLITE_H
#define __DBI_SQLITE_H

#include <sqlite3.h>
#include "dbi/dbi.h"

namespace lorm {
  class sqlite : public lorm::dbi {
    public:
      sqlite(std::string cs);
      void close();
      long execute(const std::string &query);
      void create_table(const std::string & name, std::vector<lorm::column_t> columns);
      std::vector<std::map<std::string, std::string> > select(const std::string & query);

    private:
      sqlite3 *db_;
      bool is_open_;
      std::map<dbi_column_type, std::string> types_;
  };
}

#endif // __DBI_SQLITE_H

