// Game.h
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2018
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
#pragma once

#include "GraphicsContext.h"

namespace engine
{
    // Defines a base abstract class from which each application inherits.
    class Game
    {
    public:
        // Called when GameEngine is initialized
        virtual bool Initialize() = 0;
        // Update cycle for each frame
        virtual void Update(float dtime) = 0;
        // Render cycle for each frame
        virtual void Render(GraphicsContext& gc) = 0;
        // Called when GameEngine is cleaned up
        virtual void Cleanup() = 0;
    };
}
