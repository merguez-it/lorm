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

#define IDENTITY_NOT_SET -1

typedef std::vector<lorm::column_t> columns_desc;

template <class T> class table {
  public: 
    static std::string identity_col_;
    static columns_desc columns_;
    ID_FUN(int)
    FIELD_FUN(int, lorm::SQL_INTEGER)
    FIELD_FUN(std::string, lorm::SQL_STRING)
    FIELD_FUN(double, lorm::SQL_NUMERIC)
    FIELD_FUN(datetime,lorm::SQL_DATETIME)
  
    static void has_one(const std::string & col, column<int> T::* f) {
      field(col,f,IDENTITY_NOT_SET);
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
      std::vector<std::map<std::string, std::string> > data = Lorm::getInstance()->select(query.str());
      if(data.size() > 1) {
        throw "Unique id is not unique : "+ id;
      }
      if(data.size() == 1) {
        result = (result.get_selection(data, result))[0];
      }

      return result;
    }
    
  collection<T> get_selection(std::vector<std::map<std::string, std::string> > data, T t) {
    collection<T> result_list;
    
    std::vector<std::map<std::string, std::string> >::iterator itd;
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
  
    template <typename CPPTYPE> column<CPPTYPE> T::* offset_to_columnref(unsigned long offset) {
      column<CPPTYPE> T::* g;
      memcpy(&g, &offset, sizeof(unsigned long));
      return g;
    }
  
    abstract_column& offset_to_column(unsigned long offset) {
      abstract_column *col;
      col=reinterpret_cast<abstract_column *>(this+offset); // Arrière, Satan !!
      return *col;
    }
    
    std::string column_and_values(std::string next_keyword, const std::string &separator) {
      std::string result;
      columns_desc::iterator it;
      for(it = columns_.begin(); it != columns_.end(); it++) {
        abstract_column& col=offset_to_column((*it).offset);
        std::string condition = col.where((*it).name);
        if(!condition.empty()) {
          result += next_keyword + condition;
          next_keyword = separator;
        }
      }
      return result;
    }
  
    void column_and_values_for_insert(/* out */ std::string& cols,/* out */ std::string& values) {
      columns_desc::iterator it;
      std::string sep="";
      for(it = columns_.begin(); it != columns_.end(); it++) {
        abstract_column& col = offset_to_column((*it).offset);
        if( !col.is_null() ) {
          cols  += sep + (*it).name;
          values += sep + col.as_sql_litteral();
          sep = ", ";
        }
      }
    }

    std::string column_and_values_for_select() {
      std::string result;
      std::string sep="";
      columns_desc::iterator it;
      for(it = columns_.begin(); it != columns_.end(); it++) {
        abstract_column& col = offset_to_column( (*it).offset );
        std::string condition = col.where((*it).name);
        if( !condition.empty() ) {
          result += sep + condition;
          sep = " AND ";
        }
      }
      return result;
    }
  
    T save_(T* const t) {
      return save_(*t);
    }
  
    T save_(T t) { //TODO: Make this method working for any case : insert or update, no matter)
      std::string cols;
      std::string values;
      std::stringstream query;
      t.column_and_values_for_insert(cols,values);
      query << "INSERT INTO " << T::classname() << "(" << cols  << ") VALUES (" << values << ");";
      DEBUG_QUERY(query)
      long id = (Lorm::getInstance()->execute(query.str()));
      return T::search_by_id(id);
    }

    collection<T> find_(T* const t) {
      return find_(*t);
    }
    collection<T> find_(T t) {
      collection<T> result;
      std::stringstream query;
      query << "SELECT * FROM " << T::classname();
      std::string clause_where=t.column_and_values_for_select();
      if (!clause_where.empty()) {
        query << " WHERE " << clause_where << ";";
      }
      DEBUG_QUERY(query)
      std::vector<std::map<std::string, std::string> > data = Lorm::getInstance()->select(query.str());
      if(data.size() > 0) {
        result = t.get_selection(data, t);
      }
      return result;
    }

    void remove_(T* const t) {
      remove_(*t);
    }
    void remove_(T t) {
      std::stringstream query;
      query << "DELETE FROM " << T::classname();
      std::string clause_where=t.column_and_values_for_select();
      if (!clause_where.empty()) {
        query << " WHERE " << clause_where << ";";
      }
      DEBUG_QUERY(query)
      Lorm::getInstance()->execute(query.str());
    }

    int count_(T* const t) {
      return count_(*t);
    }
    int count_(T t) {
      std::stringstream query;
      query << "SELECT COUNT(*) AS count FROM " << T::classname();
      std::string clause_where=t.column_and_values_for_select();
      if (!clause_where.empty()) {
        query << " WHERE " << clause_where << ";";
      }
      DEBUG_QUERY(query)
      std::vector<std::map<std::string, std::string> > data = Lorm::getInstance()->select(query.str());
      return util::from_string<int>(data[0]["count"]);
    }

    T update_(T* const t, T u) {
      return update_(*t, u);
    }
    T update_(T t, T u) { //TODO: virer t pour utiliser plutôt "this", non ?
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

    std::string to_string_(T* const t) {
      return to_string_(*t);
    }
  
    std::string to_string_(T t) {
      return t.column_and_values("\t", "\n\t");
    }
    
    template <class FOREIGN> FOREIGN load_has_one(int foreign_id) {
      FOREIGN result;
      
      std::stringstream query;
      query << "SELECT * FROM " << FOREIGN::classname();
      query << " WHERE " << FOREIGN::identity_col_ << " = " << foreign_id << ";"; 
      DEBUG_QUERY(query)
      std::vector<std::map<std::string, std::string> > data = Lorm::getInstance()->select(query.str());
      if(data.size() > 1) {
        throw "Unique object id is not unique :" + foreign_id;
      }
      if(data.size() == 1) {
        result = (result.get_selection(data, result))[0];
      }
      return result;
    }
};

#define SELECT_MACRO_FROM_N(N,FOREIGN_CLASS,...)  CONCATENATE(HAS_ONE_,N)(FOREIGN_CLASS,__VA_ARGS__)
#define HAS_ONE(FOREIGN_CLASS,...) SELECT_MACRO_FROM_N(COUNT_ARGS(__VA_ARGS__),FOREIGN_CLASS,__VA_ARGS__)

#define HAS_ONE_1(FOREIGN_CLASS,role)  HAS_ONE_2(FOREIGN_CLASS,role,role##_id)

#define HAS_ONE_2(FOREIGN_CLASS,role,roleId)\
private:\
  std::tr1::shared_ptr<FOREIGN_CLASS> role##_;\
public:\
  column<int> roleId;\
  FOREIGN_CLASS role (bool force_reload=false) {\
    if (!role##_.get() || force_reload) {\
      role##_.reset(new FOREIGN_CLASS());\
      *role##_=this->load_has_one<FOREIGN_CLASS>(*(roleId.value));\
    }\
    return *role##_;\
  }\
  void role (const FOREIGN_CLASS &that_role) {\
    if (IDENTITY_NOT_SET==*(that_role.id.value)) {\
      throw "Identity not set !!";\
    }\
    roleId=that_role.id.value;\
    FOREIGN_CLASS *p=new FOREIGN_CLASS(that_role);\
    role##_.reset(p);\
  }\


#define TABLE_INIT(K, ...) \
  K(); \
  static void register_table();\
  K save(); \
  collection<K> find(); \
  int count(); \
  K update(K u); \
  void remove(); \
  std::string to_string(); \
  static std::string classname() { std::string cname = #__VA_ARGS__; if(cname.empty()) { return pluralize(lower(#K)); } return cname; }


#define REGISTER_TABLE(K) \
  template <class T> std::vector<lorm::column_t> table<K>::columns_;\
  template <class T> std::string table<K>::identity_col_;\
  K::K() {if (columns_.empty() ) K::register_table(); } \
  K K::save() { return save_(this); } \
  collection<K> K::find() { return find_(this); } \
  int K::count() { return count_(this); } \
  K K::update(K u) { return update_(this, u); } \
  void K::remove() { remove_(this); } \
  std::string K::to_string() { return to_string_(this); } \
  void K::register_table() 

#undef FIELD_FUN
#undef FIELD_DATA

#endif // __LORM_TABLE_H

