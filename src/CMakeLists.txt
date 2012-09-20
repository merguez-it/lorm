cmake_minimum_required (VERSION 2.8)
project(LORM_LIB)

include_directories(
  ${LORM_OUTPUT_TEMP_DIR}
  ${LORM_SOURCE_DIR}/include
  )

set(LORM_SOURCES
  util.cc
  db.cc
  )


if(HAVE_POSTGRESQL)
  set(LORM_SOURCES 
    ${LORM_SOURCES}
    dbi/pg.cc
    )
  set(DBI_LIBS
    ${DBI_LIBS}
    ${PostgreSQL_LIBRARIES}
    )
endif()

if(HAVE_MYSQL)
  set(LORM_SOURCES
    ${LORM_SOURCES}
    dbi/mysql.cc
    )
  set(DBI_LIBS
    ${DBI_LIBS}
    ${MYSQL_LIBRARIES}
    )
endif()

if(HAVE_SQLITE)
  set(LORM_SOURCES
    ${LORM_SOURCES}
    dbi/sqlite.cc
    )
  set(DBI_LIBS
    ${DBI_LIBS}
    ${SQLITE3_LIBRARIES}
    )
endif()

add_library(lorm SHARED ${LORM_SOURCES})
target_link_libraries(lorm ${DBI_LIBS})

add_library(lorm-static STATIC ${LORM_SOURCES})
target_link_libraries(lorm-static ${DBI_LIBS})

