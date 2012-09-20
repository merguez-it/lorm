#ifndef __LORM_DB_H
#define __LORM_DB_H 

#include "singleton.h"
#include "dbi/dbi.h"
#include <string>

class Lorm : public Singleton<Lorm> {
  friend class Singleton<Lorm>;

  private:
    Lorm();
    void conn(std::string cs);
    void discon();

  public:
    static void connect(std::string cs);
    static void disconnect();

    long execute(const std::string & query);
    void create_table(const std::string & name, std::vector<lorm::column_t> columns);
    std::vector<std::map<std::string, std::string> > select(const std::string & query);

  private:
    std::string connection_string_;
    bool is_connected_;
    lorm::dbi *dbi_;
};

#endif // __LORM_DB_H 

