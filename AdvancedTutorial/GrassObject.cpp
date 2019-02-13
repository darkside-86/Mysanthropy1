// GrassObject.cpp
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

#include "GrassObject.h"

#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"

GrassObject::GrassObject(const glm::vec3& position)
    : position_(position)
{
    ogl::Vertex vertices[8] = {
        {{-0.5f, 1.f, 0.f},{255,255,255,255},{ 0.f, 0.f},{ 0.f, 0.f, 1.f}},
        {{-0.5f, 0.f, 0.f},{255,255,255,255},{ 0.f, 1.f},{ 0.f, 0.f, 1.f}},
        {{ 0.5f, 1.f, 0.f},{255,255,255,255},{ 1.f, 0.f},{ 0.f, 0.f, 1.f}},
        {{ 0.5f, 0.f, 0.f},{255,255,255,255},{ 1.f, 1.f},{ 0.f, 0.f, 1.f}},

        {{ 0.f, 1.f, 0.5f},{255,255,255,255},{ 0.f, 0.f},{ 0.f, 1.f, 0.f}},
        {{ 0.f, 0.f, 0.5f},{255,255,255,255},{ 0.f, 1.f},{ 0.f, 1.f, 0.f}},
        {{ 0.f, 1.f,-0.5f},{255,255,255,255},{ 1.f, 0.f},{ 0.f, 1.f, 0.f}},
        {{ 0.f, 0.f,-0.5f},{255,255,255,255},{ 1.f, 1.f},{ 0.f, 1.f, 0.f}},
    };

    vbo_.SetData(sizeof(ogl::Vertex)*8, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_.AddBuffer(vbo_, vbl);

    texture_ = engine::GameEngine::Get().GetTextureManager().GetTexture("res/textures/grass.png");
    texture_->SetRepeat(false);
}

GrassObject::~GrassObject()
{
    engine::GameEngine::Get().GetTextureManager().UnloadTexture("res/textures/grass.png");
}

void GrassObject::Update(float dtime)
{

}

void GrassObject::Render(ogl::Program& program)
{
    program.Use();
    program.SetUniform<glm::mat4>("u_model", glm::translate(glm::mat4(1.f), position_));
    texture_->Bind();
    vao_.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
}