//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_GRAMMAR_HPP
#define DARKFALLS_GRAMMAR_HPP

#include <ostream>
#include <map>
#include <memory>
#include "Command.hpp"

namespace Mud
{
namespace Dictionary
{
    class Tokenizer;
}

namespace Grammar
{

class Grammar
{
public:
    Grammar();
    void Parse(Dictionary::Tokenizer &tokenizer, std::ostream &response) const;

private:
    std::map<std::string, std::shared_ptr<Command> > m_commands;
};

} // Grammar
} // Mud

#endif //DARKFALLS_GRAMMAR_HPP
