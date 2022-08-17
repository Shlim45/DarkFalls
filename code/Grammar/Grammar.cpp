//
// Created by shlim on 8/16/22.
//

#include "Grammar.hpp"
#include "code/Dictionary/Tokenizer.hpp"

using namespace Mud::Grammar;

Grammar::Grammar()
{
    m_commands["hello"] = std::make_shared<HelloCommand>();
}

void Grammar::Parse(Dictionary::Tokenizer &tokenizer, std::ostream &response) const
{
    const auto &verb = tokenizer.GetString();
    const auto verbCommand = m_commands.find(verb);

    if (verbCommand == m_commands.end())
    {
        response << "Command \'" << verb << "\' is not a valid command."
                 << Server::NEWLINE;
    }
    else
    {
        verbCommand->second->Execute(response);
    }
}
