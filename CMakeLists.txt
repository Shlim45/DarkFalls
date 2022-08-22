cmake_minimum_required(VERSION 3.00)
project(DarkFalls)

find_package( Boost COMPONENTS system REQUIRED )
find_package( Threads )

include_directories( ${Boost_INCLUDE_DIR} . )

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wreturn-type -Wreorder)
set(SRCS
        code/main.cpp
        code/Dictionary/Tokenizer.cpp
        code/Grammar/Grammar.cpp
        code/Grammar/Command.cpp
        code/Logic/MudInterface.cpp
        code/World/Room.cpp
        code/World/Area.cpp
        code/World/World.cpp
        code/Server/Server.cpp
        code/Server/ConnectionBase.cpp
        code/Server/LineOrientedConnection.cpp)

add_executable(DarkFalls ${SRCS})
target_link_libraries(DarkFalls LINK_PUBLIC ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})