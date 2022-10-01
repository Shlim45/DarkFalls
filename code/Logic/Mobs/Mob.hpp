//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_MOB_HPP
#define DARKFALLS_MOB_HPP

#include "code/Logic/includes.hpp"
#include "code/Logic/MobState.hpp"
#include "code/Logic/MobStats.hpp"
#include "code/Logic/CombatStats.hpp"

namespace Mud
{
namespace Security
{
    enum class Flag;
    enum class Role;
}
namespace Logic
{
    class Room;
    class Item;

    enum class Realm;

    class Mob
    {
    public:
        static uint64_t mobCount; //

        explicit Mob(std::string name)
                : m_name(std::move(name)), m_level(1),
                  m_curState(MobState()), m_maxState(MobState()),
                  m_baseStats(MobStats()), m_curStats(MobStats())
        {
//            m_referenceId = ++mobCount;
        }

        Mob() = default;

        virtual ~Mob() = default;

        virtual bool operator==(Mob &rhs)
        {
            return rhs.m_referenceId == m_referenceId;
        }

        virtual bool operator!=(Mob &rhs)
        {
            return rhs.Name() != Name();
        }

        [[nodiscard]] std::string Name() const
        { return m_name; }

        void SetName(std::string name)
        { m_name = std::move(name); }

        [[nodiscard]] virtual std::string Article() const
        { return ""; };

        [[nodiscard]] virtual std::string Keyword() const
        { return m_name; };

        [[nodiscard]] virtual std::string DisplayName() const
        { return m_name; }

        [[nodiscard]] int Location() const
        { return m_location; }

        void SetLocation(int newLocation)
        { m_location = newLocation; }

        [[nodiscard]] uint32_t Experience() const
        { return m_experience; }

        void SetExperience(uint32_t exp)
        { m_experience = exp; };

        [[nodiscard]] uint8_t Level() const
        { return m_level; }

        void SetLevel(uint8_t level)
        { m_level = level; };

        MobState &CurState()
        { return m_curState; }

        MobState &MaxState()
        { return m_maxState; }

        MobStats &BaseStats()
        { return m_baseStats; }

        MobStats &CurStats()
        { return m_curStats; }

        CombatStats &CombStats()
        { return m_combatStats; }

        [[nodiscard]] Realm GetRealm() const
        { return m_realm; }

        virtual void SetRealm(Realm realm)
        { m_realm = realm; }

        uint8_t RaceID() const
        { return m_raceId; }

        virtual void SetRaceID(uint8_t raceId)
        { m_raceId = raceId; }

        virtual void Tell(const std::string &message)
        {}

        virtual void Quit()
        {}

        virtual void AdjExperience(int howMuch)
        {}

        virtual void SetSecurityRole(Security::Role role)
        {}

        [[nodiscard]] virtual bool HasSecurityFlag(Security::Flag toCheck) const
        { return false; }

        virtual bool IsPlayer()
        { return false; }

        virtual void HandleDeath(Mob &killer)
        { }

        bool IsAlive() { return m_curState.GetLifeState() == MobLifeState::ALIVE; }

        std::set<std::shared_ptr<Item>> Inventory() { return m_inventory; }
        void AddItemToInventory(const std::shared_ptr<Item>& item);
        void RemoveItemFromInventory(const std::shared_ptr<Item>& item);
        std::shared_ptr<Item> FindItemInInventory(const std::string &name);

        [[nodiscard]] uint64_t ReferenceId() const { return m_referenceId; }
        void SetReferenceId(uint64_t refId) { m_referenceId = refId; }
        void SetReferenceId() { m_referenceId = ++mobCount; }

    protected:
        std::string m_name;
        int m_location{};
        uint32_t m_experience{};
        uint8_t m_level{};

        Realm m_realm{};
        uint8_t m_raceId{};

        MobState m_curState;
        MobState m_maxState;

        MobStats m_baseStats{};
        MobStats m_curStats{};

        CombatStats m_combatStats{};

        std::set<std::shared_ptr<Item>> m_inventory;

        uint64_t m_referenceId{};
    };

} // Logic
} // Mud

#endif //DARKFALLS_MOB_HPP
