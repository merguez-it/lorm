#ifndef __DBI_MYSQL_H
#define __DBI_MYSQL_H

#include <mysql/mysql.h>
#include "dbi/dbi.h"

namespace lorm {
  class mysql : public lorm::dbi {
  };
}

#endif // __DBI_MYSQL_H

