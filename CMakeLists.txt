# cmake最小版本需求
cmake_minimum_required(VERSION 3.23.1 FATAL_ERROR)

# 设置此项目的名称
project(test-project)

set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 生成可执行文件target ，后面填写的是生成此可执行文件所依赖的源文件列表。
# add_executable(bdb_test bdb_test.cc)

# 生成库文件，SHARED代表动态库，STATIC代表静态库， 最后一个参数代表此库的源文件列表 add_library(lib_name SHARED or
# STATIC lib_source_code)

# 设置一个名字var_name 的变量，同时给此变量赋值为var_value SET(var_name var_value)

# 指定编译器 CMAKE_C_FLAGS_DEBUG          ----  C 编译器 CMAKE_CXX_FLAGS_DEBUG ----  C++
# 编译器 -std=c++11  使用 C++11 -g：只是编译器，在编译的时候，产生调试信息。
# -Wall：生成所有警告信息。一下是具体的选项，可以单独使用
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17  -g  -Wall  ")

# 指定编译类型，debug 或者为 release debug 会生成相关调试信息，可以使用 GDB 进行 release
# 不会生成调试信息。当无法进行调试时查看此处是否设置为 debug.
set(CMAKE_BUILD_TYPE Debug)

# 打印消息
message("MSG")

# 给变量var_name赋值为var_value，comment是此变量的注释，和SET 有类似的功效，用于给某变量设置默认值 option(var_name
# "comment" var_value)


include(FetchContent)
Set(FETCHCONTENT_QUIET FALSE)
### gflags (before glog), must build before glog
find_package(gflags REQUIRED)

## glog
FetchContent_Declare(
  glog
  GIT_REPOSITORY https://github.com/google/glog.git
  GIT_TAG        96a2f23dca4cc7180821ca5f32e526314395d26a
  GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(glog)

## googletest
# Do this in the main file so it can be fetched before setting project warnings.
# After this is fixed with FetchContent, move to test/CMakeLists.txt.
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        1ed6a8c67a0bd675149ece27bbec0ef1759854cf
  GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(googletest)

## rocksdb
FetchContent_Declare(
  rocksdb
  GIT_REPOSITORY https://github.com/facebook/rocksdb.git
  GIT_TAG        v8.10.0
  GIT_PROGRESS   TRUE
)

FetchContent_Populate(rocksdb)
add_custom_target(librocksdb ALL
  WORKING_DIRECTORY ${rocksdb_SOURCE_DIR}
  COMMAND cmake -DWITH_GFLAGS=Off -DWITH_LIBURING=Off -DWITH_ZSTD=0 -DFAIL_ON_WARNINGS=Off -B ${rocksdb_BINARY_DIR}
  COMMAND cmake --build ${rocksdb_BINARY_DIR} --parallel 8 --target rocksdb
  BYPRODUCTS ${rocksdb_BINARY_DIR}/librocksdb.a
  COMMENT "Building RocksDB"
  USES_TERMINAL
)
include_directories(SYSTEM "${rocksdb_SOURCE_DIR}/include")

# 调用xxx子目录的CMakeLists.txt执行 add_subdirectory(xxx)
add_subdirectory(bdb-example)
add_subdirectory(rocksdb-example)

# 给编译器添加xxx参数
add_compile_options(xxx)

# 添加include路径，也就是头文件路径 
# target_include_directories(bdb_test PUBLIC /root/workspace/bdb/install/include)

# 给编译器添加库目录，有点儿类似-L 
# target_link_directories(bdb_test PUBLIC /root/workspace/bdb/install/lib)

# 给目标添加依赖库，有点儿类似-l(小写的L)
# target_link_libraries(bdb_test db_cxx)
