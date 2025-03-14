cmake_minimum_required(VERSION 3.15)
project(RunTimeTest)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_LIBRARY_PATH "/usr/local/lib/")

set(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# 添加包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

file(GLOB SRC ${PROJECT_SOURCE_DIR}/source/*.cpp)

# 添加可执行文件
add_executable(demo ${SRC})


#给编译器加一个选项，可以让PBC库调用gmp库，否则PBC不能通过pbc调用gmp
set(CMAKE_CXX_FLAGS "-Wl,--copy-dt-needed-entries")


find_library(LIBgmp gmp)
find_library(LIBpbc pbc)
find_library(LIBPBC PBC)
find_library(LIBCRPTOPP cryptopp)

# 链接库
target_link_libraries(demo PRIVATE ${LIBgmp} ${LIBpbc} ${LIBgmp} ${LIBPBC} ${LIBCRPTOPP})
