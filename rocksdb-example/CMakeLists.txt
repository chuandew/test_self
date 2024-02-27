# find_path(ROCKSDB_INCLUDE_DIR rocksdb/db.h
#     PATHS "")
# include_directories(${ROCKSDB_INCLUDE_DIR})
# message(STATUS "include path: ${ROCKSDB_INCLUDE_DIR}")

# find_library(ROCKSDB_LIB rocksdb)
# message(STATUS "finding ${ROCKSDB_LIB}")

add_executable(rocksdb_test rocksdb_test.cc)
add_dependencies(rocksdb_test librocksdb)
target_link_libraries(rocksdb_test  
    ${rocksdb_BINARY_DIR}/librocksdb.a 
    pthread)
