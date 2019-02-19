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
        virtual void Render(ogl::Program& program) = 0;
        glm::vec3 GetPosition() { return position_; }
        void SetPosition(const glm::vec3& pos) { position_ = pos; }
        glm::vec3 GetVelocity() { return velocity_; }
        void SetVelocity(const glm::vec3& vel) { velocity_ = vel; }
        glm::vec3 GetAcceleration() { return acceleration_; }
        void SetAcceleration(const glm::vec3& acc) { acceleration_ = acc; }
    private:
        glm::vec3 position_;
        glm::vec3 velocity_;
        glm::vec3 acceleration_;
    };
}
