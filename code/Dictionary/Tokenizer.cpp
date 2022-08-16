//
// Created by shlim on 8/16/22.
//

#include "Tokenizer.hpp"

using namespace Mud::Dictionary;

namespace
{
    const std::string NO_STRING;
    const char ASCII_DEL = 0x7F;
}

const std::string &Tokenizer::GetString()
{
    if (m_cacheIterator != m_cache.end())
    {
        return *(m_cacheIterator++);
    }
    else if (m_pos == m_end)
    {
        return NO_STRING;
    }
    else
    {
        return ReadNextString();
    }
}

const std::string &Tokenizer::ReadNextString()
{
    auto &result = m_cache.emplace_back();
    m_cacheIterator = m_cache.end();

    for (; m_pos != m_end; ++m_pos)
    {
        if (*m_pos <= ' ' || *m_pos >= ASCII_DEL)
        {
            break;
        }
        else if (*m_pos >= 'A' && *m_pos <= 'Z')
        {
            result.push_back(*m_pos - 'A' + 'a');
        }
        else
        {
            result.push_back(*m_pos);
        }
    }

    AdvanceToNextToken();
    return result;
}

void Tokenizer::AdvanceToNextToken()
{
    for (; m_pos != m_end; ++m_pos)
    {
        if (*m_pos >= ' ' && *m_pos < ASCII_DEL)
            break;
    }
}
