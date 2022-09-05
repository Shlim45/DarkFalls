//
// Created by shlim on 8/26/22.
//

#ifndef DARKFALLS_DBCONNECTION_HPP
#define DARKFALLS_DBCONNECTION_HPP

#include <mysql-cppconn-8/jdbc/mysql_connection.h>
#include <mysql-cppconn-8/jdbc/cppconn/driver.h>
#include <mysql-cppconn-8/jdbc/cppconn/exception.h>
#include <mysql-cppconn-8/jdbc/cppconn/prepared_statement.h>

namespace Mud
{
namespace Logic
{
    class World;
}
namespace DB
{
    // https://docs.microsoft.com/en-us/azure/mysql/single-server/connect-cpp
    class DBConnection
    {
    public:
        explicit DBConnection(const char *sql_hostname, const char *username, const char *password);

        void QueryDB(const std::string &query);

        void InitializeDB();

        void LoadAreas(Logic::World &world);
        void SaveAreas(Logic::World &world);
        void SaveArea(Logic::World &world, int areaID);

        void LoadRooms(Logic::World &world);
        void SaveRooms(Logic::World &world);
        void SaveRoom(Logic::World &world, int roomID);

        void ShutdownDB()
        {
            delete m_stmt;
            delete m_pstmt;
            delete m_con;
        }

    private:
        sql::Driver *m_driver;
        sql::Connection *m_con;
        sql::Statement *m_stmt;
        sql::PreparedStatement *m_pstmt;
    };

} // Mud
} // DB

#endif //DARKFALLS_DBCONNECTION_HPP
