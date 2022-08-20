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
             << mudInterface->GetPlayer()->MaxHealth() << Server::BR_WHITETEXT << Server::NEWLINE
             << "  Fat: " << Server::BR_GREENTEXT << mudInterface->GetPlayer()->Fatigue() << "/"
             << mudInterface->GetPlayer()->MaxFatigue() << Server::BR_WHITETEXT << Server::NEWLINE
             << "Power: " << Server::BR_REDTEXT << mudInterface->GetPlayer()->Power() << "/"
             << mudInterface->GetPlayer()->MaxPower() << Server::BR_WHITETEXT << Server::NEWLINE;
}

void LookCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    auto &response = mudInterface->ostream();
    auto room = mudInterface->GetPlayer()->Location();

    if (room == nullptr)
    {
        response << "[" << Server::BR_REDTEXT << "The Void" << Server::WHITETEXT << "]"
                 << Server::NEWLINE << "You\'re in the void!" << Server::NEWLINE;
    }
    else
    {
        response << "[" << Server::REDTEXT << room->AreaName() << Server::BR_WHITETEXT << "]" << Server::NEWLINE
                 << room->RoomDescription() << Server::NEWLINE;;
    }

}

void QuitCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    mudInterface->GetPlayer()->Quit();
}