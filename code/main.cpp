#include <iostream>
#include "Server/Text.hpp"
#include "World/World.hpp"
#include "World/Area.hpp"
#include "World/Room.hpp"
#include "Server/Server.hpp"

namespace
{
    const int PORT = 4000;
    const std::string MUDNAME = "DarkFalls";
    const int MAJOR = 0;
    const int MINOR = 1;
    const int PATCH = 0;
}

int Mud::Logic::Area::areaCount = 0;
int Mud::Logic::Room::roomCount = 0;

int main()
{
    std::cout << Mud::Server::BR_GREENTEXT << MUDNAME << Mud::Server::PLAINTEXT << " version "
              << Mud::Server::YELLOWTEXT << MAJOR << "." << MINOR << "." << PATCH << Mud::Server::PLAINTEXT
              << " started.\n";

    std::cout << "Initializing World...\n";
    Mud::Logic::World world;

    std::cout << "Loading Areas...\n";

//    Mud::Logic::Area area("Tamia City");
    world.GenerateArea("The Void");
    world.GenerateArea("City of Tamia");
    std::cout << "Areas Loaded.\n";

    std::cout << "Loading Rooms...\n";
    world.GenerateRoom("You are in the void!", 0, 0,0,0);
    world.GenerateRoom("You are standing in the first dark room.  The walls are bare and the air is musty.",
                       1, 0, 0, 0);
    world.GenerateRoom("You are standing in the second dark room.  There is a picture of a snow-covered "
                       "embankment with deer gazing at a log cabin.",
                       1, 1, 0, 0);
    world.GenerateRoom("You are standing in the third dark room.  There is a picture of dogs shooting pool "
                       "and smoking cigars hanging on the wall.",
                       1, 2, 0, 0);
    std::cout << "Rooms Loaded.\n";

    std::cout << "Loading Exits...\n";
//    room1.AddExit(std::make_shared<Mud::Logic::Exit>(Mud::Logic::Direction::EAST));
//
//    room2.AddExit(std::make_shared<Mud::Logic::Exit>(Mud::Logic::Direction::EAST));
//    room2.AddExit(std::make_shared<Mud::Logic::Exit>(Mud::Logic::Direction::WEST));
//
//    room3.AddExit(std::make_shared<Mud::Logic::Exit>(Mud::Logic::Direction::WEST));
    std::cout << "Exits Loaded.\n";

    std::cout << "World initialized." << std::endl;

    Mud::Server::Server server(PORT, world);
    server.Run();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
