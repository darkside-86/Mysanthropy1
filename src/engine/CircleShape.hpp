// Circle.hpp
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

#include "Shape.hpp"

namespace engine
{
    // Defines a 2D drawable circle shape with varying precision--number of points
    class CircleShape : public Shape
    {
    public:
        // Constructor. precision is the number of points e.g. precision=5 results in a pentagon shape
        CircleShape(float x, float y, float radius, int precision);
        // Destructor
        virtual ~CircleShape();
        // Render the circle
        virtual void Render(GraphicsContext& gc) override;  
        // Save the modified vertices (see Shape) to graphics card memory
        virtual void ModifyVertices() override;
    };
}