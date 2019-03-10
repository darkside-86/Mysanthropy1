// GameObject.hpp
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
#pragma once

#include <glm/glm.hpp>

#include "GraphicsContext.hpp"
#include "ogl/Program.hpp"

namespace engine 
{
    // Defines an abstract base class for 2D objects such as Sprites, mobs, entities, etc.
    class GameObject2D
    {
    public:
        // Updates the object for e.g. movement, animation etc. dtime is in seconds.
        virtual void Update(float dtime) = 0;
        // Renders the object given a specified shader. Camera should probably negated.
        virtual void Render(const glm::vec2& camPos, ogl::Program& program) = 0;
        // Gets the world position of the object
        void CalculatePosition(float dtime) 
        {
            velocity += acceleration * dtime;
            position += velocity * dtime;
        }
        glm::vec2 position = {0.f,0.f};
        // The speed of the object.
        glm::vec2 velocity = {0.f,0.f};
        // The rate at which velocity changes
        glm::vec2 acceleration = {0.f,0.f};
    };
}
