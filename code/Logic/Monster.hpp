//
// Created by shlim45 on 8/27/22.
//

#ifndef DARKFALLS_MONSTER_HPP
#define DARKFALLS_MONSTER_HPP

#include <sstream>
#include "Mob.hpp"
#include "code/Server/Text.hpp"

namespace Mud
{
namespace Logic
{

class Monster : public Mob
{
public:
    static int monsterCount;
    Monster() : Mob("generic mob"), m_monsterId(0), m_article("a"), m_keyword("mob")
    {
//        ++monsterCount;
    }
    explicit Monster(uint32_t id, std::string art, std::string name, std::string kw)
    : Mob(std::move(name)), m_monsterId(id), m_article(std::move(art)), m_keyword(std::move(kw))
    {
        ++monsterCount;
    }

    bool operator==(Monster &rhs) const
    {
        return rhs.MonsterID() == MonsterID();
    }

    bool operator==(Mob &rhs) const
    {
        // TODO(jon): Add MobID to MOB class
        return rhs.DisplayName() == DisplayName() && rhs.Location() == Location();
    }

    bool operator==(Player &rhs) const
    {
        return false;
    }

    bool operator!=(Monster &rhs) const
    {
        return rhs.MonsterID() != MonsterID();
    }

    bool operator!=(Mob &rhs) const
    {
        // TODO(jon): Add MobID to MOB class
        return rhs.DisplayName() != DisplayName() && rhs.Location() == Location();
    }

    bool operator!=(Player &rhs) const
    {
        return true;
    }

    [[nodiscard]] uint32_t MonsterID() const { return m_monsterId; }

    [[nodiscard]] std::string Keyword() const override
    {
        return m_keyword;
    }

    [[nodiscard]] std::string DisplayName() const override
    {
        if (m_article.length())
            return m_article + " " + m_name;
        return m_name;
    }

    bool IsPlayer() override { return false; }

    void HandleDeath(Mob &killer) override
    {
        if (killer.IsPlayer())
        {
            auto lvlDiff = m_level - killer.Level();
            if (lvlDiff > 5)
                lvlDiff = 5;
            else if (lvlDiff < -5)
                lvlDiff = -5;
            auto bonus = 1.0f + ((float) lvlDiff) / 10.0f;
            const int exp = (int) ((float) m_experience * bonus);
            killer.AdjExperience(exp);
            std::stringstream rpMessage;
            rpMessage << "You gain " << Server::YELLOWTEXT << exp << Server::PLAINTEXT
                      <<" experience points!" << Server::NEWLINE << Server::NEWLINE;
            killer.Tell(rpMessage.str());
        }
    }


public:

    std::unique_ptr<Monster> CopyOf()
    {
        Monster monster;
        monster.m_monsterId = m_monsterId;
        monster.m_article = m_article;
        monster.m_name = m_name;
        monster.m_keyword = m_keyword;
        return std::make_unique<Monster>(monster);
    }

    static int GetLoadedCount() { return monsterCount; }

private:
    uint32_t m_monsterId;
    std::string m_article;
    std::string m_keyword;
};

} // Mud
} // Logic

#endif //DARKFALLS_MONSTER_HPP
