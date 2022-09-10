//
// Created by shlim on 9/7/22.
//

#ifndef DARKFALLS_ITEM_HPP
#define DARKFALLS_ITEM_HPP

#include "code/Logic/includes.hpp"

namespace Mud
{
namespace Logic
{
class Mob;

class Item
{
public:
    static int itemCount;
    Item() = default;
    ~Item() = default;
    explicit Item(uint32_t itemId) : m_itemId(itemId)
    {
        m_article = "a";
        m_name = "generic item";
        m_keyword = "item";
    }

    explicit Item(uint32_t itemId, std::string art, std::string name, std::string key)
            : m_itemId(itemId), m_article(std::move(art)), m_name(std::move(name)), m_keyword(std::move(key))
    {
    }

    explicit Item(uint32_t itemId, std::string art, std::string name, std::string key, uint16_t value, uint16_t flags)
            : m_itemId(itemId),
            m_article(std::move(art)), m_name(std::move(name)), m_keyword(std::move(key)),
            m_value(value), m_flags(flags)
    {
    }


    bool operator==(Item &rhs) const
    {
        return rhs.ItemID() == ItemID();
    }

    bool operator!=(Item &rhs) const
    {
        return rhs.ItemID() != ItemID();
    }

    [[nodiscard]] uint32_t ItemID() const { return m_itemId; }

    [[nodiscard]] std::string Article() const
    {
        return m_article;
    }
    void SetArticle(std::string &art) { m_article = std::move(art); }

    [[nodiscard]] std::string Keyword() const
    {
        return m_keyword;
    }
    void SetKeyword(std::string &key) { m_keyword = std::move(key); }

    [[nodiscard]] std::string Name() const
    {
        return m_name;
    }
    void SetName(std::string &name) { m_name = std::move(name); }

    [[nodiscard]] std::string DisplayName() const
    {
        std::stringstream result;
        if (m_article.length())
            result << m_article << " ";
        result << m_name;
        return result.str();
    }

    [[nodiscard]] uint16_t Value() const { return m_value; }
    void SetValue(uint16_t value)        { m_value = value; }

    [[nodiscard]] uint16_t Flags() const        { return m_flags; }
    void SetFlags(uint16_t flags) { m_flags = flags; }
    void AddFlag(uint16_t flag)   { m_flags |= flag; }

    [[nodiscard]] int Location() const      { return m_location; }
    void SetLocation(int loc) { m_location = loc; }

    std::shared_ptr<Mob> &Owner() { return m_owner; }
    void SetOwner(std::shared_ptr<Mob> &owner) { m_owner = owner; }
    void ClearOwner() { m_owner = nullptr; }

    std::shared_ptr<Item> CopyOf()
    {
        auto item = std::make_shared<Item>(m_itemId, m_article, m_name, m_keyword);
        item->m_flags = m_flags;
        item->m_referenceId = ++itemCount;
        item->m_value = m_value;
        return item;
    }

    [[nodiscard]] uint64_t ReferenceId() const { return m_referenceId; }
    void SetReferenceId(uint64_t refId) { m_referenceId = refId; }
    void SetReferenceId() { m_referenceId = ++itemCount; }

    static int GetWorldCount() { return itemCount; }

private:
    uint32_t m_itemId;
    std::string m_article;
    std::string m_name;
    std::string m_keyword;
    uint16_t m_value{};
    uint16_t m_flags{};
    int m_location{};
    std::shared_ptr<Mob> m_owner = nullptr;
    uint64_t m_referenceId{};
};

} // Mud
} // Logic

#endif //DARKFALLS_ITEM_HPP
