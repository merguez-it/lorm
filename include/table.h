#ifndef __LORM_TABLE_H
#define __LORM_TABLE_H

#include "column.h"
#include "collection.h"
#include "db.h"
#include "util.h"
#include "types.h"
#include "datetime.h"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__) 
#define FOR_EACH_ARG_N( \
     _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
    _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, \
    _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, \
    _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, \
    _91, _92, _93, _94, _95, _96, _97, _98, _99,   N, ...) N 
#define FOR_EACH_RSEQ_N() \
    99, 98, 97, 96, 95, 94, 93, 92, 91, 90, \
    89, 88, 87, 86, 85, 84, 83, 82, 81, 80, \
    79, 78, 77, 76, 75, 74, 73, 72, 71, 70, \
    69, 68, 67, 66, 65, 64, 63, 62, 61, 60, \
    59, 58, 57, 56, 55, 54, 53, 52, 51, 50, \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
    39, 38, 37, 36, 35, 34, 33, 32, 31, 30, \
    29, 28, 27, 26, 25, 24, 23, 22, 21, 20, \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
     9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define FOR_EACH_1(what, o, x, ...) what(o, x)
#define FOR_EACH_2(what, o, x, ...) what(o, x) FOR_EACH_1(what, o, __VA_ARGS__)
#define FOR_EACH_3(what, o, x, ...) what(o, x) FOR_EACH_2(what, o, __VA_ARGS__)
#define FOR_EACH_4(what, o, x, ...) what(o, x) FOR_EACH_3(what, o, __VA_ARGS__)
#define FOR_EACH_5(what, o, x, ...) what(o, x) FOR_EACH_4(what, o, __VA_ARGS__)
#define FOR_EACH_6(what, o, x, ...) what(o, x) FOR_EACH_5(what, o, __VA_ARGS__)
#define FOR_EACH_7(what, o, x, ...) what(o, x) FOR_EACH_6(what, o, __VA_ARGS__)
#define FOR_EACH_8(what, o, x, ...) what(o, x) FOR_EACH_7(what, o, __VA_ARGS__)
#define FOR_EACH_9(what, o, x, ...) what(o, x) FOR_EACH_8(what, o, __VA_ARGS__)
#define FOR_EACH_10(what, o, x, ...) what(o, x) FOR_EACH_9(what, o, __VA_ARGS__)
#define FOR_EACH_11(what, o, x, ...) what(o, x) FOR_EACH_10(what, o, __VA_ARGS__)
#define FOR_EACH_12(what, o, x, ...) what(o, x) FOR_EACH_11(what, o, __VA_ARGS__)
#define FOR_EACH_13(what, o, x, ...) what(o, x) FOR_EACH_12(what, o, __VA_ARGS__)
#define FOR_EACH_14(what, o, x, ...) what(o, x) FOR_EACH_13(what, o, __VA_ARGS__)
#define FOR_EACH_15(what, o, x, ...) what(o, x) FOR_EACH_14(what, o, __VA_ARGS__)
#define FOR_EACH_16(what, o, x, ...) what(o, x) FOR_EACH_15(what, o, __VA_ARGS__)
#define FOR_EACH_17(what, o, x, ...) what(o, x) FOR_EACH_16(what, o, __VA_ARGS__)
#define FOR_EACH_18(what, o, x, ...) what(o, x) FOR_EACH_17(what, o, __VA_ARGS__)
#define FOR_EACH_19(what, o, x, ...) what(o, x) FOR_EACH_18(what, o, __VA_ARGS__)
#define FOR_EACH_20(what, o, x, ...) what(o, x) FOR_EACH_19(what, o, __VA_ARGS__)
#define FOR_EACH_21(what, o, x, ...) what(o, x) FOR_EACH_20(what, o, __VA_ARGS__)
#define FOR_EACH_22(what, o, x, ...) what(o, x) FOR_EACH_21(what, o, __VA_ARGS__)
#define FOR_EACH_23(what, o, x, ...) what(o, x) FOR_EACH_22(what, o, __VA_ARGS__)
#define FOR_EACH_24(what, o, x, ...) what(o, x) FOR_EACH_23(what, o, __VA_ARGS__)
#define FOR_EACH_25(what, o, x, ...) what(o, x) FOR_EACH_24(what, o, __VA_ARGS__)
#define FOR_EACH_26(what, o, x, ...) what(o, x) FOR_EACH_25(what, o, __VA_ARGS__)
#define FOR_EACH_27(what, o, x, ...) what(o, x) FOR_EACH_26(what, o, __VA_ARGS__)
#define FOR_EACH_28(what, o, x, ...) what(o, x) FOR_EACH_27(what, o, __VA_ARGS__)
#define FOR_EACH_29(what, o, x, ...) what(o, x) FOR_EACH_28(what, o, __VA_ARGS__)
#define FOR_EACH_30(what, o, x, ...) what(o, x) FOR_EACH_29(what, o, __VA_ARGS__)
#define FOR_EACH_31(what, o, x, ...) what(o, x) FOR_EACH_30(what, o, __VA_ARGS__)
#define FOR_EACH_32(what, o, x, ...) what(o, x) FOR_EACH_31(what, o, __VA_ARGS__)
#define FOR_EACH_33(what, o, x, ...) what(o, x) FOR_EACH_32(what, o, __VA_ARGS__)
#define FOR_EACH_34(what, o, x, ...) what(o, x) FOR_EACH_33(what, o, __VA_ARGS__)
#define FOR_EACH_35(what, o, x, ...) what(o, x) FOR_EACH_34(what, o, __VA_ARGS__)
#define FOR_EACH_36(what, o, x, ...) what(o, x) FOR_EACH_35(what, o, __VA_ARGS__)
#define FOR_EACH_37(what, o, x, ...) what(o, x) FOR_EACH_36(what, o, __VA_ARGS__)
#define FOR_EACH_38(what, o, x, ...) what(o, x) FOR_EACH_37(what, o, __VA_ARGS__)
#define FOR_EACH_39(what, o, x, ...) what(o, x) FOR_EACH_38(what, o, __VA_ARGS__)
#define FOR_EACH_40(what, o, x, ...) what(o, x) FOR_EACH_39(what, o, __VA_ARGS__)
#define FOR_EACH_41(what, o, x, ...) what(o, x) FOR_EACH_40(what, o, __VA_ARGS__)
#define FOR_EACH_42(what, o, x, ...) what(o, x) FOR_EACH_41(what, o, __VA_ARGS__)
#define FOR_EACH_43(what, o, x, ...) what(o, x) FOR_EACH_42(what, o, __VA_ARGS__)
#define FOR_EACH_44(what, o, x, ...) what(o, x) FOR_EACH_43(what, o, __VA_ARGS__)
#define FOR_EACH_45(what, o, x, ...) what(o, x) FOR_EACH_44(what, o, __VA_ARGS__)
#define FOR_EACH_46(what, o, x, ...) what(o, x) FOR_EACH_45(what, o, __VA_ARGS__)
#define FOR_EACH_47(what, o, x, ...) what(o, x) FOR_EACH_46(what, o, __VA_ARGS__)
#define FOR_EACH_48(what, o, x, ...) what(o, x) FOR_EACH_47(what, o, __VA_ARGS__)
#define FOR_EACH_49(what, o, x, ...) what(o, x) FOR_EACH_48(what, o, __VA_ARGS__)
#define FOR_EACH_50(what, o, x, ...) what(o, x) FOR_EACH_49(what, o, __VA_ARGS__)
#define FOR_EACH_51(what, o, x, ...) what(o, x) FOR_EACH_50(what, o, __VA_ARGS__)
#define FOR_EACH_52(what, o, x, ...) what(o, x) FOR_EACH_51(what, o, __VA_ARGS__)
#define FOR_EACH_53(what, o, x, ...) what(o, x) FOR_EACH_52(what, o, __VA_ARGS__)
#define FOR_EACH_54(what, o, x, ...) what(o, x) FOR_EACH_53(what, o, __VA_ARGS__)
#define FOR_EACH_55(what, o, x, ...) what(o, x) FOR_EACH_54(what, o, __VA_ARGS__)
#define FOR_EACH_56(what, o, x, ...) what(o, x) FOR_EACH_55(what, o, __VA_ARGS__)
#define FOR_EACH_57(what, o, x, ...) what(o, x) FOR_EACH_56(what, o, __VA_ARGS__)
#define FOR_EACH_58(what, o, x, ...) what(o, x) FOR_EACH_57(what, o, __VA_ARGS__)
#define FOR_EACH_59(what, o, x, ...) what(o, x) FOR_EACH_58(what, o, __VA_ARGS__)
#define FOR_EACH_60(what, o, x, ...) what(o, x) FOR_EACH_59(what, o, __VA_ARGS__)
#define FOR_EACH_61(what, o, x, ...) what(o, x) FOR_EACH_60(what, o, __VA_ARGS__)
#define FOR_EACH_62(what, o, x, ...) what(o, x) FOR_EACH_61(what, o, __VA_ARGS__)
#define FOR_EACH_63(what, o, x, ...) what(o, x) FOR_EACH_62(what, o, __VA_ARGS__)
#define FOR_EACH_64(what, o, x, ...) what(o, x) FOR_EACH_63(what, o, __VA_ARGS__)
#define FOR_EACH_65(what, o, x, ...) what(o, x) FOR_EACH_64(what, o, __VA_ARGS__)
#define FOR_EACH_66(what, o, x, ...) what(o, x) FOR_EACH_65(what, o, __VA_ARGS__)
#define FOR_EACH_67(what, o, x, ...) what(o, x) FOR_EACH_66(what, o, __VA_ARGS__)
#define FOR_EACH_68(what, o, x, ...) what(o, x) FOR_EACH_67(what, o, __VA_ARGS__)
#define FOR_EACH_69(what, o, x, ...) what(o, x) FOR_EACH_68(what, o, __VA_ARGS__)
#define FOR_EACH_70(what, o, x, ...) what(o, x) FOR_EACH_69(what, o, __VA_ARGS__)
#define FOR_EACH_71(what, o, x, ...) what(o, x) FOR_EACH_70(what, o, __VA_ARGS__)
#define FOR_EACH_72(what, o, x, ...) what(o, x) FOR_EACH_71(what, o, __VA_ARGS__)
#define FOR_EACH_73(what, o, x, ...) what(o, x) FOR_EACH_72(what, o, __VA_ARGS__)
#define FOR_EACH_74(what, o, x, ...) what(o, x) FOR_EACH_73(what, o, __VA_ARGS__)
#define FOR_EACH_75(what, o, x, ...) what(o, x) FOR_EACH_74(what, o, __VA_ARGS__)
#define FOR_EACH_76(what, o, x, ...) what(o, x) FOR_EACH_75(what, o, __VA_ARGS__)
#define FOR_EACH_77(what, o, x, ...) what(o, x) FOR_EACH_76(what, o, __VA_ARGS__)
#define FOR_EACH_78(what, o, x, ...) what(o, x) FOR_EACH_77(what, o, __VA_ARGS__)
#define FOR_EACH_79(what, o, x, ...) what(o, x) FOR_EACH_78(what, o, __VA_ARGS__)
#define FOR_EACH_80(what, o, x, ...) what(o, x) FOR_EACH_79(what, o, __VA_ARGS__)
#define FOR_EACH_81(what, o, x, ...) what(o, x) FOR_EACH_80(what, o, __VA_ARGS__)
#define FOR_EACH_82(what, o, x, ...) what(o, x) FOR_EACH_81(what, o, __VA_ARGS__)
#define FOR_EACH_83(what, o, x, ...) what(o, x) FOR_EACH_82(what, o, __VA_ARGS__)
#define FOR_EACH_84(what, o, x, ...) what(o, x) FOR_EACH_83(what, o, __VA_ARGS__)
#define FOR_EACH_85(what, o, x, ...) what(o, x) FOR_EACH_84(what, o, __VA_ARGS__)
#define FOR_EACH_86(what, o, x, ...) what(o, x) FOR_EACH_85(what, o, __VA_ARGS__)
#define FOR_EACH_87(what, o, x, ...) what(o, x) FOR_EACH_86(what, o, __VA_ARGS__)
#define FOR_EACH_88(what, o, x, ...) what(o, x) FOR_EACH_87(what, o, __VA_ARGS__)
#define FOR_EACH_89(what, o, x, ...) what(o, x) FOR_EACH_88(what, o, __VA_ARGS__)
#define FOR_EACH_90(what, o, x, ...) what(o, x) FOR_EACH_89(what, o, __VA_ARGS__)
#define FOR_EACH_91(what, o, x, ...) what(o, x) FOR_EACH_90(what, o, __VA_ARGS__)
#define FOR_EACH_92(what, o, x, ...) what(o, x) FOR_EACH_91(what, o, __VA_ARGS__)
#define FOR_EACH_93(what, o, x, ...) what(o, x) FOR_EACH_92(what, o, __VA_ARGS__)
#define FOR_EACH_94(what, o, x, ...) what(o, x) FOR_EACH_93(what, o, __VA_ARGS__)
#define FOR_EACH_95(what, o, x, ...) what(o, x) FOR_EACH_94(what, o, __VA_ARGS__)
#define FOR_EACH_96(what, o, x, ...) what(o, x) FOR_EACH_95(what, o, __VA_ARGS__)
#define FOR_EACH_97(what, o, x, ...) what(o, x) FOR_EACH_96(what, o, __VA_ARGS__)

#define FOR_EACH_(N, what, o, x, ...) CONCATENATE(FOR_EACH_, N)(what, o, x, __VA_ARGS__)
#define FOR_EACH(what, o, x, ...) FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, o, x, __VA_ARGS__)

#define DO_REGISTER_(o, x) \
  if(!o.x.is_null()) { \
    x = o.x; \
  }

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

template <class T, int V = 1> class table {
  public: 

    ID_FUN(int)
    FIELD_FUN(int, lorm::SQL_INTEGER)
    FIELD_FUN(std::string, lorm::SQL_STRING)
    FIELD_FUN(double, lorm::SQL_NUMERIC)
//    FIELD_FUN(datetime,lorm::SQL_DATETIME)
    
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
        throw 1;
      }
      if(data.size() == 1) {
        result = (result.get_selection(data, result))[0];
      }

      return result;
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
      std::vector<lorm::column_t>::iterator it;
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
      std::vector<lorm::column_t>::iterator it;
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
      std::vector<lorm::column_t>::iterator it;
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
  
    T save_(T t) {
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
      keyword= " WHERE ";
      query << t.column_and_values(keyword," AND ");
      DEBUG_QUERY(query)
      Lorm::getInstance()->execute(query.str());
      T result = t;
      result = u;
      return result;
    }

    std::string to_string_(T* const t) {
      return to_string_(*t);
    }
  
    std::string to_string_(T t) {
      return t.column_and_values("\t", "\n\t");
    }
  
    collection<T> get_selection(std::vector<std::map<std::string, std::string> > data, T t) {
      collection<T> result_list;

      std::vector<std::map<std::string, std::string> >::iterator itd;
      for(itd = data.begin(); itd != data.end(); itd++) {
        T result;
        std::vector<lorm::column_t>::iterator it;

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
//              case lorm::SQL_DATETIME:
//              {
//                column<datetime> T::* f=offset_to_columnref<datetime>((*it).offset);
//                (result.*f) = datetime::from_sql((*itd)[(*it).name]);
//              }
//              break;
              case lorm::SQL_NUMERIC:
                {
                  column<double> T::* f=offset_to_columnref<double>((*it).offset);
                  (result.*f) = util::from_string<double>((*itd)[(*it).name]);
                }
                break;
              default:
                throw 1; // TODO
            }
          }
        }

        result_list.push_back(result);
      }

      return result_list;
    }
  protected:
    static std::string identity_col_;
    static std::vector<lorm::column_t> columns_;
};

#define TABLE_INIT(K) \
  K(); \
  static void register_table();\
  void operator=(const K& o); \
  K save(); \
  collection<K> find(); \
  int count(); \
  K update(K u); \
  void remove(); \
  std::string to_string(); \
  static std::string classname() { return pluralize(lower(#K)); }


#define REGISTER_TABLE(K, ...) \
  template <class K, int V> std::vector<lorm::column_t> table<K,V>::columns_;\
  template <class K, int V> std::string table<K,V>::identity_col_;\
  void K::operator=(const K& o) { \
    FOR_EACH(DO_REGISTER_, o, __VA_ARGS__) \
  }\
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

