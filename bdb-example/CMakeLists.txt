
add_executable(bdb_test bdb_test.cc)

# 添加include路径，也就是头文件路径 
target_include_directories(bdb_test PUBLIC /home/chuande/workspace/libdb/install/include)

# 给编译器添加库目录，有点儿类似-L 
target_link_directories(bdb_test PUBLIC /home/chuande/workspace/libdb/install/lib)

# 给目标添加依赖库，有点儿类似-l(小写的L)
target_link_libraries(bdb_test db_cxx)