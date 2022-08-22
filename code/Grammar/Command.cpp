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
    auto roomID = player->RoomID();
    auto room = world.FindRoom(roomID);

    if (roomID == 0 || room == nullptr)
    {
        response << "[" << Server::BR_REDTEXT << "The Void" << Server::PLAINTEXT << "]"
                 << Server::NEWLINE << "You\'re in the void!" << Server::NEWLINE;
    }
    else
    {
        response << room->HandleLook(player) << Server::NEWLINE;
    }

}

void QuitCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player = mudInterface->GetPlayer();
    auto room = world.FindRoom(player->RoomID());
    if (room != nullptr)
        room->RemovePlayer(player);
    player->Tell(Server::NEWLINE + "Bye bye!" + Server::NEWLINE);
    player->Quit();
}

void NorthCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto room = world.FindRoom(player->RoomID());
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit->Destination() == Logic::Direction::NORTH)
        {
            // valid direction
            exit->AdjustXYZByDirection(coords, Logic::Direction::NORTH);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom->AddPlayer(player);
    player->Tell("You travel north." + Server::NEWLINE + newRoom->HandleLook(player));
}

void SouthCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto room = world.FindRoom(player->RoomID());
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit->Destination() == Logic::Direction::SOUTH)
        {
            // valid direction
            exit->AdjustXYZByDirection(coords, Logic::Direction::SOUTH);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom->AddPlayer(player);
    player->Tell("You travel south." + Server::NEWLINE + newRoom->HandleLook(player));
}

void EastCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto room = world.FindRoom(player->RoomID());
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit->Destination() == Logic::Direction::EAST)
        {
            // valid direction
            exit->AdjustXYZByDirection(coords, Logic::Direction::EAST);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom->AddPlayer(player);
    player->Tell("You travel east." + Server::NEWLINE + newRoom->HandleLook(player));
}

void WestCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto room = world.FindRoom(player->RoomID());
    auto exits          = room->Exits();
    auto area               = world.FindArea(room->AreaName());
    std::tuple<int,int,int> coords = room->Coords();

    for (auto &exit : exits)
    {
        if (exit->Destination() == Logic::Direction::WEST)
        {
            // valid direction
            exit->AdjustXYZByDirection(coords, Logic::Direction::WEST);
            break;
        }
    }

    auto newRoom = area.FindRoom(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    room->RemovePlayer(player);
    newRoom->AddPlayer(player);
    player->Tell("You travel west." + Server::NEWLINE + newRoom->HandleLook(player));
}