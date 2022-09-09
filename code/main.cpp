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

uint64_t Mud::Logic::Mob::mobCount = 0;

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

    Mud::DB::DBConnection dbConnection(SQL_HOSTNAME, SQL_USERNAME, SQL_PASSWORD);
    dbConnection.InitializeDB();

    Mud::Logic::World world(dbConnection);
    world.LoadWorld();

    std::cout << "\nInitializing Server...\n";
    Mud::Server::Server server(PORT, world);
    server.Run();

    std::cout << "\nShutdown sequence initiated!" << std::endl;

    world.Shutdown();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
