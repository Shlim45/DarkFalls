//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_COMMAND_HPP
#define DARKFALLS_COMMAND_HPP

#include <memory>

namespace Mud
{
namespace Dictionary { class Tokenizer; }
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
    virtual ~Command() = default;
    virtual void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                         Logic::World &world) const;
};

class MoveCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class GotoCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class CreateCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class AccountCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class HealthCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class LookCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class AttackCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class SayCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class WhoCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class InfoCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

class QuitCommand : public Command
{
    void Execute(Dictionary::Tokenizer &commands, const std::shared_ptr<Logic::MudInterface> &mudInterface,
                 Logic::World &world) const override;
};

} // Grammar
} // Mud

#endif //DARKFALLS_COMMAND_HPP
