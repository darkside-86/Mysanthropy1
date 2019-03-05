// main.cpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2019
//-----------------------------------------------------------------------------
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.If not, see < https://www.gnu.org/licenses/>.
//-----------------------------------------------------------------------------
#include <iostream>

#include "engine/GameEngine.hpp"
#include "TileGame/TileEditor.hpp"

int main(int argc, char* argv[])
{
    engine::GameEngine& ge = engine::GameEngine::Get();
    engine::Logger& logger = ge.GetLogger();
    bool ok = ge.Initialize("Tile Map Editor", 1024, 576);
    {
        TileEditor game;
        ok = ok && game.Initialize();
        if(!ok)
        {
            logger.Logf(engine::Logger::Severity::FATAL, "main: Unable to initialize!");
        }
        engine::GameEngine::Get().StartGameLoop(game);
        game.Cleanup();
    }
    engine::GameEngine::Get().Cleanup();
    return 0;
}