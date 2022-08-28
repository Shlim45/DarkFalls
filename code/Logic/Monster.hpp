//
// Created by shlim45 on 8/27/22.
//

#ifndef DARKFALLS_MONSTER_HPP
#define DARKFALLS_MONSTER_HPP

#include "Mob.hpp"

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
        std::cout << "[DEBUG] Monster() called." << std::endl;
        ++monsterCount;
    }
    explicit Monster(uint32_t id, std::string art, std::string name, std::string kw)
    : Mob(std::move(name)), m_monsterId(id), m_article(std::move(art)), m_keyword(std::move(kw))
    {
        ++monsterCount;
    }

    uint32_t MonsterID() { return m_monsterId; }

    std::string Keyword() const override
    {
        return m_keyword;
    }

    std::string DisplayName() const override
    {
        if (m_article.length())
            return m_article + " " + m_name;
        return m_name;
    }

    std::unique_ptr<Monster> CopyOf()
    {
        return std::make_unique<Monster>(m_monsterId, m_article, m_name, m_keyword);
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
