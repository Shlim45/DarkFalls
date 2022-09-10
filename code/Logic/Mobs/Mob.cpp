//
// Created by shlim on 8/24/22.
//

#include "Mob.hpp"
#include "code/Logic/Items/Item.hpp"

using namespace Mud::Logic;

uint64_t Mob::mobCount = 0;

void Mob::AddItemToInventory(const std::shared_ptr<Item>& item)
{
    if (item)
    {
        item->SetLocation(-1);
        m_inventory.insert(item);
    }
}

void Mob::RemoveItemFromInventory(const std::shared_ptr<Item> &item)
{
    m_inventory.erase(item);
}

std::shared_ptr<Item> Mob::FindItemInInventory(const std::string &name)
{
    // Full name
    for (const auto& I : m_inventory)
        if (I->Name() == name)
            return I;

    const auto len = name.length();
    for (const auto& m : m_inventory)
        if (m->Name().substr(0, len) == name)
            return m;

    // Keyword
    for (const auto& m : m_inventory)
        if (m->Keyword() == name)
            return m;

    for (const auto& m : m_inventory)
        if (m->Keyword().substr(0, len) == name)
            return m;

    return nullptr;
}
