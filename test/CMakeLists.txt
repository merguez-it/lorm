cmake_minimum_required (VERSION 2.8)
project(LORM_TESTS)

include_directories(
  ${LORM_EXTERN_SOURCE_DIR}/gtest-1.6.0/include
  ${LORM_SOURCE_DIR}/include
  )

set(TESTS_LIBS
  lorm-static
  gtest
  gtest_main
  )

add_executable(uri_unittest "uri_unittest.cc")
target_link_libraries(uri_unittest ${TESTS_LIBS})
add_test(URI_UNITTEST uri_unittest)

add_executable(datetime_unittest "datetime_unittest.cc")
target_link_libraries(datetime_unittest ${TESTS_LIBS})
add_test(DATETIME_UNITTEST datetime_unittest)

add_executable(person_unittest "person_unittest.cc")
target_link_libraries(person_unittest ${TESTS_LIBS})
add_test(PERSON_UNITTEST person_unittest)

add_executable(to_one_unittest "to_one_unittest.cc")
target_link_libraries(to_one_unittest ${TESTS_LIBS})
add_test(TO_ONE_UNITTEST to_one_unittest)

if(MYSQL_FOUND)
  add_executable(table_mysql_unittest "table_mysql_unittest.cc")
  target_link_libraries(table_mysql_unittest ${TESTS_LIBS})
  add_test(TABLE_MYSQL_UNITTEST table_mysql_unittest)
endif()
