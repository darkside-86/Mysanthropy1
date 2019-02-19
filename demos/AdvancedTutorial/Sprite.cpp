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

#include "ogl/Vertex.h"

struct VTX
{
    struct Position
    {
        float x,y,z;
    } position;

    struct TexCoords
    {
        float s, t;
    } texCoords;
};

Sprite::Sprite(ogl::Texture* texture, int w, int h)
{
    width_ = w;
    height_ = h;
    position_ = {0.0f, 0.0f, 0.0f};
    velocity_ = {0.0f, 0.0f, 0.0f};
    texture_ = texture;

    VTX vertices[6] = {
        // position          texCoords
        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.0f,(float)h, 0.0f}, {0.0f, 1.0f}},
        {{(float)w, (float)h, 0.0f}, {1.0f,1.0f}},

        {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{(float)w, (float)h, 0.0f}, {1.0f, 1.0f}},
        {{(float)w, 0.0f, 0.0f}, {1.0f, 0.0f}}
    };
    vbo_.SetData(sizeof(VTX)*6, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    vbl.Push<float>(3, false);
    vbl.Push<float>(2, false);
    vao_.AddBuffer(vbo_, vbl);
}

Sprite::~Sprite()
{

}

void Sprite::Update(float dtime)
{
    position_ += dtime * velocity_;
}

void Sprite::Render(ogl::Program& program)
{
    program.Use();
    vao_.Bind();
    texture_->Bind();
    glm::mat4 model = glm::translate(glm::mat4(1.f), position_);
    program.SetUniform<glm::mat4>("u_model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 1);
}