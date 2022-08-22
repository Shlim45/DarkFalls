#include <iostream>
#include "Server/Server.hpp"
#include "World/World.hpp"

namespace
{
    const int PORT = 4000;
    const std::string MUDNAME = "DarkFalls";
    const int MAJOR = 0;
    const int MINOR = 1;
    const int PATCH = 0;
}

int main()
{
    std::cout << Mud::Server::BR_GREENTEXT << MUDNAME << Mud::Server::PLAINTEXT << " version "
              << Mud::Server::YELLOWTEXT << MAJOR << "." << MINOR << "." << PATCH << Mud::Server::PLAINTEXT
              << " started.\n";

    std::cout << "Initializing World...\n";
    Mud::Logic::World world;

    std::cout << "Loading Areas...\n";
    Mud::Logic::Area area("Tamia City");
    std::cout << "Areas Loaded.\n";

    std::cout << "Loading Rooms...\n";
    Mud::Logic::Room room1(1, "You are standing in the first dark room.  The walls are bare and the air is musty.");
    room1.SetCoords(0,0,0);
    Mud::Logic::Room room2(2, "You are standing in the second dark room.  There is a picture of a snow-covered embankment with deer gazing at a log cabin.");
    room2.SetCoords(1,0,0);
    Mud::Logic::Room room3(3, "You are standing in the third dark room.  There is a picture of dogs shooting pool and smoking cigars hanging on the wall.");
    room3.SetCoords(2,0,0);
    std::cout << "Rooms Loaded.\n";

    std::cout << "Loading Exits...\n";
    room1.AddExit(std::make_shared<Mud::Logic::Exit>(Mud::Logic::Direction::EAST));

    room2.AddExit(std::make_shared<Mud::Logic::Exit>(Mud::Logic::Direction::EAST));
    room2.AddExit(std::make_shared<Mud::Logic::Exit>(Mud::Logic::Direction::WEST));

    room3.AddExit(std::make_shared<Mud::Logic::Exit>(Mud::Logic::Direction::WEST));
    std::cout << "Exits Loaded.\n";

    area.AddRoom(0,0,0, room1);
    area.AddRoom(1,0,0, room2);
    area.AddRoom(2,0,0, room3);

    world.AddArea(area);
    std::cout << "World initialized." << std::endl;

    Mud::Server::Server server(PORT, world);
    server.Run();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
