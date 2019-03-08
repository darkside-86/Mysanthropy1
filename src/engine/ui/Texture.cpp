// Texture.cpp
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.hpp"

namespace engine { namespace ui {

    Texture::Texture(Object* parent, const std::string& pathToTexture, int w, int h) :
        Object(parent)
    {
        width_ = w;
        height_ = h;

        texture_ = new ogl::Texture(pathToTexture);

        if(width_ <= 0)
            width_ = texture_->GetWidth();
        if(height_ <= 0)
            height_ = texture_->GetHeight();

        ogl::Vertex verts[6] = {
            {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {0.0f, 0.0f}, {0.0f, 0.f, 1.f}},
            {{0.0f, 1.0f, 0.0f}, {255,255,255,255}, {0.0f, 1.0f}, {0.0f, 0.f, 1.f}},
            {{1.0f, 1.0f, 0.0f}, {255,255,255,255}, {1.0f, 1.0f}, {0.0f, 0.f, 1.f}},
            {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {0.0f, 0.0f}, {0.0f, 0.f, 1.f}},
            {{1.0f, 1.0f, 0.0f}, {255,255,255,255}, {1.0f, 1.0f}, {0.0f, 0.f, 1.f}},
            {{1.0f, 0.0f, 0.0f}, {255,255,255,255}, {1.0f, 0.0f}, {0.0f, 0.f, 1.f}},
        };
        vbo_.SetData(6*sizeof(ogl::Vertex), verts, GL_STATIC_DRAW);
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        vao_.AddBuffer(vbo_, vbl);
    }

    Texture::~Texture()
    {
        delete texture_;
    }

    void Texture::Render(GraphicsContext& gc)
    {
        if(!IsVisible())
            return;
        gc.PushModel();
        gc.TranslateModel((float)xPos_, (float)yPos_, 0.0f);
        gc.PushModel(); // push twice because of scaling
        gc.ScaleModel((float)width_, (float)height_, 1.0f);
        gc.SetMVP();
        vao_.Bind();
        texture_->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        gc.PopModel(); // pop off the scaled model so we're back to translated model for children
        for(auto eachChild : children_)
        {       
            if(eachChild->GetXPos() >= 0 && eachChild->GetXPos() <= GetWidth()
              && eachChild->GetYPos() >= 0 && eachChild->GetYPos() <= GetHeight())
                eachChild->Render(gc);
        }
        gc.PopModel();
    }

}}