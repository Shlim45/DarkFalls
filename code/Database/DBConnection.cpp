//
// Created by shlim on 9/5/22.
//

#include "DBConnection.hpp"
#include "code/World/World.hpp"
#include "code/Logic/Items/Item.hpp"
#include "code/Logic/Mobs/Monster.hpp"
#include "code/Logic/Mobs/Player.hpp"

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

//    m_stmt->execute("DROP TABLE IF EXISTS MonsterCatalog");

    m_stmt->execute("CREATE TABLE IF NOT EXISTS MonsterCatalog("
                    "monsterID MEDIUMINT UNSIGNED NOT NULL, "
                    "article VARCHAR(3), "
                    "name VARCHAR(50) NOT NULL, "
                    "keyword VARCHAR(50) NOT NULL, "
                    "realm TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "exp MEDIUMINT UNSIGNED NOT NULL DEFAULT 0, "
                    "level TINYINT UNSIGNED NOT NULL DEFAULT 1, "
                    "hits SMALLINT UNSIGNED NOT NULL DEFAULT 0, "
                    "fat SMALLINT UNSIGNED NOT NULL DEFAULT 0, "
                    "power SMALLINT UNSIGNED NOT NULL DEFAULT 0, "
                    "str TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "con TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "agi TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "dex TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "intel TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "wis TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "PRIMARY KEY (monsterID)"
                    ");");

    m_stmt->execute("CREATE TABLE IF NOT EXISTS Characters("
                    "name VARCHAR(12) NOT NULL, "
                    "secFlags MEDIUMINT UNSIGNED NOT NULL DEFAULT 0, "
                    "realm TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "exp MEDIUMINT UNSIGNED NOT NULL DEFAULT 0, "
                    "level TINYINT UNSIGNED NOT NULL DEFAULT 1, "
                    "hits SMALLINT UNSIGNED NOT NULL DEFAULT 0, "
                    "fat SMALLINT UNSIGNED NOT NULL DEFAULT 0, "
                    "power SMALLINT UNSIGNED NOT NULL DEFAULT 0, "
                    "str TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "con TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "agi TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "dex TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "intel TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "wis TINYINT UNSIGNED NOT NULL DEFAULT 0, "
                    "PRIMARY KEY (name)"
                    ");");

    m_stmt->execute("CREATE TABLE IF NOT EXISTS ItemCatalog("
                    "itemID MEDIUMINT UNSIGNED NOT NULL, "
                    "article VARCHAR(3), "
                    "name VARCHAR(50) NOT NULL, "
                    "keyword VARCHAR(50) NOT NULL, "
                    "value SMALLINT UNSIGNED NOT NULL DEFAULT 0, "
                    "flags MEDIUMINT UNSIGNED NOT NULL DEFAULT 0, "
                    "PRIMARY KEY (itemID)"
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

void DBConnection::LoadItems(Mud::Logic::World &world)
{
    const std::string query = "SELECT * FROM ItemCatalog;";
    m_pstmt = m_con->prepareStatement(query);
    auto rs = m_pstmt->executeQuery();

    while (rs->next())
    {
        const uint32_t itemID = rs->getInt64("itemID");
        const std::string article = rs->getString("article");
        const std::string name = rs->getString("name");
        const std::string keyword = rs->getString("keyword");
        const uint16_t value = rs->getInt("value");
        const uint16_t flags = rs->getInt("flags");

        world.GenerateItem(itemID, article, name, keyword, value, flags);
    }

    delete rs;
    delete m_pstmt;
}

void DBConnection::SaveItems(Logic::World &world)
{
    std::cout << "Saving Items to database..." << std::endl;

    m_pstmt = m_con->prepareStatement("INSERT INTO ItemCatalog VALUES (?, ?, ?, ?, ?, ?) "
                                      "AS new ON DUPLICATE KEY UPDATE itemID=new.itemID, "
                                      "article=new.article, name=new.name, keyword=new.keyword, "
                                      "value=new.value, flags=new.flags;");

    auto itemCatalog = world.Items();
    uint32_t itemID;
    std::string article, name, keyword;
    uint16_t value, flags;

    for (auto const &r : itemCatalog)
    {
        auto &item = r.second;
        itemID = item->ItemID();
        article = item->Article();
        name = item->Name();
        keyword = item->Keyword();
        value = item->Value();
        flags = item->Flags();

        m_pstmt->setInt64(1, itemID);
        m_pstmt->setString(2, article);
        m_pstmt->setString(3, name);
        m_pstmt->setString(4, keyword);
        m_pstmt->setInt(5, value);
        m_pstmt->setInt(6, flags);

        m_pstmt->execute();
    }

    delete m_pstmt;
    std::cout << "Items saved." << std::endl;
}

void DBConnection::SaveItem(Logic::World &world, uint32_t itemID)
{
    std::cout << "Saving Items to database..." << std::endl;

    m_pstmt = m_con->prepareStatement("INSERT INTO ItemCatalog VALUES (?, ?, ?, ?, ?, ?) "
                                      "AS new ON DUPLICATE KEY UPDATE itemID=new.itemID, "
                                      "article=new.article, name=new.name, keyword=new.keyword, "
                                      "value=new.value, flags=new.flags;");

    auto item = world.FindItem(itemID);

    if (!item)
        return;

    m_pstmt->setInt64(1, item->ItemID());
    m_pstmt->setString(2, item->Article());
    m_pstmt->setString(3, item->Name());
    m_pstmt->setString(4, item->Keyword());
    m_pstmt->setInt(5, item->Value());
    m_pstmt->setInt(6, item->Flags());

    m_pstmt->execute();

    delete m_pstmt;
    std::cout << "Item " << item->DisplayName() << "(#"<< itemID << ") saved to database." << std::endl;
}

void DBConnection::LoadMonsters(Mud::Logic::World &world)
{
    const std::string query = "SELECT * FROM MonsterCatalog;";
    m_pstmt = m_con->prepareStatement(query);
    auto rs = m_pstmt->executeQuery();

    while (rs->next())
    {
        const uint32_t monsterID = rs->getInt64("monsterID");
        const std::string article = rs->getString("article");
        const std::string name = rs->getString("name");
        const std::string keyword = rs->getString("keyword");
        const uint8_t realm = rs->getInt("realm");
        const uint32_t exp = rs->getInt("exp");
        const uint8_t level = rs->getInt("level");
        const uint8_t hits = rs->getInt("hits");
        const uint8_t fat = rs->getInt("fat");
        const uint8_t power = rs->getInt("power");
        const uint8_t str = rs->getInt("str");
        const uint8_t con = rs->getInt("con");
        const uint8_t agi = rs->getInt("agi");
        const uint8_t dex = rs->getInt("dex");
        const uint8_t intel = rs->getInt("intel");
        const uint8_t wis = rs->getInt("wis");

        world.GenerateMonster(monsterID, article, name, keyword, exp, level, hits, fat, power,
                              str, con, agi, dex, intel, wis, (Logic::Realm) realm);
    }

    delete rs;
    delete m_pstmt;
}

void DBConnection::SaveMonsters(Logic::World &world)
{
    std::cout << "Saving Monsters to database..." << std::endl;

    m_pstmt = m_con->prepareStatement("INSERT INTO MonsterCatalog VALUES "
                                      "(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) "
                                      "AS new ON DUPLICATE KEY UPDATE monsterID=new.monsterID, "
                                      "article=new.article, name=new.name, keyword=new.keyword, realm=new.realm, "
                                      "exp=new.exp, level=new.level, hits=new.hits, fat=new.fat, "
                                      "power=new.power, str=new.str, con=new.con, agi=new.agi, "
                                      "dex=new.dex, intel=new.intel, wis=new.wis;");

    auto monsterCatalog = world.Monsters();

    for (auto const &r : monsterCatalog)
    {
        auto &monster = r.second;

        m_pstmt->setInt64(1, monster->MonsterID());
        m_pstmt->setString(2, monster->Article());
        m_pstmt->setString(3, monster->Name());
        m_pstmt->setString(4, monster->Keyword());
        m_pstmt->setInt(5, static_cast<uint8_t>(monster->GetRealm()));
        m_pstmt->setInt64(6, monster->Experience());
        m_pstmt->setInt(7, monster->Level());
        m_pstmt->setInt(8, monster->MaxState().Health());
        m_pstmt->setInt(9, monster->MaxState().Fatigue());
        m_pstmt->setInt(10, monster->MaxState().Power());
        m_pstmt->setInt(11, monster->BaseStats().GetStat(Logic::MobStats::Stat::STRENGTH));
        m_pstmt->setInt(12, monster->BaseStats().GetStat(Logic::MobStats::Stat::CONSTITUTION));
        m_pstmt->setInt(13, monster->BaseStats().GetStat(Logic::MobStats::Stat::AGILITY));
        m_pstmt->setInt(14, monster->BaseStats().GetStat(Logic::MobStats::Stat::DEXTERITY));
        m_pstmt->setInt(15, monster->BaseStats().GetStat(Logic::MobStats::Stat::INTELLIGENCE));
        m_pstmt->setInt(16, monster->BaseStats().GetStat(Logic::MobStats::Stat::WISDOM));

        m_pstmt->execute();
    }

    delete m_pstmt;
    std::cout << "Monsters saved." << std::endl;
}

void DBConnection::SaveMonster(Logic::World &world, uint32_t monsterID)
{
    m_pstmt = m_con->prepareStatement("INSERT INTO MonsterCatalog VALUES "
                                      "(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?) "
                                      "AS new ON DUPLICATE KEY UPDATE monsterID=new.monsterID, "
                                      "article=new.article, name=new.name, keyword=new.keyword, realm=new.realm, "
                                      "exp=new.exp, level=new.level, hits=new.hits, fat=new.fat, "
                                      "power=new.power, str=new.str, con=new.con, agi=new.agi, "
                                      "dex=new.dex, intel=new.intel, wis=new.wis;");

    auto monster = world.FindMonster(monsterID);
    if (!monster)
        return;

    m_pstmt->setInt64(1, monster->MonsterID());
    m_pstmt->setString(2, monster->Article());
    m_pstmt->setString(3, monster->Name());
    m_pstmt->setString(4, monster->Keyword());
    m_pstmt->setInt(5, static_cast<uint8_t>(monster->GetRealm()));
    m_pstmt->setInt64(6, monster->Experience());
    m_pstmt->setInt(7, monster->Level());
    m_pstmt->setInt(8, monster->MaxState().Health());
    m_pstmt->setInt(9, monster->MaxState().Fatigue());
    m_pstmt->setInt(10, monster->MaxState().Power());
    m_pstmt->setInt(11, monster->BaseStats().GetStat(Logic::MobStats::Stat::STRENGTH));
    m_pstmt->setInt(12, monster->BaseStats().GetStat(Logic::MobStats::Stat::CONSTITUTION));
    m_pstmt->setInt(13, monster->BaseStats().GetStat(Logic::MobStats::Stat::AGILITY));
    m_pstmt->setInt(14, monster->BaseStats().GetStat(Logic::MobStats::Stat::DEXTERITY));
    m_pstmt->setInt(15, monster->BaseStats().GetStat(Logic::MobStats::Stat::INTELLIGENCE));
    m_pstmt->setInt(16, monster->BaseStats().GetStat(Logic::MobStats::Stat::WISDOM));

    m_pstmt->execute();

    delete m_pstmt;
    std::cout << "Monster " << monster->DisplayName() << "(#"<< monsterID << ") saved to database." << std::endl;
}
