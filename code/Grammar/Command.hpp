//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_COMMAND_HPP
#define DARKFALLS_COMMAND_HPP

#include <memory>
#include "code/Server/Text.hpp"

namespace Mud
{
namespace Logic
{
    class MudInterface;
    class World;
} // Logic

namespace Grammar
{

class Command
{
public:
    virtual ~Command() {}
    virtual void Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const;
};

class HelloCommand : public Command
{
    void Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const override;
};

class HealthCommand : public Command
{
    void Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const override;
};

class LookCommand : public Command
{
    void Execute(std::shared_ptr<Logic::MudInterface> mudInterface, Logic::World &world) const override;
};

} // Grammar
} // Mud

#endif //DARKFALLS_COMMAND_HPP
