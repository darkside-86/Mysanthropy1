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

    // A textured quad for placing over the bottom half of an entity or sprite whose center base
    //  position is in a tile considered "liquid" by the map configuration. The game uses the map
    //  configuration to determine what texture is used.
    class SwimFilter
    {
    public:
        // ctor.
        SwimFilter(const std::string& texturePath);
        // dtor.
        virtual ~SwimFilter();
        // Render the quad. Position is determined by location of the sprite that is "swimming".
        void Render(const glm::vec2& camera, ogl::Program& program);
        // set the dimensions of the vao object and location based on a sprite
        void SetSpriteData(const Sprite* sprite);
    private:
        // location of where to render, filled by sprite location data
        glm::vec2 location_;
        // ogl objects for drawing the quad
        ogl::Vertex vertices_[6];
        // see above (this is configured according to map)
        ogl::Texture* texture_;
        // see above
        ogl::VertexArray vao_;
        // see above
        ogl::VertexBuffer vbo_;
    };

}