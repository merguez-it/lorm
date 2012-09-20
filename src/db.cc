#include "db.h"
#include "util.h"
#include "config.h"

#ifdef HAVE_SQLITE
#include "dbi/sqlite.h"
#endif

#ifdef HAVE_MYSQL
#include "dbi/mysql.h"
#endif

#ifdef HAVE_POSTGRESQL
#include "dbi/pg.h"
#endif

#include <iostream>

Lorm::Lorm() : is_connected_(false) {}

void Lorm::conn(std::string cs) {
  if(is_connected_) {
    discon();
  }

  std::vector<std::string> conn_info = explode(cs, "://");
  if(conn_info[0] == "sqlite") {
#ifdef HAVE_SQLITE
    dbi_ = new lorm::sqlite(conn_info[1]);
#else
    throw 1; // TODO
#endif
    // TODO add pg & mysql
  }

  connection_string_ = cs;
  is_connected_ = true;
}

void Lorm::discon() {
  dbi_->close();
  delete dbi_;
  connection_string_ = "";
  is_connected_ = false;
}

void Lorm::connect(std::string cs) {
  Lorm::getInstance()->conn(cs);
}

void Lorm::disconnect() {
  Lorm::getInstance()->discon();
}

long Lorm::execute(const std::string & query) {
  return dbi_->execute(query);
}

void Lorm::create_table(const std::string & name, std::vector<lorm::column_t> columns) {
  dbi_->create_table(name, columns);
}

std::vector<std::map<std::string, std::string> > Lorm::select(const std::string & query) {
  return dbi_->select(query);
}
