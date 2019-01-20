// Frame.h
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

#include "Color.h"
#include "Object.h"

#include "ogl/Texture.h"
#include "ogl/VertexBuffer.h"
#include "ogl/VertexArray.h"

namespace engine { namespace ui {

    class Frame : public Object
    {
    public:
        Frame(Object* parent, int w, int h, int xpos, int ypos, ogl::Texture* texture, const Color& color);
        virtual ~Frame();
        virtual void Render(GraphicsContext& gc) override;  
        Object* CheckPoint(int x, int y);  
        virtual void SetWidth(int w) override;
        virtual void SetHeight(int h) override;
        void SetColor(const Color& color) 
            { color_ = color; CreateRectangle((float)width_,(float)height_,color_); CreateBorder(); }
        Color GetColor() { return color_; }
        void SetBorderColor(const Color& color)
            { borderColor_ = color; CreateBorder(); }
        Color GetBorderColor() { return borderColor_; }
        void SetBorderSize(int size) { borderSize_ = size; CreateBorder(); }
        int GetBorderSize() { return borderSize_; }
    protected:
        void CreateRectangle(float w, float h, const Color& color);
        void CreateBorder();
    private:
        Color color_;
        ogl::Texture* texture_;
        ogl::VertexArray* vao_ = nullptr;
        ogl::VertexBuffer* vbo_ = nullptr;
        int borderSize_ = 0;
        Color borderColor_;
        ogl::VertexArray* borderVao_ = nullptr;
        ogl::VertexBuffer* borderVbo_ = nullptr;
    };

}}