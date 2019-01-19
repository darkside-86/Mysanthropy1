// Label.cpp
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

#include "Label.h"

#include "../GameEngine.h"
#include "ogl/Vertex.h"

namespace engine { namespace ui {
    Label::Label(Object* parent, const std::string& text, const std::string& fontAlias, int size, const Color& color)
        : Object(parent), text_(text), font_(fontAlias), ptSize_(size), color_(color)
    {
        CreateText();
    }

    Label::~Label()
    {
        delete vao_;
        delete vbo_;
        delete texture_;
    }

    void Label::Render(GraphicsContext& gc)
    {
        gc.PushModel();
        gc.TranslateModel((float)this->xPos_, (float)this->yPos_, 0.0f);
        gc.SetMVP();
        vao_->Bind();
        vbo_->Bind();
        texture_->Bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
        gc.PopModel();
    }

    void Label::CreateText()
    {
        GameEngine& ge = GameEngine::Get();
        TextRenderer& tr = ge.GetTextRenderer();
        tr.SetFGColor(color_.r, color_.g, color_.b, color_.a);
        delete texture_;
        texture_ = tr.RenderText(font_, text_, false);
        delete vao_;
        vao_ = new ogl::VertexArray();
        delete vbo_;
        vbo_ = new ogl::VertexBuffer();
        ogl::Vertex::Color c = {
            255,255,255,255
        };
        float width = (float)texture_->GetWidth();
        float height = (float)texture_->GetHeight();
        ogl::Vertex vertices[5] = {
            {{0.f, 0.f, 0.f},    c,     {0.f,0.f}},
            {{0.f, height, 0.f}, c,     {0.f,1.f}},
            {{width, height, 0.f}, c,   {1.f,1.f}},
            {{width, 0.f, 0.f},  c,     {1.f,0.f}},
            {{0.f, 0.f, 0.f},    c,     {0.f,0.f}}
        };
        vbo_->SetData(sizeof(ogl::Vertex)*5, vertices, GL_STATIC_DRAW);
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        vao_->AddBuffer(*vbo_, vbl);
    }
}}