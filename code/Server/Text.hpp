//
// Created by shlim on 8/16/22.
//

#ifndef DARKFALLS_TEXT_HPP
#define DARKFALLS_TEXT_HPP

namespace Mud
{
namespace Server
{
    // ANSI sequences
    const std::string BLACKTEXT("\x1B[30m");
    const std::string REDTEXT("\x1B[31m");
    const std::string GREENTEXT("\x1B[32m");
    const std::string BROWNTEXT("\x1B[33m");
    const std::string BLUETEXT("\x1B[34m");
    const std::string MAGENTATEXT("\x1B[35m");
    const std::string CYANTEXT("\x1B[36m");
    const std::string GREYTEXT("\x1B[37m");

    const std::string BR_BLACKTEXT("\x1B[90m");
    const std::string BR_REDTEXT("\x1B[91m");
    const std::string BR_GREENTEXT("\x1B[92m");
    const std::string YELLOWTEXT("\x1B[93m");
    const std::string BR_BLUETEXT("\x1B[94m");
    const std::string BR_MAGENTATEXT("\x1B[95m");
    const std::string BR_CYANTEXT("\x1B[96m");

    const std::string PLAINTEXT("\x1B[97m");

    // Telnet sequences
    const std::string ECHOOFF("\xFF\xFB\x01");
    const std::string ECHOON("\xFF\xFC\x01");

    const std::string NEWLINE("\r\n");

    static std::string ColorizeText(const std::string &text, const std::string &color)
    {
        return color + text + PLAINTEXT;
    }

} // Server
} // Mud

#endif //DARKFALLS_TEXT_HPP
