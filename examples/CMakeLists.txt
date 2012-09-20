cmake_minimum_required (VERSION 2.8)
project(LORM_EXAMPLES)

include_directories(
  ${LORM_EXAMPLES_SOURCE_DIR}
  ${LORM_SOURCE_DIR}/include
  )

add_executable(sample_person "person.cc")
target_link_libraries(sample_person lorm-static)
