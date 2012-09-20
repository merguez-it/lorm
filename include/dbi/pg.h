#ifndef __DBI_PG_H
#define __DBI_PG_H

#include <postgres.h>
#include "dbi/dbi.h"

namespace lorm {
  class pg : public lorm::dbi {
  };
}

#endif // __DBI_PG_H

