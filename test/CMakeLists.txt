cmake_minimum_required (VERSION 2.8)
project(LORM_TESTS)

include(FindGTest)

include_directories(${LORM_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

add_executable(datetime_unittest "datetime_unittest.cc")
target_link_libraries(datetime_unittest lorm-static ${GTEST_BOTH_LIBRARIES})
add_test(DATETIME_UNITTEST datetime_unittest)