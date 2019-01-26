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

#include "../GameEngine.h"
#include "ogl/Vertex.h"

namespace engine { namespace ui {

    Frame::Frame(Object* parent, int w, int h, int xpos, int ypos, ogl::Texture* texture, const Color& color) 
        : Object(parent), texture_(texture), color_(color), vbo_(nullptr), vao_(nullptr), borderVao_(nullptr),
          borderVbo_(nullptr)
    {
        xPos_ = xpos;
        yPos_ = ypos;
        width_ = w;
        height_ = h;
        // allocate vbo_ and vao_
        CreateRectangle((float)width_, (float)height_, color);
        // possibly allocate borderVao_ and borderVbo_
        if(borderSize_)
            CreateBorder();
    }

    Frame::~Frame()
    {
        GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING, "Destructor on Frame %llx", this);
        delete vbo_;
        delete vao_;
        delete borderVao_;
        delete borderVbo_;
    }
        
    void Frame::Render(GraphicsContext& gc)
    {
        if(!visible_)
            return;
        gc.PushModel();
        gc.TranslateModel((float)xPos_, (float)yPos_, 0.f);
        gc.SetMVP();
        if(texture_) 
            texture_->Bind();
        vao_->Bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        if(borderSize_ && borderVao_)
        {
            static const unsigned int indices[] = {0,1,3,2,0};
            borderVao_->Bind();
            float oldLineWidth;
            glGetFloatv(GL_LINE_WIDTH, &oldLineWidth);
            glLineWidth((float)borderSize_);
            glDrawElements(GL_LINE_STRIP, 5, GL_UNSIGNED_INT, indices);
            glLineWidth(oldLineWidth);
        }
        for(auto eachChild : children_)
        {
            eachChild->Render(gc);
        }
        gc.PopModel();
    }

    void Frame::SetWidth(int w)
    {
        width_ = w; 
        CreateRectangle((float)width_, (float)height_, color_);
    }

    void Frame::SetHeight(int h)
    {
        height_ = h; 
        CreateRectangle((float)width_, (float)height_, color_);
    }

    void Frame::CreateRectangle(float w, float h, const Color& color)
    {
        // todo: clip to parent width and height
        ogl::Vertex vertices[4];
        float s=1.f,t=1.f;
        if(texture_)
        {
            s = w / (float)texture_->GetWidth();
            t = h / (float)texture_->GetHeight();
        }
        ogl::Vertex::Color c = {
            (unsigned char)(255.f*color.r), (unsigned char)(255.f*color.g), 
            (unsigned char)(255.f*color.b), (unsigned char)(255.f*color.a)
        };
        vertices[0] =  {{0.f, 0.f, 0.f}, c, {0.f,0.f}};
        vertices[1] =  {{0.f, h, 0.f}, c, {0.f,t}};
        vertices[2] =  {{w, 0.f, 0.f}, c, {s,0.f}};
        vertices[3] =  {{w, h, 0.f}, c, {s,t}};
        delete vbo_;
        vbo_ = new ogl::VertexBuffer();
        vbo_->SetData(sizeof(vertices), vertices, GL_STATIC_DRAW);
        delete vao_;
        vao_ = new ogl::VertexArray();
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        vao_->AddBuffer(*vbo_, vbl);
    }

    void Frame::CreateBorder()
    {
        ogl::Vertex vertices[4];
        const float left = 0.f;
        const float top = 0.f;
        const float right = (float)width_;
        const float bottom = (float)height_;
        const unsigned char red = (unsigned char)(borderColor_.r * 255.f);
        const unsigned char green = (unsigned char)(borderColor_.g * 255.f);
        const unsigned char blue = (unsigned char)(borderColor_.b * 255.f);
        const unsigned char alpha = (unsigned char)(borderColor_.a * 255.f);
        float s=1.f,t=1.f;
        if(texture_)
        {
            s = (right - left)/(float)texture_->GetWidth();
            t = (bottom - top)/(float)texture_->GetHeight();
        }
        vertices[0] =  {{left,top,0.f}, {red,green,blue,alpha}, {0.f,0.f}};
        vertices[1] =  {{left,bottom,0.f}, {red,green,blue,alpha}, {0.f,t}};
        vertices[2] =  {{right,top,0.f},{red,green,blue,alpha}, {s,0.f}};
        vertices[3] =  {{right,bottom,0.f}, {red,green,blue,alpha}, {s,t}};
        delete borderVao_;
        delete borderVbo_;
        borderVbo_ = new ogl::VertexBuffer();
        borderVbo_->SetData(sizeof(ogl::Vertex)*4, vertices, GL_STATIC_DRAW);
        borderVao_ = new ogl::VertexArray();
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        borderVao_->AddBuffer(*borderVbo_, vbl);
    }

}}