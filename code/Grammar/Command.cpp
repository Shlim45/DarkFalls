//
// Created by shlim on 8/17/22.
//

#include "Command.hpp"
#include "code/Logic/MudInterface.hpp"
#include "code/Logic/Mob.hpp"

using namespace Mud::Grammar;

void Command::Execute(std::shared_ptr<Logic::MudInterface> mudInterface) const
{
    auto &response = mudInterface->ostream();
    response << "Command found!" << Server::NEWLINE;
}

void HelloCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface) const
{
    auto &response = mudInterface->ostream();
    response << "Hello there, " << mudInterface->Player()->Name() << "!" << Server::NEWLINE;
}

void HealthCommand::Execute(std::shared_ptr<Logic::MudInterface> mudInterface) const
{
    auto &response = mudInterface->ostream();

    response << " Hits: " << mudInterface->Player()->Health() << "/" << mudInterface->Player()->MaxHealth() << Server::NEWLINE
             << "  Fat: " << mudInterface->Player()->Fatigue() << "/" << mudInterface->Player()->MaxFatigue() << Server::NEWLINE
             << "Power: " << mudInterface->Player()->Power() << "/" << mudInterface->Player()->MaxPower() << Server::NEWLINE;
}
