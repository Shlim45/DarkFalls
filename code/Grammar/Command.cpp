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

    response << " Hits: " << mudInterface->GetPlayer()->Health() << "/"
             << mudInterface->GetPlayer()->MaxHealth() << Server::NEWLINE
             << "  Fat: " << mudInterface->GetPlayer()->Fatigue() << "/"
             << mudInterface->GetPlayer()->MaxFatigue() << Server::NEWLINE
             << "Power: " << mudInterface->GetPlayer()->Power() << "/"
             << mudInterface->GetPlayer()->MaxPower() << Server::NEWLINE;
}

void LookCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();

    response << "[" << Server::BR_REDTEXT << "City of Tamia" << Server::WHITETEXT << "]" << Server::NEWLINE
             << world.FindRoom(1).RoomDescription() << Server::NEWLINE;
}