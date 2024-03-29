//
// Created by shlim45 on 8/27/22.
//

#ifndef DARKFALLS_MONSTER_HPP
#define DARKFALLS_MONSTER_HPP

#include <sstream>
#include <utility>
#include "Mob.hpp"
#include "code/Server/Text.hpp"

namespace Mud
{
namespace Logic
{
class Player;

class Monster : public Mob
{
public:
    static int monsterCount;

    Monster() : Mob("generic mob"), m_monsterId(0), m_article("a"), m_keyword("mob")
    {    }

    explicit Monster(uint32_t id, std::string art, const std::string &name, std::string kw)
    : Mob(name), m_monsterId(id), m_article(std::move(art)), m_keyword(std::move(kw))
    {    }


    bool operator==(Monster &rhs) const
    {
        return rhs.ReferenceId() == m_referenceId;
    }

    bool operator==(Mob &rhs) const
    {
        return rhs.ReferenceId() == m_referenceId;
    }

    bool operator==(Player &rhs) const
    {
        return false;
    }

    bool operator!=(Monster &rhs) const
    {
        return rhs.ReferenceId() != m_referenceId;
    }

    bool operator!=(Mob &rhs) const
    {
        // TODO(jon): Add MobID to MOB class
        return rhs.ReferenceId() != m_referenceId;
    }

    bool operator!=(Player &rhs) const
    {
        return true;
    }

    [[nodiscard]] uint32_t MonsterID() const { return m_monsterId; }

    [[nodiscard]] std::string Article() const override
    {
        return m_article;
    }

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
            std::stringstream expMessage;
            expMessage << "You gain " << Server::YELLOWTEXT << exp << Server::PLAINTEXT
                       << " experience points!" << Server::NEWLINE;
            killer.Tell(expMessage.str());
        }

        // TODO(jon): needs to accept &world to destroy the monster.
    }

public:

    std::shared_ptr<Monster> CopyOf()
    {
        auto monster = std::make_shared<Monster>(m_monsterId, m_article, m_name, m_keyword);
        monster->MaxState().SetHealth(MaxState().Health());
        monster->MaxState().SetFatigue(MaxState().Fatigue());
        monster->MaxState().SetPower(MaxState().Power());
        monster->CurState().RecoverMobState(monster->MaxState());
        for (int i = 0; i < MobStats::NUM_STATS; i++)
            monster->BaseStats().SetStat(i, BaseStats().GetStat(i));
        monster->SetExperience(m_experience);
        monster->SetLevel(m_level);
        monster->SetRealm(m_realm);
        return monster;
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
