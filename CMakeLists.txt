cmake_minimum_required(VERSION 3.00)
project(DarkFalls)

find_package( Boost COMPONENTS system REQUIRED )
find_package( Threads )

include_directories( ${Boost_INCLUDE_DIR} . )

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wreturn-type -Wreorder)
set(SRCS
        code/main.cpp
        code/Server/Connection.cpp
        code/Server/LineOrientedConnection.cpp)

add_executable(DarkFalls ${SRCS})
target_link_libraries(DarkFalls LINK_PUBLIC ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})