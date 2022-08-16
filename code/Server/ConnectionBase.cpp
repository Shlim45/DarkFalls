//
// Created by shlim on 8/14/22.
//
#include <iostream>
#include "ConnectionBase.hpp"

using namespace Mud::Server;

void ConnectionBase::WriteToSocket()
{
    if (m_writing)
    {
        m_moreToWrite = true;
        return;
    }
    m_writing = true;

    std::swap(m_outputBuffer, m_bufferBeingWritten);
    std::swap(m_outputStream, m_streamBeingWritten);

    async_write(m_socket, *m_bufferBeingWritten,
                [this](boost::system::error_code error, std::size_t length)
                {
                    m_writing = false;

                    if (error)
                        std::cerr << "[CONNECTION] ERROR: async_write returned an error "
                                  << error.message() << std::endl;
                    else if (m_moreToWrite)
                    {
                        WriteToSocket();
                        m_moreToWrite = false;
                        return;
                    }

                    if (!m_reading)
                    {
                        m_closeHandler();
                    }
                });
}