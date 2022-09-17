//
// Created by shlim on 8/17/22.
//

#include "Command.hpp"
#include "code/Logic/MudInterface.hpp"
#include "code/Logic/Mobs/Player.hpp"
#include "code/Logic/Accounts/PlayerAccount.hpp"
#include "code/World/World.hpp"
#include "code/Dictionary/Tokenizer.hpp"
#include "code/Logic/Items/Item.hpp"

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
    auto &room = world.GetRoom(player->Location());

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
    auto gotoPlayer = world.GetPlayer(gotoWhat);
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

        auto &gotoRoom = world.GetRoom(roomID);
        if (gotoRoom)
        {
            response << "You go to Room #" << std::to_string(roomID) << "." << Server::NEWLINE;
            world.MovePlayer(player, gotoRoom);
            return;
        }
    }

    // Monster

    auto gotoMonster = world.GetMonsterLive(gotoWhat);
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

        auto &gotoRoom = world.GetRoom(roomID);
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
    auto &room = world.GetRoom(player->Location());

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
        if (world.GetArea(areaId)->FindRoomID(std::get<0>(coords), std::get<1>(coords),
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

        auto &gotoRoom = world.GetRoom(newRoomID);
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

void
StatCommand::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                     Mud::Logic::World &world) const
{
    auto &player = mudInterface->GetPlayer();
    auto &response = mudInterface->ostream();
    const std::string SPACES = "            ";
    const int MAX_STATNAME_LENGTH = 12;

    if (!player->HasSecurityFlag(Security::Flag::PLAYERS))
    {
        response << Server::NEWLINE << "You do not have permission to create ROOMS."
                 << Server::NEWLINE << Server::NEWLINE;
        return;
    }

    const auto targetName = commands.CombineRemaining();
    auto &pTarget = player;
    if (targetName.length())
    {
        auto pTargetPos = world.Players().find(targetName);
        if (pTargetPos != world.Players().end())
            pTarget = pTargetPos->second;
        else
            pTarget = nullptr;
    }

    if (pTarget)
    {
        auto curVitals = pTarget->CurState();
        auto maxVitals = pTarget->MaxState();
        response << Server::NEWLINE << "Name: " << Server::ColorizeText(pTarget->Name(), Server::BR_CYANTEXT) << Server::NEWLINE
                 << "Experience: " << pTarget->Experience() << Server::NEWLINE
                 << "Level: " << static_cast<int>(pTarget->Level()) << Server::NEWLINE
                 << Server::NEWLINE
                 << "Hits: " << Server::BR_BLUETEXT << curVitals.Health() << Server::PLAINTEXT << "/"
                 << Server::BR_BLUETEXT << maxVitals.Health() << Server::PLAINTEXT
                 << "  Fat: " << Server::BR_GREENTEXT << curVitals.Fatigue()<< Server::PLAINTEXT << "/"
                 << Server::BR_GREENTEXT << maxVitals.Fatigue() << Server::PLAINTEXT
                 << "  Power: " << Server::BR_REDTEXT << curVitals.Power()<< Server::PLAINTEXT << "/"
                 << Server::BR_REDTEXT << maxVitals.Power() << Server::PLAINTEXT << Server::NEWLINE

                 << Server::NEWLINE << "Statistics:" << Server::NEWLINE;

        for (uint8_t i = 0; i < Mud::Logic::MobStats::NUM_STATS; i++)
        {
            int statValue = pTarget->BaseStats().GetStat(i);
            const std::string& STAT = Mud::Logic::MobStats::StatNames.at(i);
            const auto padding = SPACES.substr(0, MAX_STATNAME_LENGTH - STAT.length());
            response << padding << Server::YELLOWTEXT << STAT << Server::PLAINTEXT << ": "
                     << statValue << Server::NEWLINE;
        }

        response << Server::NEWLINE;

        for (uint8_t i = 0; i < Mud::Logic::CombatStats::NUM_STATS; i++)
        {
            int statValue = pTarget->CombStats().GetStat(i);
            response << Server::YELLOWTEXT << Mud::Logic::CombatStats::StatNames.at(i) << Server::PLAINTEXT << ": "
                     << statValue << Server::NEWLINE;
        }

        return;
    }

    auto mTarget = world.GetMonsterLive(targetName);
    if (mTarget)
    {
        auto curVitals = mTarget->CurState();
        auto maxVitals = mTarget->MaxState();
        response << Server::NEWLINE
                 << "Name:       " << mTarget->Name() << Server::NEWLINE
                 << "Experience: " << mTarget->Experience() << Server::NEWLINE
                 << "Level:      " << static_cast<int>(mTarget->Level()) << Server::NEWLINE
                 << Server::NEWLINE
                 << "Hits: " << Server::BR_BLUETEXT << curVitals.Health() << Server::PLAINTEXT << "/"
                 << Server::BR_BLUETEXT << maxVitals.Health() << Server::PLAINTEXT
                 << "  Fat: " << Server::BR_GREENTEXT << curVitals.Fatigue()<< Server::PLAINTEXT << "/"
                 << Server::BR_GREENTEXT << maxVitals.Fatigue() << Server::PLAINTEXT
                 << "  Power: " << Server::BR_REDTEXT << curVitals.Power()<< Server::PLAINTEXT << "/"
                 << Server::BR_REDTEXT << maxVitals.Power() << Server::PLAINTEXT << Server::NEWLINE

                 << Server::NEWLINE << "Statistics:" << Server::NEWLINE;

        for (uint8_t i = 0; i < Mud::Logic::MobStats::NUM_STATS; i++)
        {
            int statValue = mTarget->BaseStats().GetStat(i);
            const std::string& STAT = Mud::Logic::MobStats::StatNames.at(i);
            const auto padding = SPACES.substr(0, MAX_STATNAME_LENGTH - STAT.length());
            response << padding << Server::YELLOWTEXT << STAT << Server::PLAINTEXT << ": "
                     << statValue << Server::NEWLINE;
        }

        response << Server::NEWLINE;

        for (uint8_t i = 0; i < Mud::Logic::CombatStats::NUM_STATS; i++)
        {
            int statValue = mTarget->CombStats().GetStat(i);
            response << Server::YELLOWTEXT << Mud::Logic::CombatStats::StatNames.at(i) << Server::PLAINTEXT << ": "
                     << statValue << Server::NEWLINE;
        }

        return;
    }

    response << Server::NEWLINE << "Target '" << targetName << "' not found."
             << Server::NEWLINE << Server::NEWLINE;
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
    auto &room = world.GetRoom(roomID);

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

void TakeCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                          Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto &player = mudInterface->GetPlayer();
    auto &room = world.GetRoom(player->Location());

    auto takeWhat = commands.GetString();
    if (takeWhat.length() == 0)
    {
        player->Tell("You must specify an item to take.");
        return;
    }

    auto toTake = room->FindItem(takeWhat);
    if (toTake)
    {
        room->RemoveItem(toTake);
        player->AddItemToInventory(toTake);
        room->Show("You pick up " + Server::ColorizeText(toTake->DisplayName(), Server::YELLOWTEXT) + ".", *player,
                   player->DisplayName() + " picks up " + Server::ColorizeText(toTake->DisplayName(), Server::YELLOWTEXT) + ".");
    }
    else
    {
        response << Server::NEWLINE << "You dont see a '" << takeWhat << "' here."
                 << Server::NEWLINE << Server::NEWLINE;
    }
}

void DropCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                          Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto player = mudInterface->GetPlayer();
    auto &room = world.GetRoom(player->Location());

    auto dropWhat = commands.GetString();
    if (dropWhat.length() == 0)
    {
        player->Tell("You must specify an item to drop.");
        return;
    }

    auto toDrop = player->FindItemInInventory(dropWhat);
    if (toDrop)
    {
        player->RemoveItemFromInventory(toDrop);
        room->AddItem(toDrop);
        room->Show("You drop " + Server::ColorizeText(toDrop->DisplayName(), Server::YELLOWTEXT) + ".", *player,
                   player->DisplayName() + " drops " + Server::ColorizeText(toDrop->DisplayName(), Server::YELLOWTEXT) + ".");
    }
    else
    {
        response << Server::NEWLINE << "You dont see a '" << dropWhat << "' here."
                 << Server::NEWLINE << Server::NEWLINE;
    }
}

void InventoryCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                          Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto player = mudInterface->GetPlayer();

    std::stringstream result;
    result << Server::NEWLINE << "You are currently carrying:" << Server::NEWLINE;
    if (player->Inventory().empty())
        result << "  nothing" << Server::NEWLINE;
    else
        for (auto &I : player->Inventory())
            result << "  " << Server::ColorizeText(I->DisplayName(), Server::YELLOWTEXT) << Server::NEWLINE;

    result << Server::NEWLINE << "You are carrying " << player->Inventory().size() << " items."
            << Server::NEWLINE << Server::NEWLINE;

    response << result.str();
}

void AttackCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                            Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto player = mudInterface->GetPlayer();
    if (!player->IsAlive())
    {
        response << Server::NEWLINE << "You must be alive to do that." << Server::NEWLINE;
        return;
    }

    auto attackWhat = commands.GetString();
    if (attackWhat.length() == 0)
    {
        response << Server::NEWLINE << "You must specify a target." << Server::NEWLINE;
        return;
    }

    auto roomID = player->Location();
    auto &room = world.GetRoom(roomID);

    auto pTarget = room->FindPlayer(attackWhat);
    auto mTarget = room->FindMonster(attackWhat);
    if (pTarget == nullptr && mTarget == nullptr)
    {
        response << Server::NEWLINE << "You don't see them here." << Server::NEWLINE;
        return;
    }
    else if (pTarget)
        world.CombatLibrary().HandleAttack(*player, *pTarget, world);
    else if (mTarget)
        world.CombatLibrary().HandleAttack(*player, *mTarget, world);
}

void QuitCommand::Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                          Logic::World &world) const
{
    auto player = mudInterface->GetPlayer();
    auto roomID = player->Location();
    auto &room = world.GetRoom(roomID);
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
    if (!player->IsAlive())
    {
        mudInterface->ostream() << Server::NEWLINE << "You must be alive to do that." << Server::NEWLINE;
        return;
    }

    auto sayWhat = commands.CombineRemaining();
    if (sayWhat.length() == 0)
    {
        player->Tell("Say what?");
        return;
    }

    auto &room = world.GetRoom(player->Location());
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
        for (auto & onlinePlayer : *onlinePlayers)
            response << "  " << onlinePlayer.second->Name() << Server::NEWLINE;
        response << Server::NEWLINE << "There are " << onlinePlayers->size()
                 << " players online in DarkFalls." << Server::NEWLINE;
    }
    else
    {
        auto onlinePlayers = &world.Players();
        for (auto & onlinePlayer : *onlinePlayers)
            if (onlinePlayer.second->Name() == whoTarget)
            {
                response << onlinePlayer.second->Name() << " is online in room " << onlinePlayer.second->Location() << "." << Server::NEWLINE;
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

    const std::string SPACES = "            ";
    const int MAX_STATNAME_LENGTH = 12;
    for (uint8_t i = 0; i < Mud::Logic::MobStats::NUM_STATS; i++)
    {
        int statValue = player->BaseStats().GetStat(i);
        const std::string& STAT = Mud::Logic::MobStats::StatNames.at(i);
        const auto padding = SPACES.substr(0, MAX_STATNAME_LENGTH - STAT.length());
        response << padding << Server::YELLOWTEXT << STAT << Server::PLAINTEXT << ": "
                 << statValue << Server::NEWLINE;
    }
}

void
MoveCommand::Execute(Mud::Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                     Mud::Logic::World &world) const
{
    auto player            = mudInterface->GetPlayer();
    if (!player->IsAlive())
    {
        mudInterface->ostream() << Server::NEWLINE << "You must be alive to do that." << Server::NEWLINE;
        return;
    }

    auto cmd = commands.GetCommand();
    boost::to_lower(cmd);

    Logic::Direction dir = Logic::CardinalExit::GetDirectionFromString(cmd);
    auto &room = world.GetRoom(player->Location());
    std::tuple<int,int,int> coords = room->Coords();

    if (!room->HasCardinalExit(dir))
    {
        player->Tell("You cannot travel in that direction.");
        return;
    }

    Logic::CardinalExit::AdjustXYZByDirection(coords, dir);

    auto area = &world.GetArea(room->AreaID());
    int newRoomID = area->get()->FindRoomID(std::get<0>(coords), std::get<1>(coords), std::get<2>(coords));

    world.WalkPlayer(player, newRoomID, dir);
}
