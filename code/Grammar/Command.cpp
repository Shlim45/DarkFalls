//
// Created by shlim on 8/17/22.
//

#include "Command.hpp"
#include "code/Logic/MudInterface.hpp"
#include "code/Logic/Player.hpp"
#include "code/World/World.hpp"

using namespace Mud::Grammar;

void Command::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    response << "Command found!" << Server::NEWLINE;
}

void HelloCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    response << "Hello there, " << mudInterface->GetPlayer()->Name() << "!" << Server::NEWLINE;
}

void HealthCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();

    response << " Hits: " << Server::BR_BLUETEXT << mudInterface->GetPlayer()->Health() << "/"
             << mudInterface->GetPlayer()->MaxHealth() << Server::PLAINTEXT << Server::NEWLINE
             << "  Fat: " << Server::BR_GREENTEXT << mudInterface->GetPlayer()->Fatigue() << "/"
             << mudInterface->GetPlayer()->MaxFatigue() << Server::PLAINTEXT << Server::NEWLINE
             << "Power: " << Server::BR_REDTEXT << mudInterface->GetPlayer()->Power() << "/"
             << mudInterface->GetPlayer()->MaxPower() << Server::PLAINTEXT << Server::NEWLINE;
}

void LookCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto player = mudInterface->GetPlayer();
    auto room = player->Location();

    if (room == nullptr)
    {
        response << "[" << Server::BR_REDTEXT << "The Void" << Server::PLAINTEXT << "]"
                 << Server::NEWLINE << "You\'re in the void!" << Server::NEWLINE;
    }
    else
    {
        response << room->HandleLook(*player) << Server::NEWLINE;
    }

}

void QuitCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player = mudInterface->GetPlayer();
    auto room = player->Location();
    if (room != nullptr)
        room->RemovePlayer(*player);
    player->Tell("Bye bye!");
    player->Quit();
}

void NorthCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::NORTH)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::NORTH);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel north.");
    newRoom.HandleLook(player);
}

void SouthCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::SOUTH)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::SOUTH);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel south.");
    newRoom.HandleLook(player);
}

void EastCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::EAST)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::EAST);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel east.");
    newRoom.HandleLook(player);
}

void WestCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::WEST)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::WEST);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel west.");
    newRoom.HandleLook(player);
}

void NorthEastCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::NORTHEAST)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::NORTHEAST);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel northeast.");
    newRoom.HandleLook(player);
}

void NorthWestCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::NORTHWEST)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::NORTHWEST);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel northwest.");
    newRoom.HandleLook(player);
}

void SouthEastCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::SOUTHEAST)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::SOUTHEAST);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel southeast.");
    newRoom.HandleLook(player);
}

void SouthWestCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::SOUTHWEST)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::SOUTHWEST);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel southwest.");
    newRoom.HandleLook(player);
}

void UpCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::UP)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::UP);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel up.");
    newRoom.HandleLook(player);
}

void DownCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = *mudInterface->GetPlayer();
    auto room             = player.Location();
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit.Destination() == Logic::Direction::DOWN)
        {
            // valid direction
            exit.AdjustXYZByDirection(coords, Logic::Direction::DOWN);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom.AddPlayer(player);
    player.Tell("You travel down.");
    newRoom.HandleLook(player);
}