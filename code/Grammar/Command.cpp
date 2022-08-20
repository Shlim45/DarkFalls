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