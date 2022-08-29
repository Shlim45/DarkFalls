//
// Created by shlim on 8/17/22.
//

#include "Command.hpp"
#include "code/Logic/MudInterface.hpp"
#include "code/Logic/Player.hpp"
#include "code/Logic/PlayerAccount.hpp"
#include "code/World/World.hpp"
#include "code/Dictionary/Tokenizer.hpp"
#include "code/Logic/MobStats.hpp"

using namespace Mud::Grammar;

void Command::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                      Mud::Logic::World &world) const
{

    auto &response = mudInterface->ostream();
    response << "Command found!" << Server::NEWLINE;
}

void AccountCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
//    mudInterface->ostream() << mudInterface->GetAccount() << Server::NEWLINE;
    auto &os = mudInterface->ostream();
    const auto &pa = mudInterface->GetAccount();
    os << Server::NEWLINE << "Username:    " << pa->UserName() << Server::NEWLINE
       << "Players:     " << Server::NEWLINE;
    for (const auto &p : pa->Players())
        os << " " << p->Name() << Server::NEWLINE;
    os << "Last Active: " << pa->LastDateTime() << Server::NEWLINE
       << "Last IP:     " << pa->LastIP() << Server::NEWLINE << Server::NEWLINE;
}

void HealthCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    const auto &curVitals = mudInterface->GetPlayer()->CurState();
    const auto &maxVitals = mudInterface->GetPlayer()->MaxState();

    response << Server::NEWLINE
             << " Hits: " << Server::BR_BLUETEXT << curVitals.Health()<< Server::PLAINTEXT << "/"
             << Server::BR_BLUETEXT << maxVitals.Health() << Server::PLAINTEXT << Server::NEWLINE
             << "  Fat: " << Server::BR_GREENTEXT << curVitals.Fatigue()<< Server::PLAINTEXT << "/"
             << Server::BR_GREENTEXT << maxVitals.Fatigue() << Server::PLAINTEXT << Server::NEWLINE
             << "Power: " << Server::BR_REDTEXT << curVitals.Power()<< Server::PLAINTEXT << "/"
             << Server::BR_REDTEXT << maxVitals.Power() << Server::PLAINTEXT << Server::NEWLINE;
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
    if (lookAtWhat == "me")
        lookAt = mudInterface->GetPlayer();
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
    {
        room->Show(player->Name() + " disappears in a puff of smoke.", player);
        room->RemovePlayer(player);
    }
    player->Tell(Server::NEWLINE + "Bye bye!" + Server::NEWLINE);
    player->Quit();
    world.RemoveOnlinePlayer(player);
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
    room->get()->Show(player->Name() + " leaves north.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel north." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from the south.", player);
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
    room->get()->Show(player->Name() + " leaves south.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel south." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from the north.", player);
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
    room->get()->Show(player->Name() + " leaves east.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel east." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from the west.", player);
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
    room->get()->Show(player->Name() + " leaves west.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel west." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from the east.", player);
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
    room->get()->Show(player->Name() + " leaves northeast.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel northeast." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from the southwest.", player);
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
    room->get()->Show(player->Name() + " leaves northwest.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel northwest." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from the southeast.", player);
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
    room->get()->Show(player->Name() + " leaves southeast.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel southeast." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from the northwest.", player);
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
    room->get()->Show(player->Name() + " leaves southwest.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel southwest." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from the northeast.", player);
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
    room->get()->Show(player->Name() + " leaves up.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel up." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from below.", player);
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
    room->get()->Show(player->Name() + " leaves down.", player);

    newRoom->get()->AddPlayer(player);
    player->Tell("You travel down." + Server::NEWLINE + newRoom->get()->HandleLook(player));
    newRoom->get()->Show(player->Name() + " arrives from above.", player);
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

    auto &room = world.FindRoom(player->Location());
    mudInterface->ostream() << "You say, '" << sayWhat << "'" << Server::NEWLINE;
    room->Show(player->Name() + " says, '" + sayWhat + "'", player);
}

void WhoCommand::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                         Mud::Logic::World &world) const
{
    auto &response = mudInterface->ostream();
//    auto player = mudInterface->GetPlayer();
//    auto roomID = player->Location();
//    auto &room = world.FindRoom(roomID);

    auto whoTarget = commands.CombineRemaining();
    if (whoTarget.length() == 0 || whoTarget == "all")
    {
        auto onlinePlayers = &world.Players();
        for (auto p = onlinePlayers->begin(); p != onlinePlayers->end(); p++)
            response << "  " << p->second->Name() << Server::NEWLINE;
        response << Server::NEWLINE << "There are " << onlinePlayers->size() << " players online in DarkFalls." << Server::NEWLINE;
    }
    else
    {
        auto onlinePlayers = &world.Players();
        for (auto p = onlinePlayers->begin(); p != onlinePlayers->end(); p++)
            if (p->second->Name() == whoTarget)
            {
                response << p->second->Name() << " is online in room " << p->second->Location() << "." << Server::NEWLINE;
                return;
            }
        response << "Player not found." << Server::NEWLINE;
    }
}

void
InfoCommand::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                     Mud::Logic::World &world) const
{
    auto &player = mudInterface->GetPlayer();
    auto &response = mudInterface->ostream();
    response << Server::NEWLINE << "Name: " << player->Name() << Server::NEWLINE
             << "Experience: " << player->Experience() << Server::NEWLINE
             << "Hits: " << player->CurState().Health() << "/" << player->MaxState().Health()
             << " Fat: " << player->CurState().Fatigue() << "/" << player->MaxState().Fatigue()
             << " Power: " << player->CurState().Power() << "/" << player->MaxState().Power() << Server::NEWLINE
             << Server::NEWLINE << "Statistics:" << Server::NEWLINE;
    for (uint8_t i = 0; i < Mud::Logic::MobStats::NUM_STATS; i++)
    {
        int statValue = player->BaseStats().GetStat(i);
        response << Server::YELLOWTEXT << Mud::Logic::MobStats::StatNames.at(i) << Server::PLAINTEXT << ": "
                 << statValue << Server::NEWLINE;
    }
}
