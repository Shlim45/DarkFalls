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
    /* Miscellaneous */
    m_commands["hello"] = std::make_shared<HelloCommand>();
    m_commands["health"] = std::make_shared<HealthCommand>();
    m_commands["look"] = std::make_shared<LookCommand>();
    m_commands["quit"] = std::make_shared<QuitCommand>();

    /* Movement */
    auto north = std::make_shared<NorthCommand>();
    m_commands["north"] = north;
    m_commands["n"] = north;

    auto south = std::make_shared<SouthCommand>();
    m_commands["south"] = south;
    m_commands["s"] = south;

    auto east = std::make_shared<EastCommand>();
    m_commands["east"] = east;
    m_commands["e"] = east;

    auto west = std::make_shared<WestCommand>();
    m_commands["west"] = west;
    m_commands["w"] = west;

    auto northeast = std::make_shared<NorthEastCommand>();
    m_commands["northeast"] = northeast;
    m_commands["ne"] = northeast;

    auto northwest = std::make_shared<NorthWestCommand>();
    m_commands["northwest"] = northwest;
    m_commands["nw"] = northwest;

    auto southeast = std::make_shared<SouthEastCommand>();
    m_commands["southeast"] = southeast;
    m_commands["se"] = southeast;

    auto southwest = std::make_shared<SouthWestCommand>();
    m_commands["southwest"] = southwest;
    m_commands["sw"] = southwest;

    auto up = std::make_shared<UpCommand>();
    m_commands["up"] = up;
    m_commands["u"] = up;

    auto down = std::make_shared<DownCommand>();
    m_commands["down"] = down;
    m_commands["d"] = down;
}

void Grammar::Parse(Dictionary::Tokenizer &tokenizer, std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const
{
    const auto &verb = tokenizer.GetString();
    const auto verbCommand = m_commands.find(verb);

    auto &response = mudInterface->ostream();

    if (verbCommand == m_commands.end())
    {
        response << "Command \'" << verb << "\' is not a valid command."
                 << Server::NEWLINE;
    }
    else
    {
        verbCommand->second->Execute(mudInterface, world);
    }
}
