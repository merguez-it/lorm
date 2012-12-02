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

#define DEBUG 1

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
columns_.push_back(c); \
} \
static void field(const std::string & col, column<CPPTYPE> T::* f, bool nullable = true) { \
lorm::column_t c; \
c.offset=*(reinterpret_cast<unsigned long *> (&f));\
c.is_id = false; \
c.type = LORMTYPE; \
c.name = col; \
c.nullable = nullable; \
c.has_default = false; \
columns_.push_back(c); \
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
columns_.push_back(c); \
} 


typedef std::vector<lorm::column_t> columns_desc;
typedef std::vector<std::map<std::string, std::string> > result_set;

template <class T> class table {
public: 
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
    return search_by_id(*id.value);
  }
  
  static T search_by_id(int id) {
    T result;
    
    std::stringstream query;
    query << "SELECT * FROM " << T::classname();
    query << " WHERE " << T::identity_col_ << " = " << id << ";"; 
    DEBUG_QUERY(query)
    result_set data = Lorm::getInstance()->select(query.str());
    if(data.size() > 1) {
      throw "Unique id is not unique : "+ id;
    }
    if(data.size() == 1) {
      result = (result.get_selection(data))[0];
    }
    
    return result;
  }
  
  collection<T> get_selection(result_set data) const {
    collection<T> result_list;
    result_set::iterator itd;
    for(itd = data.begin(); itd != data.end(); itd++) {
      T result;
      columns_desc::iterator it;
      
      for(it = columns_.begin(); it != columns_.end(); it++) {
        if((*itd).count((*it).name) > 0) {
          switch((*it).type) {
            case lorm::SQL_INTEGER:
            {
              column<int> T::* f=offset_to_columnref<int>((*it).offset);
              (result.*f) = util::from_string<int>((*itd)[(*it).name]);
            }
              break;
            case lorm::SQL_STRING:
            {
              column<std::string> T::* f=offset_to_columnref<std::string>((*it).offset);
              (result.*f) = (*itd)[(*it).name];
            }
              break;
            case lorm::SQL_DATETIME:
            {
              column<datetime> T::* f=offset_to_columnref<datetime>((*it).offset);
              (result.*f) = datetime::datetime((*itd)[(*it).name]);
            }
              break;
            case lorm::SQL_NUMERIC:
            {
              column<double> T::* f=offset_to_columnref<double>((*it).offset);
              (result.*f) = util::from_string<double>((*itd)[(*it).name]);
            }
              break;
            default:
              throw "DB Type not defined"; // TODO
          }
        }
      }
      result_list.push_back(result);
    }
    return result_list;
  }
 
protected:
  
  template <typename CPPTYPE> 
  column<CPPTYPE> T::* offset_to_columnref(unsigned long offset) const {
    column<CPPTYPE> T::* g;
    memcpy(&g, &offset, sizeof(unsigned long));
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
      const abstract_column& col=offset_to_column((*it).offset);
      std::string condition = col.where((*it).name);
      if(!condition.empty()) {
        result += next_keyword + condition;
        next_keyword = separator;
      }
    }
    return result;
  }
  
  void column_and_values_for_insert(/* out */ std::string& cols,/* out */ std::string& values) const {
    columns_desc::iterator it;
    std::string sep="";
    for(it = columns_.begin(); it != columns_.end(); it++) {
      const abstract_column& col = offset_to_column((*it).offset);
      if( !col.is_null() ) {
        cols  += sep + (*it).name;
        values += sep + col.as_sql_litteral();
        sep = ", ";
      }
    }
  }
  
  std::string column_and_values_for_select() const {
    std::string result;
    std::string sep="";
    columns_desc::iterator it;
    for(it = columns_.begin(); it != columns_.end(); it++) {
      const abstract_column& col = offset_to_column( (*it).offset );
      std::string condition = col.where((*it).name);
      if( !condition.empty() ) {
        result += sep + condition;
        sep = " AND ";
      }
    }
    return result;
  }
  
  
  template <class FOREIGN_CLASS> 
  collection<FOREIGN_CLASS> join_many_using_table( const std::string &linkTable, 
                                                  const std::string &linkedSourceKey,
                                                  const std::string &linkTargetKey,int id)  const {  
    collection<FOREIGN_CLASS> result;
    std::stringstream query;
    // #define select * from $targetTable inner join $linkTable on $targetTable.$targetKey=$linkTable.$linkTargetKey and $linkTable.$linkedSourceKey=;
    query << \
    "SELECT * FROM " << FOREIGN_CLASS::classname() << " INNER JOIN " << linkTable << \
    " ON " << FOREIGN_CLASS::classname() << "." << FOREIGN_CLASS::identity_col_ << " = " << linkTable << "." << linkTargetKey << \
    " AND " << linkTable << "." << linkedSourceKey << " = " << id;
    DEBUG_QUERY(query)
    result_set data = Lorm::getInstance()->select(query.str());
    if(data.size() > 0) {
      FOREIGN_CLASS dum; // TODO: get_selection should be a template sdtatic function
      result = dum.get_selection(data);
    }
    return result;
  }
  
  T save_() const {
    std::string cols;
    std::string values;
    std::stringstream query;
    column_and_values_for_insert(cols,values);
    query << "INSERT INTO " << T::classname() << "(" << cols  << ") VALUES (" << values << ");";
    DEBUG_QUERY(query)
    long id = (Lorm::getInstance()->execute(query.str()));
    return T::search_by_id(id);
  }
  
  collection<T> find_() const {
    collection<T> result;
    std::stringstream query;
    query << "SELECT * FROM " << T::classname();
    std::string clause_where=column_and_values_for_select();
    if (!clause_where.empty()) {
      query << " WHERE " << clause_where << ";";
    }
    DEBUG_QUERY(query)
    result_set data = Lorm::getInstance()->select(query.str());
    if(data.size() > 0) {
      result = get_selection(data);
    }
    return result;
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
    std::stringstream query;
    query << "SELECT COUNT(*) AS count FROM " << T::classname();
    std::string clause_where=column_and_values_for_select();
    if (!clause_where.empty()) {
      query << " WHERE " << clause_where << ";";
    }
    DEBUG_QUERY(query)
    result_set data = Lorm::getInstance()->select(query.str());
    return util::from_string<int>(data[0]["count"]);
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
    collection< FOREIGN_CLASS > *pCol= new ( collection< FOREIGN_CLASS > );\
    *pCol=reverse_criteria.find();\
    role##_.reset(pCol);\
  }\
  return *role##_;\
}

#define has_and_belongs_to_many( THIS_CLASS, FOREIGN_CLASS, role, linkTable, linkToSource, linkToTarget)\
collection < FOREIGN_CLASS > &THIS_CLASS::role (bool force_reload) {\
  if (this->id.is_null()) throw "Cannot access roles of an unsaved object";\
  if (!role##_.get() || force_reload) {\
    collection< FOREIGN_CLASS > *pCol= new ( collection< FOREIGN_CLASS > );\
    *pCol=join_many_using_table<FOREIGN_CLASS>(std::string(linkTable), std::string(linkToSource), std::string(linkToTarget),*(this->id.value));\
    role##_.reset(pCol);\
  }\
  return *role##_;\
}

#define TABLE_INIT(K, ...) \
  K(); \
  static void register_table();\
  K save() const; \
  collection<K> find() const; \
  int count() const; \
  K update(K u) const; \
  const K& update() const ; \
  void remove() const; \
  std::string to_string() const; \
  static std::string classname() { std::string cname = #__VA_ARGS__; if(cname.empty()) { return pluralize(lower(#K)); } return cname; }


#define REGISTER_TABLE(K) \
  template <class T> std::vector<lorm::column_t> table<K>::columns_;\
  template <class T> std::string table<K>::identity_col_;\
  K::K() {if (columns_.empty() ) K::register_table(); } \
  K K::save() const { return save_(); } \
  collection<K> K::find() const { return find_(); } \
  int K::count() const { return count_(); } \
  K K::update(K u) const { return update_(this, u); } \
  const K& K::update() const { K t; t.id=id; K::update_(t,*this); return *this;}\
  void K::remove() const { remove_(); } \
  std::string K::to_string() const { return to_string_(); } \
  void K::register_table() 

#undef FIELD_FUN
#undef FIELD_DATA

#endif // __LORM_TABLE_H

