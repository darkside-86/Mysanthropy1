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
    class GameObject
    {
    public:
        virtual void Update(float dtime) = 0;
        virtual void Render(const glm::vec3& camPos, ogl::Program& program) = 0;
        virtual glm::vec3 GetPosition() { return position_; }
        virtual void SetPosition(const glm::vec3& pos) { position_ = pos; }
        virtual glm::vec3 GetVelocity() { return velocity_; }
        virtual void SetVelocity(const glm::vec3& vel) { velocity_ = vel; }
        virtual glm::vec3 GetAcceleration() { return acceleration_; }
        virtual void SetAcceleration(const glm::vec3& acc) { acceleration_ = acc; }
    protected:
        glm::vec3 position_ = {0.f,0.f,0.f};
        glm::vec3 velocity_ = {0.f,0.f,0.f};
        glm::vec3 acceleration_ = {0.f,0.f,0.f};
    };
}
