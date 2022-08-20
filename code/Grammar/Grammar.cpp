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
    m_commands["hello"] = std::make_shared<HelloCommand>();
    m_commands["health"] = std::make_shared<HealthCommand>();
    m_commands["look"] = std::make_shared<LookCommand>();
    m_commands["quit"] = std::make_shared<QuitCommand>();
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
