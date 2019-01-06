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
#include "Frame.h"

#include "ogl/Vertex.h"

namespace engine { namespace ui {

    Frame::Frame(Object* parent, int w, int h, int xpos, int ypos, ogl::Texture* texture) 
        : Object(parent), texture_(texture)
    {
        xPos_ = xpos;
        yPos_ = ypos;
        width_ = w;
        height_ = h;
        // todo: clip visible Frame area to parent object.
        CreateRectangle((float)width_, (float)height_);
    }

    Frame::~Frame()
    {
        delete vbo_;
        delete vao_;
    }
        
    void Frame::Render(GraphicsContext& gc)
    {
        if(!visible_)
            return;
        gc.TranslateModel((float)xPos_, (float)yPos_, 0.f);
        gc.SetMVP();
        if(texture_) texture_->Bind();
        vao_->Bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        for(auto eachChild : children_)
        {
            eachChild->Render(gc);
        }
    }

    void Frame::SetWidth(int w)
    {
        width_ = w; // todo: clip object to parent object
    }

    void Frame::SetHeight(int h)
    {
        height_ = h; // todo: clip object to parent object
    }

    void Frame::CreateRectangle(float w, float h)
    {
        ogl::Vertex vertices[4];
        float s=1.f,t=1.f;
        if(texture_)
        {
            s = w / (float)texture_->GetWidth();
            t = h / (float)texture_->GetHeight();
        }
        vertices[0] =  {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f,0.f}};
        vertices[1] =  {{0.f, h, 0.f}, {255,255,255,255}, {0.f,t}};
        vertices[2] =  {{w, 0.f, 0.f}, {255,255,255,255}, {s,0.f}};
        vertices[3] =  {{w, h, 0.f}, {255,255,255,255}, {s,t}};
        if(vbo_)
            delete vbo_;
        vbo_ = new ogl::VertexBuffer();
        vbo_->SetData(sizeof(vertices), vertices, GL_STATIC_DRAW);
        if(vao_)
            delete vao_;
        vao_ = new ogl::VertexArray();
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        vao_->AddBuffer(*vbo_, vbl);
    }

}}