//
// Created by shlim on 8/16/22.
//

#include "Tokenizer.hpp"

using namespace Mud::Dictionary;

namespace
{
    const std::string NO_STRING;
    const char ASCII_DEL = 0x7F;

    const char TELNET_WILL = 0xFB;
    const char TELNET_WONT = 0xFC;
    const char TELNET_DO   = 0xFD;
    const char TELNET_DONT = 0xFE;
    const char TELNET_IAC  = 0xFF;
}

void Tokenizer::IgnoreTelnetCommand()
{
    ++m_pos;
    if (*m_pos >= TELNET_WILL && *m_pos <= TELNET_DONT)
        ++m_pos;
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
    else if (*m_pos == ' ')
    {
        AdvanceThroughSpaces();
        return ReadNextString();
    }
    else
    {
        return ReadNextString();
    }
}

const std::string &Tokenizer::GetCommand()
{
    auto &result = m_cache.front();
    return result;
}

std::string Tokenizer::Combine(const char delim)
{
    std::string cacheResult;
    if (!m_cache.empty())
    {
        for (const auto &c : m_cache)
            cacheResult.append(c).push_back(delim);
    }

    auto &result = m_cache.emplace_back();
    m_cacheIterator = m_cache.end();
    for (++m_pos; m_pos != m_end; ++m_pos)
    {
        if (*m_pos < ' ' || *m_pos >= ASCII_DEL)
            break;
        else if (*m_pos == ' ')
            result.push_back(delim);
        else
            result.push_back(*m_pos);
    }
    return cacheResult + result;
}

std::string Tokenizer::CombineRemaining(const std::string &delim)
{
    auto &result = m_cache.emplace_back();
    m_cacheIterator = m_cache.end();
    for (++m_pos; m_pos != m_end; ++m_pos)
    {
        if (*m_pos < ' ' || *m_pos >= ASCII_DEL)
            break;
        else
            result.push_back(*m_pos);
    }

    return result;
}

const std::string &Tokenizer::ReadNextString()
{
    auto &result = m_cache.emplace_back();
    m_cacheIterator = m_cache.end();

    for (; m_pos != m_end; ++m_pos)
    {
        if (*m_pos == TELNET_IAC)
            IgnoreTelnetCommand();
        else if (*m_pos <= ' ' || *m_pos >= ASCII_DEL)
        {
            break;
        }
//        else if (*m_pos >= 'A' && *m_pos <= 'Z')
//        {
//            result.push_back(*m_pos - 'A' + 'a');
//        }
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
        if (*m_pos == TELNET_IAC)
            IgnoreTelnetCommand();
        else if (*m_pos >= ' ' && *m_pos < ASCII_DEL)
            break;
    }
}

void Tokenizer::AdvanceThroughSpaces()
{
    for (; m_pos != m_end; ++m_pos)
    {
        if (*m_pos == TELNET_IAC)
            IgnoreTelnetCommand();
        else if (*m_pos > ' ' && *m_pos < ASCII_DEL)
            break;
    }
}
