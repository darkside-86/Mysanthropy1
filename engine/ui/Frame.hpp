// Frame.hpp
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

#include "Color.hpp"
#include "Object.hpp"

#include "ogl/Texture.hpp"
#include "ogl/VertexBuffer.hpp"
#include "ogl/VertexArray.hpp"

namespace engine { namespace ui {

    // Defines a rectangular object with optional border of customizable size. A frame has a texture that
    // repeats based on size of Frame rather than stretching. Even the border uses the same texture when rendering
    // its lines. Super class for many of the UI classes.
    class Frame : public Object
    {
    public:
        // Constructor. A texture and color are both parameters because the color is blended with the texture in
        // rendering.
        Frame(Object* parent, int w, int h, int xpos, int ypos, ogl::Texture* texture, const Color& color);
        // Destructor
        virtual ~Frame();
        // Renders the frame in 2D space
        virtual void Render(GraphicsContext& gc) override;   
        // Sets the width of the frame in logical pixels
        virtual void SetWidth(int w) override;
        // Sets the height of the frame in logical pixels
        virtual void SetHeight(int h) override;
        // Sets the color of the frame. The color is blended with the texture. The internal opengl objects are
        // modified to reflect the change
        inline void SetColor(const Color& color) 
            { color_ = color; CreateRectangle((float)width_,(float)height_,color_); CreateBorder(); }
        // Returns the color of the frame
        inline Color GetColor() const { return color_; }
        // Sets the color of the border then rebuilds it to reflect the change
        inline void SetBorderColor(const Color& color)
            { borderColor_ = color; CreateBorder(); }
        // returns the color of the border
        inline Color GetBorderColor() const { return borderColor_; }
        // Sets the line width of the border and recreates the border to reflect the changes.
        inline void SetBorderSize(int size) { borderSize_ = size; CreateBorder(); }
        // Returns the line width of the border
        inline int GetBorderSize() const { return borderSize_; }
    protected:
        // (Re)creates the opengl objects used to render the frame
        void CreateRectangle(float w, float h, const Color& color);
        // (Re)creates the opengl objects used to render the border
        void CreateBorder();
    private:
        // The color to blend with the texture
        Color color_;
        // The texture of the frame and border.
        ogl::Texture* texture_;
        // The opengl objects used to render the frame
        ogl::VertexArray* vao_ = nullptr;
        ogl::VertexBuffer* vbo_ = nullptr;
        // The line width of the border
        int borderSize_ = 0;
        // The color of the border, to be blended with the same texture rendered in the frame
        Color borderColor_;
        // The opengl objects used to render the border around the frame
        ogl::VertexArray* borderVao_ = nullptr;
        ogl::VertexBuffer* borderVbo_ = nullptr;
    };

}}