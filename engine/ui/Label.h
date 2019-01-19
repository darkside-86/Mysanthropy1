// Label.h
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

#include <string>

#include "Color.h"
#include "Object.h"
#include "ogl/Texture.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"

namespace engine { namespace ui {

    class Label : public Object
    {
    public:
        Label(Object* parent, const std::string& text, const std::string& fontAlias, int size, const Color& color);
        virtual ~Label();
        virtual void Render(GraphicsContext& gc) override;

        void SetText(const std::string& text) { text_ = text; CreateText(); }
        std::string GetText() { return text_; }
    private:
        void CreateText();

        std::string text_;
        std::string font_;
        int ptSize_;
        Color color_;
        ogl::VertexArray* vao_ = nullptr;
        ogl::VertexBuffer* vbo_ = nullptr;
        ogl::Texture* texture_ = nullptr;
    };

}}