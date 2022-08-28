#include <iostream>
#include "Server/Text.hpp"
#include "World/World.hpp"
#include "World/Area.hpp"
#include "World/Room.hpp"
#include "Logic/Monster.hpp"
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
int Mud::Logic::Monster::monsterCount = 0;

std::string Mud::Logic::Exit::DirectionNames[10] =
        {
                "north",
                "south",
                "east",
                "west",
                "northeast",
                "northwest",
                "southeast",
                "southwest",
                "up",
                "down",
        };

int main()
{
    std::cout << Mud::Server::BR_GREENTEXT << MUDNAME << Mud::Server::PLAINTEXT << " version "
              << Mud::Server::YELLOWTEXT << MAJOR << "." << MINOR << "." << PATCH << Mud::Server::PLAINTEXT
              << " started.\n";

    std::cout << "\nInitializing World...\n\n";
    Mud::Logic::World world;

    std::cout << "Loading Areas...\n";

    world.GenerateArea("The Void");
    world.GenerateArea("City of Tamia");
    std::cout << Mud::Logic::Area::GetWorldCount() << " Areas Loaded.\n\n";

    std::cout << "Loading Rooms...\n";

    world.GenerateRoom("You are in the void!", 0, 0,0,0);
    uint16_t cExits1 = (1 << static_cast<int>(Mud::Logic::Direction::EAST));
    world.GenerateRoom("You are standing in the first dark room.  The walls are bare and the air is musty.",
                       1, 0, 0, 0, cExits1);

    uint16_t cExits2 = (1 << static_cast<int>(Mud::Logic::Direction::EAST));
    cExits2 |= (1 << static_cast<int>(Mud::Logic::Direction::WEST));
    world.GenerateRoom("You are standing in the second dark room.  There is a picture of a snow-covered "
                       "embankment with deer gazing at a log cabin.",
                       1, 1, 0, 0, cExits2);

    uint16_t cExits3 = (1 << static_cast<int>(Mud::Logic::Direction::WEST));
    world.GenerateRoom("You are standing in the third dark room.  There is a picture of dogs shooting pool "
                       "and smoking cigars hanging on the wall.",
                       1, 2, 0, 0, cExits3);

    std::cout << Mud::Logic::Room::GetWorldCount() << " Rooms Loaded.\n\n";

    std::cout << "Loading Exits...\n";
    // NOTE(jon): Load objects (portals)
    std::cout << "Exits Loaded.\n\n";

    std::cout << "World initialized.\n" << std::endl;

    std::cout << "Loading Monsters...\n";
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
    std::cout << Mud::Logic::Monster::GetLoadedCount() << " Monsters loaded.\n\n";

    Mud::Server::Server server(PORT, world);
    server.Run();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
