//
// Created by shlim on 8/17/22.
//

#ifndef DARKFALLS_GAME_HPP
#define DARKFALLS_GAME_HPP

#include "code/Server/Server.hpp"
#include "code/World/World.hpp"
#include <iostream>

namespace Mud
{
    class Game
    {
    public:
        Game(const int PORT) : m_server(PORT)
        {
            World::World world;
            m_world = std::make_unique<World::World>(world);
        }

        void Start()
        {
            std::thread server_thread([this]()
                                      {
                                          m_server.Run();
                                      });
            std::cout << "TEST" << std::endl;
            if (server_thread.joinable())
                server_thread.join();
        }

    private:
        std::unique_ptr<World::World> m_world;
        Server::Server m_server;
    };

} // Mud

#endif //DARKFALLS_GAME_HPP
