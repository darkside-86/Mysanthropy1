// BuildingOutline.hpp
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

#include "ogl/Program.hpp"
#include "ogl/Texture.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"

namespace game
{
    // Defines a semi-transparent square used for indicating where a building should be placed by user.
    //  The texture in use can be set to green to indicate that the building is placeable, and red to
    //  indicate that the building cannot be placed at the specified location.
    class BuildingOutline
    {
    public:
        // constructor - takes width and height in logical pixels
        BuildingOutline(int w, int h);
        // destructor
        virtual ~BuildingOutline();
        // Sets the current texture to the error indicating one
        void SetToError();
        // Sets the current texture to the placeable indicator (starting state)
        void ClearError();
        // Draws the square on the screen at the specified logical pixel coordinates
        void DrawOnScreenAt(int x, int y, ogl::Program& program);
    private:
        // the texture in use. One of the below two options.
        ogl::Texture* currentTexture_;
        // A green texture indicating that the building is ready to be placed
        ogl::Texture* placeableTexture_;
        // A red texture for the quad indicating that the building cannot be placed
        ogl::Texture* errorTexture_;
        // The ogl objects for the quad
        ogl::VertexArray vao_;
        // See above
        ogl::VertexBuffer vbo_;
    };
}