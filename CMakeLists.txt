cmake_minimum_required(VERSION 3.00)
project(DarkFalls)

find_package( Boost COMPONENTS system REQUIRED )
find_package( Threads )

include_directories( ${Boost_INCLUDE_DIR} . )

# - Try to find Mysql-Connector-C++
# Once done, this will define
#
#  MYSQLCONNECTORCPP_FOUND - system has Mysql-Connector-C++ installed
#  MYSQLCONNECTORCPP_INCLUDE_DIRS - the Mysql-Connector-C++ include directories
#  MYSQLCONNECTORCPP_LIBRARIES - link these to use Mysql-Connector-C++
#
# The user may wish to set, in the CMake GUI or otherwise, this variable:
#  MYSQLCONNECTORCPP_ROOT_DIR - path to start searching for the module

set(MYSQLCONNECTORCPP_ROOT_DIR
        "${MYSQLCONNECTORCPP_ROOT_DIR}"
        CACHE
        PATH
        "/usr/include/mysql-cppconn-8") # Where to start looking for this component.

if(WIN32)
    find_path(MYSQLCONNECTORCPP_INCLUDE_DIR
            NAMES
            mysql_connection.h
            PATHS
            "C:\\Program Files"
            HINTS
            ${MYSQLCONNECTORCPP_ROOT_DIR}
            PATH_SUFFIXES
            include)

    find_library(MYSQLCONNECTORCPP_LIBRARY
            NAMES
            mysqlcppconn
            mysqlcppconn-static
            HINTS
            ${MYSQLCONNECTORCPP_ROOT_DIR}
            PATH_SUFFIXES
            lib)

else()
    find_path(MYSQLCONNECTORCPP_INCLUDE_DIR
            mysql_connection.h
            HINTS
            ${MYSQLCONNECTORCPP_ROOT_DIR}
            PATH_SUFFIXES
            include)

    find_library(MYSQLCONNECTORCPP_LIBRARY
            NAMES
            mysqlcppconn
            mysqlcppconn-static
            HINTS
            ${MYSQLCONNECTORCPP_ROOT_DIR}
            PATH_SUFFIXES
            lib64
            lib)
endif()

mark_as_advanced(MYSQLCONNECTORCPP_INCLUDE_DIR MYSQLCONNECTORCPP_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MysqlConnectorCpp
        DEFAULT_MSG
        MYSQLCONNECTORCPP_INCLUDE_DIR
        MYSQLCONNECTORCPP_LIBRARY)

if(MYSQLCONNECTORCPP_FOUND)
    set(MYSQLCONNECTORCPP_INCLUDE_DIRS "${MYSQLCONNECTORCPP_INCLUDE_DIR}") # Add any dependencies here
    set(MYSQLCONNECTORCPP_LIBRARIES "${MYSQLCONNECTORCPP_LIBRARY}") # Add any dependencies here
    mark_as_advanced(MYSQLCONNECTORCPP_ROOT_DIR)
endif()

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wreturn-type -Wreorder)
set(SRCS
        code/main.cpp
        code/Dictionary/Tokenizer.cpp
        code/Grammar/Grammar.cpp
        code/Grammar/Command.cpp
        code/Logic/MudInterface.cpp
        code/Logic/Libraries/Combat.cpp
        code/Logic/Accounts/PlayerAccount.cpp
        code/Logic/Items/Item.cpp
        code/Logic/Items/Item.hpp
        code/Logic/MobStats.cpp
        code/Logic/Mobs/Mob.cpp
        code/Logic/Mobs/Monster.cpp
        code/World/CardinalExit.cpp
        code/World/Room.cpp
        code/World/Area.cpp
        code/World/World.cpp
        code/Server/Server.cpp
        code/Server/ConnectionBase.cpp
        code/Server/LineOrientedConnection.cpp
        code/Database/DBConnection.cpp)

include_directories(${MYSQLCONNECTORCPP_INCLUDE_DIRS})
link_directories(${MYSQLCONNECTORCPP_LIBRARIES})
add_executable(DarkFalls ${SRCS})
target_link_libraries(DarkFalls LINK_PUBLIC ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} mysqlcppconn)