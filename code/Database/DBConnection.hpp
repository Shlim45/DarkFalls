//
// Created by shlim on 8/26/22.
//

#ifndef DARKFALLS_DBCONNECTION_HPP
#define DARKFALLS_DBCONNECTION_HPP

#include "../mysql-connector-cpp/jdbc/driver/mysql_connection.h"
#include "../mysql-connector-cpp/jdbc/cppconn/driver.h"
#include "../mysql-connector-cpp/jdbc/cppconn/exception.h"
#include "../mysql-connector-cpp/jdbc/cppconn/prepared_statement.h"

namespace
{
    const std::string server = "tcp://127.0.0.1:3306";
    const std::string username = "mudhost";
    const std::string password = "B@ckstab69";
}

namespace Mud
{
namespace DB
{
    // https://docs.microsoft.com/en-us/azure/mysql/single-server/connect-cpp
    class DBConnection
    {
    public:
        DBConnection()
        {
            try
            {
                m_driver = get_driver_instance();
                m_con = m_driver->connect(server, username, password);
            }
            catch (sql::SQLException &e)
            {
                std::cerr << "[DB]: Could not connect to database server. Error: " << e.what() << std::endl;
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
