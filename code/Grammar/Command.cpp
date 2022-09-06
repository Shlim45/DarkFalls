//
// Created by shlim on 8/17/22.
//

#include "Command.hpp"
#include "code/Logic/MudInterface.hpp"
#include "code/Logic/Player.hpp"
#include "code/Logic/PlayerAccount.hpp"
#include "code/World/World.hpp"
#include "code/Dictionary/Tokenizer.hpp"

using namespace Mud::Grammar;

void Command::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                      Mud::Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    response << "Command found!" << Server::NEWLINE;
}

void GotoCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                          Logic::World &world) const
{
    auto player = mudInterface->GetPlayer();
    auto &response = mudInterface->ostream();
    auto &room = world.FindRoom(player->Location());

    if (!player->HasSecurityFlag(Mud::Security::Flag::GOTO))
    {
        response << Server::NEWLINE << "You do not have permission to do that."
                 << Server::NEWLINE << Server::NEWLINE;
        return;
    }

    auto gotoWhat = commands.CombineRemaining();
    if (gotoWhat.length() == 0)
    {
        response << Server::NEWLINE << "You must specify a Player, Room, or Monster."
                 << Server::NEWLINE << Server::NEWLINE;
        return;
    }

    int roomID = 0;

    // Player
    auto gotoPlayer = world.FindPlayer(gotoWhat);
    if (gotoPlayer)
        roomID = gotoPlayer->Location();

    if (roomID)
    {
        // Same room
        if (roomID == room->RoomID())
        {
            response << Server::NEWLINE << "Done." << Server::NEWLINE << Server::NEWLINE;
            return;
        }

        auto &gotoRoom = world.FindRoom(roomID);
        if (gotoRoom)
        {
            response << "You go to Room #" << std::to_string(roomID) << "." << Server::NEWLINE;
            world.MovePlayer(player, gotoRoom);
            return;
        }
    }

    // Monster

    auto gotoMonster = world.FindMonster(gotoWhat);
    if (gotoMonster)
        roomID = gotoMonster->Location();

    if (roomID)
    {
        // Same room
        if (roomID == room->RoomID())
        {
            response << Server::NEWLINE << "Done." << Server::NEWLINE << Server::NEWLINE;
            return;
        }

        auto &gotoRoom = world.FindRoom(roomID);
        if (gotoRoom->RoomID()) // prevent going to the void!
        {
            response << "You go to Room #" << std::to_string(roomID) << "." << Server::NEWLINE;
            world.MovePlayer(player, gotoRoom);
            return;
        }
    }

    // RoomID

    try
    {
        roomID = std::stoi(gotoWhat);
    }
    catch (std::invalid_argument &ignored)
    {
        response << Server::NEWLINE << "You must specify an online Player, Monster, or RoomID."
                 << Server::NEWLINE << Server::NEWLINE;
        return;
    }


    // Same room
    if (roomID == room->RoomID())
    {
        response << Server::NEWLINE << "Done." << Server::NEWLINE << Server::NEWLINE;
        return;
    }

    // Valid destination
    if (roomID >= 0)
    {
        response << "You go to Room #" << std::to_string(roomID) << "." << Server::NEWLINE;
        world.MovePlayer(player, roomID);
        return;
    }
    else
    {
        response << Server::NEWLINE << "You must specify an online Player, Monster, or RoomID."
                 << Server::NEWLINE << Server::NEWLINE;
    }
}

void CreateCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                            Logic::World &world) const
{
    auto player = mudInterface->GetPlayer();
    auto &response = mudInterface->ostream();
    auto &room = world.FindRoom(player->Location());

    if (!player->HasSecurityFlag(Mud::Security::Flag::CREATE))
    {
        response << Server::NEWLINE << "You do not have permission to do that."
                 << Server::NEWLINE << Server::NEWLINE;
        return;
    }

    auto createWhat = commands.GetString();
    if (createWhat.length() == 0)
    {
        response << Server::NEWLINE << "Create what?  You must specify AREA, ROOM, or MONSTER."
                 << Server::NEWLINE << Server::NEWLINE;
        return;
    }


    boost::to_upper(createWhat);
    if (createWhat == "ROOM")
    {
        if (!player->HasSecurityFlag(Mud::Security::Flag::ROOMS))
        {
            response << Server::NEWLINE << "You do not have permission to create ROOMS."
                     << Server::NEWLINE << Server::NEWLINE;
            return;
        }

        auto roomDirection = commands.GetString();
        boost::to_lower(roomDirection);

        Logic::Direction dir;
        bool found;
        for (int i = 0; i < Logic::CardinalExit::NUM_DIRECTIONS; i++)
        {
            if (Logic::CardinalExit::DirectionCodes[i] == roomDirection)
            {
                dir = static_cast<Logic::Direction>(i);
                found = true;
                break;
            }
        }

        if (!found)
        {
            response << Server::NEWLINE << "'" << roomDirection << "' is not a valid direction."
                     << Server::NEWLINE << Server::NEWLINE;
            return;
        }

        auto coords = room->Coords();
        Logic::CardinalExit::AdjustXYZByDirection(coords, dir);

        auto areaId = room->AreaID();
        // Check if room exists
        if (world.FindArea(areaId)->FindRoomID(std::get<0>(coords), std::get<1>(coords),
                std::get<2>(coords)) >= 0)
        {
            auto fullDirName = Logic::CardinalExit::DirectionNames[static_cast<int>(dir)];
            response << Server::NEWLINE << "A room already exists to the " << fullDirName << ".  "
                     << "Try adding a cardinal exit." << Server::NEWLINE << Server::NEWLINE;
            return;
        }

        auto newRoomID = Logic::Room::GetWorldCount();
        Logic::Direction oppositeDir = Logic::CardinalExit::GetOppositeDirection(dir);
        uint16_t cardinalExits = oppositeDir != dir
                ? (1 << static_cast<uint16_t>(oppositeDir))
                : 0;
        world.GenerateRoom(newRoomID, "You're standing in a bare room.", areaId,
                           std::get<0>(coords), std::get<1>(coords), std::get<2>(coords),
                                   cardinalExits);
        room->AddCardinalExit(dir);

        auto &gotoRoom = world.FindRoom(newRoomID);
        room->RemovePlayer(player);
        room->ShowOthers(player->Name() + " vanishes.", *player);

        gotoRoom->AddPlayer(player);
        mudInterface->ostream() << "You go to Created Room #" << newRoomID << "." << Server::NEWLINE
                                   << gotoRoom->HandleLook(player);
        gotoRoom->ShowOthers(player->Name() + " appears.", *player);
    }
    else
    {
        response << Server::NEWLINE << "You may only create ROOM for now." << Server::NEWLINE << Server::NEWLINE;
    }
}

void AccountCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                             Logic::World &world) const
{
    auto &os = mudInterface->ostream();
    const auto &pa = mudInterface->GetAccount();
    os << Server::NEWLINE << "Username:    " << pa->UserName() << Server::NEWLINE
       << "Players:     " << Server::NEWLINE;
    for (const auto &p : pa->Players())
        os << " " << p->Name() << Server::NEWLINE;
    os << "Last Active: " << pa->LastDateTime() << Server::NEWLINE
       << "Last IP:     " << pa->LastIP() << Server::NEWLINE << Server::NEWLINE;
}

void HealthCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                            Logic::World &world) const
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

void LookCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                          Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto player = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto &room = world.FindRoom(roomID);

    auto lookAtWhat = commands.GetString();
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

void AttackCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                            Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto player = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto &room = world.FindRoom(roomID);

    auto attackWhat = commands.GetString();
    if (attackWhat.length() == 0)
    {
        response << Server::NEWLINE << "You must specify a target." << Server::NEWLINE;
        return;
    }

    auto pTarget = room->FindPlayer(attackWhat);
    auto mTarget = room->FindMonster(attackWhat);
    if (pTarget == nullptr && mTarget == nullptr)
    {
        response << Server::NEWLINE << "You don't see them here." << Server::NEWLINE;
        return;
    }
    else if (pTarget)
        world.CombatLibrary().HandleAttackPP(*player, *pTarget, world);
    else if (mTarget)
        world.CombatLibrary().HandleAttackPM(*player, *mTarget, world);
}

void QuitCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                          Logic::World &world) const
{
    auto player = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto &room = world.FindRoom(roomID);
    if (room != nullptr)
    {
        room->ShowOthers(player->Name() + " disappears in a puff of smoke.", *player);
        room->RemovePlayer(player);
    }
    mudInterface->ostream() << Server::NEWLINE << "Bye bye!" << Server::NEWLINE;
    player->Quit();
    world.RemoveOnlinePlayer(player);
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
    std::stringstream result;
    room->Show("%s say, '" + sayWhat + "'", *player,
               "%s says, '" + sayWhat + "'");
}

void WhoCommand::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                         Mud::Logic::World &world) const
{
    auto &response = mudInterface->ostream();

    auto whoTarget = commands.GetString();
    if (whoTarget.length() == 0 || whoTarget == "all")
    {
        auto onlinePlayers = &world.Players();
        for (auto p = onlinePlayers->begin(); p != onlinePlayers->end(); p++)
            response << "  " << p->second->Name() << Server::NEWLINE;
        response << Server::NEWLINE << "There are " << onlinePlayers->size()
                 << " players online in DarkFalls." << Server::NEWLINE;
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

void
MoveCommand::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                     Mud::Logic::World &world) const
{
    auto cmd = commands.GetCommand();
    boost::to_lower(cmd);

    Logic::Direction dir = Logic::CardinalExit::GetDirectionFromString(cmd);

    auto player            = mudInterface->GetPlayer();
    auto &room = world.FindRoom(player->Location());
    std::tuple<int,int,int> coords = room->Coords();

    if (!room->HasCardinalExit(dir))
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    Logic::CardinalExit::AdjustXYZByDirection(coords, dir);

    auto area = &world.FindArea(room->AreaID());
    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));

    world.WalkPlayer(player, newRoomID, dir);
}
