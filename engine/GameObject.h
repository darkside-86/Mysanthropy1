// GameObject.h
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

#include <glm/glm.hpp>

#include "ogl/Program.h"

namespace engine 
{
    // Defines an abstract base class for 3D or 2D objects such as Sprites, mobs, entities, etc.
    class GameObject
    {
    public:
        // Updates the object for e.g. movement, animation etc. dtime is in seconds.
        virtual void Update(float dtime) = 0;
        // Renders the object given a specified shader. Camera should probably negated.
        virtual void Render(const glm::vec3& camPos, ogl::Program& program) = 0;
        // Gets the world position of the object
        inline virtual glm::vec3 GetPosition() const { return position_; }
        // Sets the world position of the object
        inline virtual void SetPosition(const glm::vec3& pos) { position_ = pos; }
        // Gets the velocity (movement speed) of the object
        inline virtual glm::vec3 GetVelocity() const { return velocity_; }
        // Sets the velocity of the object
        inline virtual void SetVelocity(const glm::vec3& vel) { velocity_ = vel; }
        // Gets the acceleration (velocity increase rate) of the object
        inline virtual glm::vec3 GetAcceleration() const { return acceleration_; }
        // Sets the acceleration of the object
        inline virtual void SetAcceleration(const glm::vec3& acc) { acceleration_ = acc; }
    protected:
        // The absolute object's position in the world
        glm::vec3 position_ = {0.f,0.f,0.f};
        // The speed of the object.
        glm::vec3 velocity_ = {0.f,0.f,0.f};
        // The rate at which velocity changes
        glm::vec3 acceleration_ = {0.f,0.f,0.f};
    };
}
