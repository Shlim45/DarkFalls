#include <iostream>
#include <thread>
#include "Server/Text.hpp"
#include "World/World.hpp"
#include "Logic/Mobs/Monster.hpp"
#include "Logic/Items/Item.hpp"
#include "Server/Server.hpp"
#include "Database/DBConnection.hpp"

namespace
{
    const int PORT = 4000;
    const std::string MUD_NAME = "DarkFalls";
    const int MAJOR = 0;
    const int MINOR = 1;
    const int PATCH = 0;
}

//void timer_start(std::function<void()> func, unsigned int interval)
//{
//    std::thread([func, interval]()
//                {
//        while(true)
//        {
//            auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
//            func();
//            std::this_thread::sleep_until(x);
//        }
//                }).detach();
//}
//
//void periodic_func()
//{
//    std::cout << "[DEBUG]: periodic_func()" << std::endl;
//}

int main()
{
    std::cout << Mud::Server::BR_GREENTEXT << MUD_NAME << Mud::Server::PLAINTEXT << " version "
              << Mud::Server::YELLOWTEXT << MAJOR << "." << MINOR << "." << PATCH << Mud::Server::PLAINTEXT
              << " started.\n";

    const char *SQL_HOSTNAME = getenv("DFSERVER");
    const char *SQL_USERNAME = getenv("DFUSER");
    const char *SQL_PASSWORD = getenv("DFPASS");
    if (!SQL_HOSTNAME || !SQL_USERNAME || !SQL_PASSWORD)
    {
        std::cerr << "[SQL_HOSTNAME]: Environment variables not found." << std::endl;
        return 1;
    }

    Mud::DB::DBConnection dbconn(SQL_HOSTNAME, SQL_USERNAME, SQL_PASSWORD);
    dbconn.InitializeDB();

    std::cout << "Initializing World...\n";
    Mud::Logic::World world;

    std::cout << "Loading Areas...\n";
    dbconn.LoadAreas(world);
    std::cout << "  " << Mud::Logic::Area::GetWorldCount() << " Areas Loaded.\n";

    std::cout << "Loading Rooms...\n";
    dbconn.LoadRooms(world);
    std::cout << "  " << Mud::Logic::Room::GetWorldCount() << " Rooms Loaded.\n";
//
//    std::cout << "Loading Exits...\n";
//    // NOTE(jon): Load objects (portals)
//    std::cout << "Exits Loaded.\n";
//
//    std::cout << "Loading Accounts...\n";
//    // NOTE(jon): Load objects (portals)
//    std::cout << "Accounts Loaded.\n";
//
//    std::cout << "Loading Players...\n";
//    // NOTE(jon): Load objects (portals)
//    std::cout << "Players Loaded.\n";

    std::cout << "World initialized.\n" << std::endl;

    std::cout << "Loading Monster templates...\n";
    world.GenerateMonster(1, "a", "traveler", "traveler");
    world.GenerateMonster(2, "a", "farmer", "farmer");
    world.GenerateMonster(3, "a", "wandering merchant", "merchant");
    world.GenerateMonster(4, "a", "small dog", "dog");

    world.FindRoom(1)->AddMonster(world.FindMonster(1)->CopyOf());

    world.FindRoom(2)->AddMonster(world.FindMonster(2)->CopyOf());
    world.FindRoom(2)->AddMonster(world.FindMonster(3)->CopyOf());

    world.FindRoom(3)->AddMonster(world.FindMonster(4)->CopyOf());
    world.FindRoom(3)->AddMonster(world.FindMonster(3)->CopyOf());
    world.FindRoom(3)->AddMonster(world.FindMonster(2)->CopyOf());
    world.FindRoom(3)->AddMonster(world.FindMonster(1)->CopyOf());
    std::cout << Mud::Logic::Monster::GetLoadedCount() << " Monster templates loaded.\n";

    std::cout << "Loading Item templates...\n";
    world.GenerateItem(1, "a", "dagger", "dagger");
    world.GenerateItem(2, "a", "sword", "sword");
    world.GenerateItem(3, "the", "wand", "wand");
    world.GenerateItem(4, "", "Bone Breaker Gauntlets", "gauntlets");

    world.FindRoom(1)->AddItem(world.FindItem(1)->CopyOf());

    world.FindRoom(2)->AddItem(world.FindItem(2)->CopyOf());
    world.FindRoom(2)->AddItem(world.FindItem(3)->CopyOf());

    world.FindRoom(3)->AddItem(world.FindItem(4)->CopyOf());
    world.FindRoom(3)->AddItem(world.FindItem(3)->CopyOf());
    world.FindRoom(3)->AddItem(world.FindItem(2)->CopyOf());
    world.FindRoom(3)->AddItem(world.FindItem(1)->CopyOf());
    std::cout << Mud::Logic::Item::GetWorldCount() << " Item templates loaded.\n";

    world.StartTicking(1000);

    std::cout << "\nInitializing Server...\n";
    Mud::Server::Server server(PORT, world);
    server.Run();

    dbconn.SaveAreas(world);
    dbconn.SaveRooms(world);
    world.Shutdown();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
