#ifndef __LORM_TABLE_H
#define __LORM_TABLE_H

#include "column.h"
#include "collection.h"
#include "db.h"
#include "util.h"
#include "types.h"
#include "datetime.h"
#include <tr1/memory>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>

#include "reference.h"

//#define DEBUG 1
//#define POTATOE_SQLITE 1

#ifdef DEBUG 
#define DEBUG_QUERY(QUERY) std::cout << QUERY.str() << std::endl;
#else 
#define DEBUG_QUERY(QUERY) // Nada !
#endif

#define FIELD_FUN(CPPTYPE, LORMTYPE) \
static void field(const std::string & col, column<CPPTYPE> T::* f, CPPTYPE def, bool nullable = true) { \
lorm::column_t c; \
c.offset=*(reinterpret_cast<unsigned long *> (&f));\
c.is_id = false; \
c.type = LORMTYPE; \
c.name = col; \
c.nullable = nullable; \
c.has_default = true; \
c.default_value = def; \
columns_[col]=c; \
} \
static void field(const std::string & col, column<CPPTYPE> T::* f, bool nullable = true) { \
lorm::column_t c; \
c.offset=*(reinterpret_cast<unsigned long *> (&f));\
c.is_id = false; \
c.type = LORMTYPE; \
c.name = col; \
c.nullable = nullable; \
c.has_default = false; \
columns_[col]=c; \
} 

#define ID_FUN(TYPE) \
static void identity(const std::string & col, column<TYPE> T::* f) { \
identity_col_ = col; \
lorm::column_t c; \
c.offset= *(reinterpret_cast<unsigned long *> (&f));\
c.is_id = true; \
c.type = lorm::SQL_INTEGER; \
c.name = col; \
c.nullable = false; \
c.has_default = false; \
columns_[col]=c; \
} 


template <class T> class table {
public: 
	bool is_loaded() {return is_loaded_;}
	table<T>() : is_loaded_(false) {};
  static std::string identity_col_;
  static columns_desc columns_;
  ID_FUN(int)
  FIELD_FUN(int, lorm::SQL_INTEGER)
  FIELD_FUN(std::string, lorm::SQL_STRING)
  FIELD_FUN(double, lorm::SQL_NUMERIC)
  FIELD_FUN(datetime,lorm::SQL_DATETIME)
  
  template <class FOREIGN_CLASS> 
  static void has_one(const std::string & col, reference<FOREIGN_CLASS> T::*f ) {
    field(col,reinterpret_cast<column<int> T::*>(f));
  }
  
  static void create() {
    T table;
    Lorm::getInstance()->create_table(T::classname(), table.columns_);
  }
  
  static T search_by_id(column<int> id) {
    return search_by_id(id.value());
  }
  
  static T search_by_id(int id) {    
    std::stringstream query;
    query << "SELECT " << T::columns_for_select() << " FROM " << T::classname();
    query << " WHERE " << T::identity_col() << " = ? ;";
    DEBUG_QUERY(query)
		collection<T> *data = select(query.str(),false,id);
    if(data->size() > 1) {
      throw "Unique id is not unique ";
    }
    if(data->size() == 0) {
      throw T::classname() + " with given unique id does not exist";
    }
		T result = data->at(0);
		delete data;
    return result;
  }
	
#ifdef POTATOE_SQLITE
	static int callBack(void *collection_as_void, int count, char **values, char **columns)
	{
		collection<T> &result_list = (*(collection<T> *)collection_as_void);
		T result;
		int iResult=result_list.size();
		result_list.push_back(result);
		for (int iCol=0; iCol < count; iCol++) {
			std::string db_col_name = columns[iCol];
			if ( columns_.count(db_col_name) == 1 && values[iCol] ) { // colonne mappée ET non nulle => on la prends
				lorm::column_t cur_col=columns_.find(db_col_name)->second;
				switch(cur_col.type) {
					case lorm::SQL_INTEGER: {
						column<int> T::* f=offset_to_columnref<int>(cur_col.offset);
						result_list.at(iResult).*f = util::from_string<int>(std::string(values[iCol]));
					}
						break;
					case lorm::SQL_STRING: {
						column<std::string> T::* f=offset_to_columnref<std::string>(cur_col.offset);
						result_list.at(iResult).*f = std::string(values[iCol]);
					}
						break;
					case lorm::SQL_DATETIME: {
						column<datetime> T::* f=offset_to_columnref<datetime>(cur_col.offset);
						result_list.at(iResult).*f = datetime::datetime(std::string(values[iCol]));
					}
						break;
					case lorm::SQL_NUMERIC: {
						column<double> T::* f=offset_to_columnref<double>(cur_col.offset);
						result_list.at(iResult).*f = util::from_string<double>(std::string(values[iCol]));
					}
						break;
					default:
						throw "DB Type not defined"; // TODO
				}
			}
		}	
		return 0;
	}
	
	static collection<T> *select(const std::string& query, bool get_keys_only=false)  {
		collection<T> *result_list = new collection<T>;
		Lorm *db = Lorm::getInstance();
		db->execute_with_callback(query, result_list, table<T>::callBack);
		return result_list;
	}
#else
	static collection<T> *select(const std::string& query, bool get_keys_only=false, int bind_id = NO_BIND)  {
		collection<T> *result_list = new collection<T>;
		Lorm *db = Lorm::getInstance();
		row_iterator row=db->select_start(query,bind_id);
		while (db->select_next(row)) {
			int n_cols=db->col_count(row);
			T result;
			result.is_loaded_=!get_keys_only;
			if (get_keys_only) {
				column<int> T::* f=offset_to_columnref<int>(columns_.find(T::identity_col())->second.offset);
				(result.*f) = db->get_int_col(row, 0);
			}
			else {
				columns_desc::const_iterator itCol=columns_.begin();
				for (int iCol=0; iCol < n_cols; iCol++) {
					if (!db->col_is_null(row, iCol) ) { // colonne mappée ET non nulle => on la prends
						switch(itCol->second.type) {
							case lorm::SQL_INTEGER: {
								column<int> T::* f=offset_to_columnref<int>(itCol->second.offset);
								(result.*f) = db->get_int_col(row, iCol);
							}
								break;
							case lorm::SQL_STRING: {
								column<std::string> T::* f=offset_to_columnref<std::string>(itCol->second.offset);
								(result.*f) = db->get_string_col(row, iCol);
							}
								break;
							case lorm::SQL_DATETIME: {
								column<datetime> T::* f=offset_to_columnref<datetime>(itCol->second.offset);
								(result.*f) = db->get_datetime_col(row, iCol);
							}
								break;
							case lorm::SQL_NUMERIC: {
								column<double> T::* f=offset_to_columnref<double>(itCol->second.offset);
								(result.*f) = db->get_double_col(row, iCol);
							}
								break;
							default:
								throw "DB Type not defined"; // TODO
						}
					}
					itCol++;
				}
			}
			result_list->push_back(result); // Using pointers could avoid useless copies
		}
		return result_list;
	}
#endif
protected:
	
	friend class lorm::dbi; // Database interface has privileged access to table<T>, for purpose of transfer optimization.
	
	bool is_loaded_; 	// true if this object had all its field formerly loaded. 
										// false if no Id is set, or if only the Id had been set (case of collections that lazy-load owned instances)
  
  static inline std::string quot(const std::string& ident) { 
  	return Lorm::getInstance()->quoted_identifier(ident);
  }
  
  template <typename CPPTYPE> 
  static column<CPPTYPE> T::* offset_to_columnref(unsigned long offset) {
    column<CPPTYPE> T::* g;
    memcpy(&g, &offset, sizeof(column<CPPTYPE> T::*));
    return g;
  }
  
  const abstract_column& offset_to_column(unsigned long offset) const {
    const abstract_column *col;
    col=reinterpret_cast<const abstract_column *>(this+offset); // Arrière, Satan !!
    return *col;
  }
  
  std::string column_and_values(std::string next_keyword, const std::string &separator) const {
    std::string result;
    columns_desc::iterator it;
    for(it = columns_.begin(); it != columns_.end(); it++) {
      const abstract_column& col=offset_to_column(it->second.offset);
      std::string condition = col.where(quot(it->first));
      if(!condition.empty()) {
        result += next_keyword + condition;
        next_keyword = separator;
      }
    }
    return result;
  }
  
	static std::string columns_for_select() {
    columns_desc::iterator it;
		std::string result;
    std::string sep="";
    for(it = columns_.begin(); it != columns_.end(); it++) {
        result  += sep + quot(it->first);
        sep = ", ";
    }
		return result;
  }
	
	void column_and_values_for_insert(/* out */ std::string& cols,/* out */ std::string& values) const {
    columns_desc::iterator it;
    std::string sep="";
    for(it = columns_.begin(); it != columns_.end(); it++) {
      const abstract_column& col = offset_to_column(it->second.offset);
      column_t& col_type = it->second;
      if( !col.is_null() ) {
        cols  += sep + quot(it->first);
        values += sep + col.as_sql_litteral();
        sep = ", ";
      } else if (col_type.has_default && col_type.type == SQL_STRING) { // TEXT default values not supported by all DB engines (e.g.: mysql)
        cols  += sep + quot(it->first);
				values += sep + column<std::string>(any_cast<std::string>(col_type.default_value)).as_sql_litteral(); // So, let's "force" the default programmatically 
        sep = ", ";
      } 
    }
  }
  
  std::string column_and_values_for_select() const {
    std::string result;
    std::string sep="";
    columns_desc::iterator it;
    for(it = columns_.begin(); it != columns_.end(); it++) {
      const abstract_column& col = offset_to_column( it->second.offset );
      std::string condition = col.where(quot(it->first));
      if( !condition.empty() ) {
        result += sep + condition;
        sep = " AND ";
      }
    }
    return result;
  }
  
  
  template <class FOREIGN_CLASS> 
  collection<FOREIGN_CLASS> * join_many_using_table( const std::string &linkTable,
                                                   const std::string &linkSourceKey,
                                                   const std::string &linkTargetKey,int id)  const {  
    std::stringstream query;
      query << "SELECT " << linkTable << "." <<   linkTargetKey << " AS " << FOREIGN_CLASS::identity_col() << " FROM " << linkTable << " WHERE " << linkTable << "." << linkSourceKey << " = ?";
//		query << "SELECT "<< quot(FOREIGN_CLASS::classname()) << ".* FROM " << quot(FOREIGN_CLASS::classname()) << 
//             " INNER JOIN " << quot(linkTable) <<
//						 " ON " << quot(FOREIGN_CLASS::classname()) << "." << quot(FOREIGN_CLASS::identity_col_) << " = " << quot(linkTable) << "." << quot(linkTargetKey) <<
//						 " AND " << quot(linkTable) << "." << quot(linkSourceKey) << " = " << id;		
		DEBUG_QUERY(query)
		return FOREIGN_CLASS::select(query.str(),true,id);
  }
  
  T save_() const {
    std::string cols;
    std::string values;
    std::stringstream query;
    column_and_values_for_insert(cols,values);
    query << "INSERT INTO " << quot(T::classname()) << "(" << cols  << ") VALUES (" << values << ");";
    DEBUG_QUERY(query)
    long id = (Lorm::getInstance()->execute(query.str()));
    return T::search_by_id(id);
  }
  
  collection<T> *find_() const {
    std::stringstream query;
      query << "SELECT " << columns_for_select()  << " FROM " << T::classname();
    std::string clause_where=column_and_values_for_select();
    if (!clause_where.empty()) {
      query << " WHERE " << clause_where << ";";
    }
    DEBUG_QUERY(query)
		return select(query.str(),false);
  }
  
  void remove_() const {
    std::stringstream query;
    query << "DELETE FROM " << T::classname();
    std::string clause_where=column_and_values_for_select();
    if (!clause_where.empty()) {
      query << " WHERE " << clause_where << ";";
    }
    DEBUG_QUERY(query)
    Lorm::getInstance()->execute(query.str());
  }
  
  int count_() const {
		int result = 0;
    std::stringstream query;
    query << "SELECT COUNT(*) AS count FROM " << T::classname();
    std::string clause_where=column_and_values_for_select();
    if (!clause_where.empty()) {
      query << " WHERE " << clause_where << ";";
    }
    DEBUG_QUERY(query)
		Lorm *db = Lorm::getInstance();
		row_iterator row = db->select_start(query.str());
		db->select_next(row); // get the "row".
		result =db->get_int_col(row, 0); // get the count in the "row".
		db->select_end(row);
	  return result;
  }
  
  T update_(const T* const t, const T& u) const {
    return update_(*t, u);
  }
    
  T update_(const T& t, const T& u) const { //TODO: virer t pour utiliser plutôt "this", non ?
    std::stringstream query;
    std::string keyword = " SET ";
    query << "UPDATE " << T::classname();
    query << u.column_and_values(keyword,", ");
    keyword = " WHERE ";
    query << t.column_and_values(keyword," AND ");
    DEBUG_QUERY(query)
    Lorm::getInstance()->execute(query.str());
    return T::search_by_id(t.id);
  }
  
  std::string to_string_() const {
    return column_and_values("\t", "\n\t");
  }
};

#define COLLECTION(FOREIGN_CLASS,role)\
private:\
  std::tr1::shared_ptr< collection < FOREIGN_CLASS > > role##_;\
public:\
  collection < FOREIGN_CLASS > &role (bool force_reload=false);

#define has_many(THIS_CLASS,FOREIGN_CLASS,role,reverse_role)\
collection < FOREIGN_CLASS > &THIS_CLASS::role (bool force_reload) {\
  if (this->id.is_null()) throw "Cannot access roles of an unsaved object";\
  if (!role##_.get() || force_reload) {\
    FOREIGN_CLASS reverse_criteria;\
    reverse_criteria.reverse_role=this->id;\
    collection < FOREIGN_CLASS > *pCol=reverse_criteria.find();\
    role##_.reset(pCol);\
  }\
  return *role##_;\
}

#define has_and_belongs_to_many( THIS_CLASS, FOREIGN_CLASS, role, linkTable, linkToSource, linkToTarget)\
collection < FOREIGN_CLASS > &THIS_CLASS::role (bool force_reload) {\
  if (this->id.is_null()) throw "Cannot access roles of an unsaved object";\
  if (!role##_.get() || force_reload) {\
    collection < FOREIGN_CLASS > *pCol=join_many_using_table<FOREIGN_CLASS>(std::string(linkTable), std::string(linkToSource), std::string(linkToTarget),this->id.value());\
    role##_.reset(pCol);\
  }\
  return *role##_;\
}

#define TABLE_INIT(K, ...) \
  K(); \
  static void register_table();\
  static const std::string& identity_col(); \
  K save() const; \
  collection<K> *find() const; \
  int count() const; \
  K update(K u) const; \
  const K& update() const ; \
  void remove() const; \
  std::string to_string() const; \
  static std::string classname() { std::string cname = #__VA_ARGS__; if(cname.empty()) { return pluralize(lower(#K)); } return cname; }


#define REGISTER_TABLE(K) \
  template <class T> std::map<std::string,lorm::column_t> table<K>::columns_;\
  template <class T> std::string table<K>::identity_col_;\
  const std::string& K::identity_col() {if (columns_.empty() ) K::register_table(); return identity_col_;} \
  K::K() {if (columns_.empty() ) K::register_table(); } \
  K K::save() const { return save_(); } \
  collection<K> *K::find() const { return find_(); } \
  int K::count() const { return count_(); } \
  K K::update(K u) const { return update_(this, u); } \
  const K& K::update() const { K t; t.id=id; K::update_(t,*this); return *this;}\
  void K::remove() const { remove_(); } \
  std::string K::to_string() const { return to_string_(); } \
  void K::register_table() 

#undef FIELD_FUN
#undef FIELD_DATA

#endif // __LORM_TABLE_H

