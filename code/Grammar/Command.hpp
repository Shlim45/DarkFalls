//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_COMMAND_HPP
#define DARKFALLS_COMMAND_HPP

#include <ostream>
#include "code/Server/Text.hpp"

namespace Mud
{
namespace Grammar
{

class Command
{
public:
    virtual ~Command() {}
    virtual void Execute(std::ostream &response) const
    {
        response << "Command found!" << Server::NEWLINE;
    }

};

class HelloCommand : public Command
{
    void Execute(std::ostream &response) const override
    {
        response << "Hello there!" << Server::NEWLINE;
    }
};

} // Grammar
} // Mud

#endif //DARKFALLS_COMMAND_HPP
