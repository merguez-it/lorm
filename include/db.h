#ifndef __LORM_DB_H
#define __LORM_DB_H 

#include "singleton.h"
#include "dbi/dbi.h"
#include <string>

using namespace lorm;


class Lorm : public Singleton<Lorm> { // Not sure singleton is a good idea => what if one has 2 DBs, or 2 DB interfaces ?
																			// - "We want a factory", answer the Knights who say "Nee !" 
  friend class Singleton<Lorm>;
  private:
    Lorm();
    void conn(std::string cs);
    void discon();

  public:
    static void connect(std::string cs);
    static void disconnect();

    long execute(const std::string & query);
    void create_table(const std::string & name, columns_desc columns);
	  // select interface
	  row_iterator select_start(const std::string & query, int bind = NO_BIND) { return dbi_->select_start(query,bind); }
		bool select_next(row_iterator& row) { return dbi_->select_next(row); } 
		void select_end(row_iterator row) {  dbi_->select_end(row); } 
		int col_count(row_iterator row) { return dbi_->col_count(row); }
		bool col_is_null(row_iterator row, int iCol) { return dbi_->col_is_null(row,iCol); }
		const char *col_name (row_iterator row, int iCol) { return dbi_->col_name(row,iCol); }
	
		int get_int_col(row_iterator row, int iCol) { return dbi_->get_int_col(row, iCol); }
		double get_double_col(row_iterator row, int iCol) { return dbi_->get_double_col(row, iCol); }
		datetime get_datetime_col(row_iterator row, int iCol) { return dbi_->get_datetime_col(row, iCol); }
		std::string get_string_col(row_iterator row, int iCol) { return dbi_->get_string_col(row, iCol); }
		void execute_with_callback(const std::string &query, void *result_collection, sqlite_callback func) {dbi_->execute_with_callback(query,result_collection,func); };
    virtual std::string quoted_identifier(const std::string& ident) {return dbi_->quoted_identifier(ident);} //default: identifiers dont need quoting

  
  private:
    std::string connection_string_;
    bool is_connected_;
    lorm::dbi *dbi_;
};

#endif // __LORM_DB_H 

