//
// Created by shlim on 8/14/22.
//

#ifndef DARKFALLS_CONNECTION_HPP
#define DARKFALLS_CONNECTION_HPP

#include <boost/asio.hpp>

namespace Mud
{
namespace Server
{
    typedef boost::asio::ip::tcp::socket SocketType;

/// Handles output
class Connection
{
public:
    explicit Connection(SocketType &&socket)
    : m_socket(std::move(socket)),
      m_outputStream1(&m_outputBuffer1),
      m_outputStream2(&m_outputBuffer2),
      m_outputBuffer(&m_outputBuffer1),
      m_bufferBeingWritten(&m_outputBuffer2),
      m_outputStream(&m_outputStream1),
      m_streamBeingWritten(&m_outputStream2),
      m_reading(true), m_writing(false), m_moreToWrite(false)
    {}

    template <typename T>
    void Write(const T &message)
    {
        *m_outputStream << message;
        WriteToSocket();
    }

    template <typename T>
    std::ostream &operator<<(const T &message)
    {
        Write(message);
        m_moreToWrite = true;
        return *m_outputStream;
    }

    std::ostream &ostream()
    {
        WriteToSocket();
        m_moreToWrite = true;
        return *m_outputStream;
    }

    template<typename Handler>
    void SetCloseHandler(Handler &&handler)
    {
        m_closeHandler = std::forward<Handler>(handler);
    }

protected:
    void DoneReading()
    {
        m_reading = false;
        WriteToSocket();
    }

    SocketType m_socket;

private:
    void WriteToSocket();

    boost::asio::streambuf m_outputBuffer1, m_outputBuffer2;
    std::ostream m_outputStream1, m_outputStream2;
    boost::asio::streambuf *m_outputBuffer, *m_bufferBeingWritten;
    std::ostream *m_outputStream, *m_streamBeingWritten;

    bool m_reading, m_writing, m_moreToWrite;

    std::function<void()> m_closeHandler;
};


} // Server
} // Mud

#endif //DARKFALLS_CONNECTION_HPP
