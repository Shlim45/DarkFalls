//
// Created by shlim on 9/3/22.
//

#ifndef DARKFALLS_SECURITY_HPP
#define DARKFALLS_SECURITY_HPP

namespace Mud
{
namespace Security
{
    static const uint32_t ALL_FLAGS = UINT16_MAX;
    enum class Flag
    {
        GOTO,
        TRANSFER,
        ROOMS,
        AREAS,
        MONSTERS,
        ITEMS,
        PLAYERS
    };

    enum class Role
    {
        NONE,
        STAFF,
        LANDS,
        MONSTERS,
        ITEMS,
        PLAYERS,
        REALM_MASTER,
        GAME_MASTER,
        DEVELOPER,
    };

    template <typename... Ts>
    static void SetFlags(uint32_t &secFlags, Ts... flagsToSet)
    {
        const int size = sizeof...(flagsToSet);
        Flag flags[size] = {static_cast<Flag>(flagsToSet)... };
        for (auto flag : flags)
            secFlags |= (1 << static_cast<uint32_t>(flag));

        // NOTE(jon): to ensure flags are set in order:
//        int dummy[sizeof...(Ts)] = { (secFlags |= flagsToSet, 0)... };
    }

    static void SetRole(uint32_t &secFlags, Role secRole)
    {
        // clear current flags
        secFlags = 0;
        switch (secRole)
        {
            case Role::NONE:
                break;
            case Role::STAFF:
                SetFlags(secFlags, Flag::GOTO, Flag::TRANSFER);
                break;
            case Role::LANDS:
                SetFlags(secFlags, Flag::GOTO, Flag::TRANSFER, Flag::AREAS, Flag::ROOMS);
                break;
            case Role::MONSTERS:
                SetFlags(secFlags, Flag::GOTO, Flag::TRANSFER, Flag::AREAS, Flag::ROOMS,
                                        Flag::MONSTERS);
                break;
            case Role::ITEMS:
                SetFlags(secFlags, Flag::GOTO, Flag::TRANSFER, Flag::AREAS, Flag::ROOMS,
                                        Flag::MONSTERS, Flag::ITEMS);
                break;
            case Role::PLAYERS:
            case Role::REALM_MASTER:
            case Role::GAME_MASTER:
                SetFlags(secFlags, Flag::GOTO, Flag::TRANSFER, Flag::AREAS, Flag::ROOMS,
                                        Flag::MONSTERS, Flag::ITEMS, Flag::PLAYERS);
                break;
            case Role::DEVELOPER:
                SetFlags(secFlags, ALL_FLAGS);
                break;
        }
    }

    static bool HasFlag(uint32_t flags, Flag flag)
    {
        return (flags & (1 << static_cast<uint32_t>(flag))) == 1;
    }

} // Security
} // Mud

#endif //DARKFALLS_SECURITY_HPP
