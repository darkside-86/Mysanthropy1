// RectangleShape.h
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

#include "Shape.h"

namespace engine
{
    // Implements a modifyable 2D rectangle
    class RectangleShape : public Shape
    {
    public:
        RectangleShape(float left, float top, float right, float bottom);
        virtual ~RectangleShape();
        virtual void Render(GraphicsContext& gc) override;
        // After modifying the array accessed by GetVertices(), this method
        //  writes the array buffer data back to the GPU.
        virtual void ModifyVertices() override;
    private:
        RectangleShape(const RectangleShape&) {}
        void operator=(const RectangleShape&) {}
    };


}