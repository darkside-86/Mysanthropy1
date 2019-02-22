// Sprite.cpp
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

#include "Sprite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ogl/ErrorCheck.h"
#include "ogl/Vertex.h"

Sprite::Sprite(ogl::Texture* img) :
    anim0_(img)
{
    width_ = anim0_->GetWidth();
    height_ = anim0_->GetHeight();
    float w = (float)width_;
    float h = (float)height_;
    ogl::Vertex vertices[6] = {
        {{0.f,0.f,0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
        {{0.f,  h,0.f},{255,255,255,255},{0.f,1.f},{0.f,0.f,1.f}},
        {{  w,  h,0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
        {{0.f,0.f,0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
        {{  w,  h,0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
        {{  w,0.f,0.f},{255,255,255,255},{1.f,0.f},{0.f,0.f,1.f}}
    };
    vbo_.SetData(6*sizeof(ogl::Vertex), vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_.AddBuffer(vbo_, vbl);
}

Sprite::~Sprite()
{

}

void Sprite::Update(float dtime)
{
    position_ += dtime * velocity_;
    velocity_ += dtime * acceleration_;
}

void Sprite::Render(ogl::Program& program)
{
    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(position_));
    program.Use();
    program.SetUniform("u_model", model);
    vao_.Bind();
    anim0_->Bind();
    OGL_ERROR_CHECK();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    OGL_ERROR_CHECK();
}