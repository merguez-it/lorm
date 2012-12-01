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

add_executable(datetime_unittest "datetime_unittest.cc")
target_link_libraries(datetime_unittest ${TESTS_LIBS})
add_test(DATETIME_UNITTEST datetime_unittest)

add_executable(table_unittest "table_unittest.cc")
target_link_libraries(table_unittest ${TESTS_LIBS})
add_test(TABLE_UNITTEST table_unittest)

add_executable(to_one_unittest "to_one_unittest.cc")
target_link_libraries(to_one_unittest ${TESTS_LIBS})
add_test(TO_ONE_UNITTEST to_one_unittest)

add_executable(many_to_many_unittest "many_to_many_unittest.cc")
target_link_libraries(many_to_many_unittest ${TESTS_LIBS})
add_test(MANY_TO_MANY_UNITTEST many_to_many_unittest)
