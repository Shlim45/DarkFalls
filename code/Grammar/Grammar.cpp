//
// Created by shlim on 8/16/22.
//

#include "Grammar.hpp"
#include "code/Dictionary/Tokenizer.hpp"
#include "code/Logic/MudInterface.hpp"
#include "code/Server/Text.hpp"

using namespace Mud::Grammar;

Grammar::Grammar()
{
    /* Privileged */
    m_commands["*goto"] = std::make_shared<GotoCommand>();
    m_commands["*create"] = std::make_shared<CreateCommand>();

    /* Miscellaneous */
    m_commands["account"] = std::make_shared<AccountCommand>();
    auto health = std::make_shared<HealthCommand>();
    m_commands["health"] = health;
    m_commands["hea"] = health;
    auto look = std::make_shared<LookCommand>();
    m_commands["look"] = look;
    m_commands["l"] = look;
    auto take = std::make_shared<TakeCommand>();
    m_commands["take"] = take;
    m_commands["get"] = take;
    auto drop = std::make_shared<DropCommand>();
    m_commands["drop"] = drop;
    m_commands["put"] = drop;
    auto inventory = std::make_shared<InventoryCommand>();
    m_commands["inventory"] = inventory;
    m_commands["inv"] = inventory;
    m_commands["quit"] = std::make_shared<QuitCommand>();
    m_commands["say"] = std::make_shared<SayCommand>();
    m_commands["who"] = std::make_shared<WhoCommand>();
    m_commands["info"] = std::make_shared<InfoCommand>();
    auto attack = std::make_shared<AttackCommand>();
    m_commands["attack"] = attack;
    m_commands["att"] = attack;
    m_commands["hit"] = attack;


    /* Movement */
    auto movement = std::make_shared<MoveCommand>();
    m_commands["north"] = movement;
    m_commands["n"] = movement;
    m_commands["south"] = movement;
    m_commands["s"] = movement;
    m_commands["east"] = movement;
    m_commands["e"] = movement;
    m_commands["west"] = movement;
    m_commands["w"] = movement;
    m_commands["northeast"] = movement;
    m_commands["ne"] = movement;
    m_commands["northwest"] = movement;
    m_commands["nw"] = movement;
    m_commands["southeast"] = movement;
    m_commands["se"] = movement;
    m_commands["southwest"] = movement;
    m_commands["sw"] = movement;
    m_commands["up"] = movement;
    m_commands["u"] = movement;
    m_commands["down"] = movement;
    m_commands["d"] = movement;
}

void Grammar::Parse(Dictionary::Tokenizer &tokenizer, const std::shared_ptr<Logic::MudInterface>& mudInterface,
                    Logic::World &world) const
{
    const auto &verb = tokenizer.GetString();
    const auto verbCommand = m_commands.find(verb);

    if (verbCommand == m_commands.end())
    {
        mudInterface->ostream() << "Command \'" << verb << "\' is not a valid command."
                 << Server::NEWLINE;
    }
    else
    {
        verbCommand->second->Execute(tokenizer, mudInterface, world);
    }
}
