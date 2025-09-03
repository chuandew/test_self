set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # 调试模式选项
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -Wpedantic -Werror")
    message(STATUS "Building in Debug mode")
else()
    # 发布模式选项
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
    message(STATUS "Building in Release mode")
endif()

# 检查必需的依赖
find_package(Threads REQUIRED)  # 线程库

# 检查平台特定的库
if(UNIX AND NOT APPLE)
    # Linux系统需要rt库（用于clock_gettime等）
    find_library(RT_LIBRARY rt)
    if(RT_LIBRARY)
        message(STATUS "Found rt library: ${RT_LIBRARY}")
    endif()
endif()

# 添加可执行文件
add_executable(event_loop_demo
    main.cc
    simple_event_loop.cc
)

# 设置目标属性
target_include_directories(event_loop_demo
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}  # 包含当前目录，用于找到头文件
)

# 链接必要的库
target_link_libraries(event_loop_demo
    PRIVATE
    Threads::Threads  # 线程库
    ${RT_LIBRARY}     # Linux的rt库（如果找到）
)

# 添加自定义目标用于代码格式检查（可选）
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i --style=file
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting source code with clang-format"
    )
endif()

# 添加自定义目标用于静态分析（可选）
find_program(CPPCHECK "cppcheck")
if(CPPCHECK)
    add_custom_target(static_analysis
        COMMAND ${CPPCHECK}
        --enable=all
        --suppress=missingInclude
        --std=c++17
        ${CMAKE_CURRENT_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running static analysis with cppcheck"
    )
endif()

# 添加文档生成目标（可选）
find_program(DOXYGEN "doxygen")
if(DOXYGEN)
    add_custom_target(docs
        COMMAND ${DOXYGEN} Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
    )
endif()

# 添加安装后信息显示
message(STATUS "==============================================")
message(STATUS "Simple Event Loop Project Configuration")
message(STATUS "==============================================")
message(STATUS "Project Name:    ${PROJECT_NAME}")
message(STATUS "Version:         ${PROJECT_VERSION}")
message(STATUS "C++ Standard:    ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:      ${CMAKE_BUILD_TYPE}")
message(STATUS "Source Directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Binary Directory: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "Threads Library: ${CMAKE_THREAD_LIBS_INIT}")
if(RT_LIBRARY)
    message(STATUS "RT Library:      ${RT_LIBRARY}")
endif()
message(STATUS "==============================================")