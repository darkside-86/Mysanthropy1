// SwimFilter.hpp
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

#include "ogl/Program.hpp"
#include "ogl/Texture.hpp"
#include "ogl/Vertex.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"
#include "Sprite.hpp"

namespace game
{

    class SwimFilter
    {
    public:
        SwimFilter();
        virtual ~SwimFilter();
        void Render(const glm::vec3& camera, ogl::Program& program);
        // set the dimensions of the vao object and location based on a sprite
        void SetSpriteData(const Sprite* sprite);
    private:
        glm::vec3 location_;
        ogl::Vertex vertices_[6];
        ogl::Texture* texture_;
        ogl::VertexArray vao_;
        ogl::VertexBuffer vbo_;
    };

}