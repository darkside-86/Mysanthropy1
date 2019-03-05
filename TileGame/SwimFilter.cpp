// SwimFilter.cpp
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

#include <glm/gtc/matrix_transform.hpp>

#include  "SwimFilter.hpp"

SwimFilter::SwimFilter()
{
    texture_ = new ogl::Texture("res/textures/swimming.png");
    vertices_[0] = {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, 1.f}};
    vertices_[1] = {{0.f, 1.f, 0.f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f, 1.f}};
    vertices_[2] = {{1.f, 1.f, 0.f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, 1.f}};
    vertices_[3] = {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, 1.f}};
    vertices_[4] = {{1.f, 1.f, 0.f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, 1.f}};
    vertices_[5] = {{1.f, 0.f, 0.f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f, 1.f}};
    vbo_.SetData(6 * sizeof(ogl::Vertex), vertices_, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_.AddBuffer(vbo_, vbl);
}

SwimFilter::~SwimFilter()
{
    delete texture_;
}

void SwimFilter::Render(const glm::vec3& camera, ogl::Program& program)
{
    program.Use();
    glm::mat4 model = glm::translate(glm::mat4(1.f), camera+location_);
    program.SetUniform("u_model", model);
    texture_->Bind();
    vao_.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void SwimFilter::SetSpriteData(const Sprite* sprite)
{
    if(sprite == nullptr)
        return;

    location_ = sprite->GetPosition();
    float w = (float)sprite->GetWidth();
    float h = (float)sprite->GetHeight() / 2.0f;
    location_.y += h;
    vertices_[0] = {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, 1.f}};
    vertices_[1] = {{0.f, h, 0.f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f, 1.f}};
    vertices_[2] = {{w, h, 0.f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, 1.f}};
    vertices_[3] = {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, 1.f}};
    vertices_[4] = {{w, h, 0.f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, 1.f}};
    vertices_[5] = {{w, 0.f, 0.f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f, 1.f}};
    vbo_.SetData(6*sizeof(ogl::Vertex), vertices_, GL_STATIC_DRAW);
}