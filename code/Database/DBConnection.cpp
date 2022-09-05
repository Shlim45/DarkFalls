//
// Created by shlim on 9/5/22.
//

#include "DBConnection.hpp"
#include "code/World/World.hpp"

using namespace Mud::DB;

DBConnection::DBConnection(const char *sql_hostname, const char *username, const char *password)
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
}

void DBConnection::QueryDB(const std::string &query)
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

void DBConnection::InitializeDB()
{
    m_con->setSchema("DarkFalls");

    // Create Tables
    m_stmt = m_con->createStatement();
    m_stmt->execute("CREATE TABLE IF NOT EXISTS Areas("
                    "areaID SMALLINT UNSIGNED NOT NULL PRIMARY KEY, "
                    "name VARCHAR(50), "
                    "realm TINYINT UNSIGNED NOT NULL DEFAULT 0"
                    ");");
    delete m_stmt;

    m_stmt = m_con->createStatement();
    m_stmt->execute("CREATE TABLE IF NOT EXISTS Rooms("
                    "roomID MEDIUMINT UNSIGNED NOT NULL, "
                    "areaID SMALLINT UNSIGNED DEFAULT 0 NOT NULL, "
                    "description VARCHAR(255), "
                    "cardinalExits SMALLINT UNSIGNED DEFAULT 0 NOT NULL, "
                    "xCoord TINYINT UNSIGNED DEFAULT 0 NOT NULL, "
                    "yCoord TINYINT UNSIGNED DEFAULT 0 NOT NULL, "
                    "zCoord TINYINT UNSIGNED DEFAULT 0 NOT NULL, "
                    "PRIMARY KEY (roomID), "
                    "FOREIGN KEY (areaID) REFERENCES Areas(areaID) ON UPDATE CASCADE ON DELETE RESTRICT"
                    ");");
    delete m_stmt;
}

void DBConnection::LoadAreas(Logic::World &world)
{
    const std::string query = "SELECT * FROM Areas;";
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

void DBConnection::SaveAreas(Mud::Logic::World &world)
{
    std::cout << "Saving Areas to database..." << std::endl;

    m_pstmt = m_con->prepareStatement("INSERT INTO Areas(areaID, name, realm) VALUES (?, ?, ?) "
                                      "AS new ON DUPLICATE KEY UPDATE "
                                      "areaID=new.areaID, name=new.name, realm=new.realm;");

    auto areaMap = world.Areas();
    uint16_t areaID;
    std::string name;
    uint8_t realm;

    for (auto const &a : areaMap)
    {
        auto &area = a.second;
        areaID = area->AreaID();
        name = area->Name();
        realm = static_cast<uint8_t>(area->GetRealm());

        m_pstmt->setInt(1, areaID);
        m_pstmt->setString(2, name);
        m_pstmt->setInt(3, realm);

        m_pstmt->execute();
    }

    delete m_pstmt;
    std::cout << "Areas saved." << std::endl;
}

void DBConnection::SaveArea(Mud::Logic::World &world, const int areaID)
{
    m_pstmt = m_con->prepareStatement("INSERT INTO Areas(areaID, name, realm) VALUES (?, ?, ?) "
                                      "AS new ON DUPLICATE KEY UPDATE "
                                      "areaID=new.areaID, name=new.name, realm=new.realm;");

    auto &area = world.FindArea(areaID);
    std::string name = area->Name();
    auto realm = static_cast<uint8_t>(area->GetRealm());

    m_pstmt->setInt(1, areaID);
    m_pstmt->setString(2, name);
    m_pstmt->setInt(3, realm);

    m_pstmt->execute();

    delete m_pstmt;

    std::cout << "Area " << name << "(#"<< areaID << ") saved to database." << std::endl;
}

void DBConnection::LoadRooms(Mud::Logic::World &world)
{
    const std::string query = "SELECT * FROM Rooms;";
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

void DBConnection::SaveRooms(Logic::World &world)
{
    std::cout << "Saving Rooms to database..." << std::endl;

    m_pstmt = m_con->prepareStatement("INSERT INTO Rooms VALUES (?, ?, ?, ?, ?, ?, ?) "
                                      "AS new ON DUPLICATE KEY UPDATE roomID=new.roomID, "
                                      "areaID=new.areaID, description=new.description, cardinalExits=new.cardinalExits, "
                                      "xCoord=new.xCoord, yCoord=new.yCoord, zCoord=new.zCoord;");

    auto roomMap = world.Rooms();
    int roomID;
    uint16_t areaID, cExits;
    std::string desc;
    std::tuple<int,int,int> coords;
    uint8_t x, y, z;

    for (auto const &r : roomMap)
    {
        auto &room = r.second;
        roomID = room->RoomID();
        areaID = room->AreaID();
        desc = room->RoomDescription();
        cExits = room->CardinalExits();
        coords = room->Coords();
        x = std::get<0>(coords);
        y = std::get<1>(coords);
        z = std::get<2>(coords);

        m_pstmt->setInt(1, roomID);
        m_pstmt->setInt(2, areaID);
        m_pstmt->setString(3, desc);
        m_pstmt->setInt(4, cExits);
        m_pstmt->setInt(5, x);
        m_pstmt->setInt(6, y);
        m_pstmt->setInt(7, z);

        m_pstmt->execute();
    }

    delete m_pstmt;
    std::cout << "Rooms saved." << std::endl;
}

void DBConnection::SaveRoom(Logic::World &world, const int roomID)
{
    m_pstmt = m_con->prepareStatement("INSERT INTO Rooms VALUES (?, ?, ?, ?, ?, ?, ?) "
                                      "AS new ON DUPLICATE KEY UPDATE roomID=new.roomID, "
                                      "areaID=new.areaID, description=new.description, cardinalExits=new.cardinalExits, "
                                      "xCoord=new.xCoord, yCoord=new.yCoord, zCoord=new.zCoord;");

    auto &room = world.FindRoom(roomID);
    uint16_t areaID, cExits;
    std::string desc;
    std::tuple<int,int,int> coords;
    uint8_t x, y, z;

    areaID = room->AreaID();
    desc = room->RoomDescription();
    cExits = room->CardinalExits();
    coords = room->Coords();
    x = std::get<0>(coords);
    y = std::get<1>(coords);
    z = std::get<2>(coords);

    m_pstmt->setInt(1, roomID);
    m_pstmt->setInt(2, areaID);
    m_pstmt->setString(3, desc);
    m_pstmt->setInt(4, cExits);
    m_pstmt->setInt(5, x);
    m_pstmt->setInt(6, y);
    m_pstmt->setInt(7, z);

    m_pstmt->execute();

    delete m_pstmt;
    std::cout << "Room #" << roomID << " saved to database." << std::endl;
}