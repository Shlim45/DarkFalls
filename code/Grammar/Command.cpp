//
// Created by shlim on 8/17/22.
//

#include "Command.hpp"
#include "code/Logic/MudInterface.hpp"
#include "code/Logic/Player.hpp"
#include "code/World/World.hpp"
#include "code/Dictionary/Tokenizer.hpp"

using namespace Mud::Grammar;

void Command::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                      Mud::Logic::World &world) const
{

    auto &response = mudInterface->ostream();
    response << "Command found!" << Server::NEWLINE;
}

void HelloCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    response << "Hello there, " << mudInterface->GetPlayer()->Name() << "!" << Server::NEWLINE;
}

void HealthCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();

    response << " Hits: " << Server::BR_BLUETEXT << mudInterface->GetPlayer()->Health() << "/"
             << mudInterface->GetPlayer()->MaxHealth() << Server::PLAINTEXT << Server::NEWLINE
             << "  Fat: " << Server::BR_GREENTEXT << mudInterface->GetPlayer()->Fatigue() << "/"
             << mudInterface->GetPlayer()->MaxFatigue() << Server::PLAINTEXT << Server::NEWLINE
             << "Power: " << Server::BR_REDTEXT << mudInterface->GetPlayer()->Power() << "/"
             << mudInterface->GetPlayer()->MaxPower() << Server::PLAINTEXT << Server::NEWLINE;
}

void LookCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto player = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto &room = world.FindRoom(roomID);

    auto lookAtWhat = commands.CombineRemaining();
    if (lookAtWhat.length() == 0)
    {
        response << room->HandleLook(player) << Server::NEWLINE;
        return;
    }

    auto lookAt = room->FindPlayer(lookAtWhat);
    if (lookAt == nullptr)
    {
        response << "You don't see them here." << Server::NEWLINE;
        return;
    }

    response << "You see " << lookAt->Name() << "." << Server::NEWLINE
             << "They are currently holding:" << Server::NEWLINE
             << "  nothing" << Server::NEWLINE
             << "They are currently wearing:" << Server::NEWLINE
             << "  nothing" << Server::NEWLINE
             << "They are standing." << Server::NEWLINE << Server::NEWLINE;
}

void QuitCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto &room = world.FindRoom(roomID);
    if (room != nullptr)
        room->RemovePlayer(player);
    player->Tell(Server::NEWLINE + "Bye bye!" + Server::NEWLINE);
    player->Quit();
}

void NorthCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::NORTH))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::NORTH);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel north." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void SouthCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::SOUTH))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::SOUTH);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel south." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void EastCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::EAST))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::EAST);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel east." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void WestCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::WEST))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::WEST);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel west." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void NorthEastCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::NORTHEAST))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::NORTHEAST);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel northeast." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void NorthWestCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::NORTHWEST))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::NORTHWEST);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel northwest." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void SouthEastCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::SOUTHEAST))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::SOUTHEAST);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel southeast." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void SouthWestCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::SOUTHWEST))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::SOUTHWEST);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel southwest." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void UpCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::UP))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::UP);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel up." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void DownCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto room = &world.FindRoom(roomID);
    auto area               = &world.FindArea(room->get()->AreaID());
    std::tuple<int,int,int> coords = room->get()->Coords();

    if (room->get()->HasCardinalExit(Logic::Direction::DOWN))
        Logic::Exit::AdjustXYZByDirection(coords, Logic::Direction::DOWN);
    else
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));
    auto newRoom = &world.FindRoom(newRoomID);
    room->get()->RemovePlayer(player);
    newRoom->get()->AddPlayer(player);
    player->Tell("You travel down." + Server::NEWLINE + newRoom->get()->HandleLook(player));
}

void SayCommand::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                         Mud::Logic::World &world) const
{
    auto player = mudInterface->GetPlayer();
    auto sayWhat = commands.CombineRemaining();
    if (sayWhat.length() == 0)
    {
        player->Tell("Say what?");
        return;
    }

    mudInterface->ostream() << "You say, '" << sayWhat << "'" << Server::NEWLINE;
}
