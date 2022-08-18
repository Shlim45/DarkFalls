#include "application/Game.hpp"

namespace
{
    const int PORT = 4000;
}

int main()
{
    Mud::Game game(PORT);
    game.Start();

    std::cout << "Program terminated normally." << std::endl;
    return 0;
}
