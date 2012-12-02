#ifndef __DBI_MYSQL_H
#define __DBI_MYSQL_H

extern "C" {
#include <mysql.h>
}
#include "dbi/dbi.h"

namespace lorm {
  class mysql : public lorm::dbi {
    public:
      mysql(std::string cs);
      void close();
      long execute(const std::string &query);
      void create_table(const std::string & name, std::vector<lorm::column_t> columns);
      std::vector<std::map<std::string, std::string> > select(const std::string & query);

    private:
      MYSQL *db_;
      bool is_open_;
      std::map<dbi_column_type, std::string> types_;
  };
}

#endif // __DBI_MYSQL_H

