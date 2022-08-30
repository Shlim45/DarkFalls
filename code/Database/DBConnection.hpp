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
namespace DB
{
    // https://docs.microsoft.com/en-us/azure/mysql/single-server/connect-cpp
    class DBConnection
    {
    public:
        explicit DBConnection(const char *sql_hostname, const char *username, const char *password)
        {
            try
            {
                m_driver = get_driver_instance();
                m_con = m_driver->connect(sql_hostname, username, password);
            }
            catch (sql::SQLException &e)
            {
                std::cerr << "[DB]: Could not connect to database sql_hostname. Error: " << e.what() << std::endl;
                return;
            }

            m_con->setSchema("DarkFalls");
        }

        void QueryDB(const std::string &query)
        {
            m_pstmt = m_con->prepareStatement(query);
            auto rs = m_pstmt->executeQuery();

            while (rs->next())
                std::cout << "Reading from table=(" << rs->getInt(1)
                          << ", " << rs->getString(2)
                          << ", " << rs->getInt(3)
                          << ")" << std::endl;

            delete rs;
            delete m_pstmt;
        }

        void LoadAreas(Logic::World &world)
        {
            const std::string query = "SELECT * FROM Areas";
            m_pstmt = m_con->prepareStatement(query);
            auto rs = m_pstmt->executeQuery();

            while (rs->next())
            {
                int areaID = rs->getInt("areaID");
                std::string name = rs->getString("name");
                uint8_t realm = rs->getInt("realm");

                world.GenerateArea(areaID, name, static_cast<Logic::Realm>(realm));
            }

            delete rs;
            delete m_pstmt;
        }

        void LoadRooms(Logic::World &world)
        {
            const std::string query = "SELECT * FROM Rooms";
            m_pstmt = m_con->prepareStatement(query);
            auto rs = m_pstmt->executeQuery();

            while (rs->next())
            {
                const int roomID = rs->getInt("roomID");
                const int areaID = rs->getInt("areaID");
                const std::string description = rs->getString("description");
                const uint16_t cardinalExits = rs->getInt("cardinalExits");
                const uint8_t xCoord = rs->getInt("xCoord");
                const uint8_t yCoord = rs->getInt("yCoord");
                const uint8_t zCoord = rs->getInt("zCoord");

                world.GenerateRoom(roomID, description, areaID, xCoord, yCoord, zCoord, cardinalExits);
            }

            delete rs;
            delete m_pstmt;
        }

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
