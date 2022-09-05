//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_TOKENIZER_HPP
#define DARKFALLS_TOKENIZER_HPP

#include <string>
#include <vector>

namespace Mud
{
namespace Dictionary
{

class Tokenizer
{
public:
    explicit Tokenizer(const std::string &line)
    : m_cacheIterator(m_cache.end()),
      m_pos(line.begin()), m_end(line.end())
    {
        AdvanceToNextToken();
    }

    const std::string &GetString();

    std::string CombineRemaining(const std::string &delim = " ");

private:
    void AdvanceToNextToken();
    void AdvanceThroughSpaces();
    const std::string &ReadNextString();
    void IgnoreTelnetCommand();

    std::vector<std::string> m_cache;
    std::vector<std::string>::const_iterator m_cacheIterator;

    std::string::const_iterator m_pos, m_end;
};

} // Mud
} // Dictionary

#endif //DARKFALLS_TOKENIZER_HPP
