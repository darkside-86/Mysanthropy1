// SimplePlane.cpp 
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

#include "SimplePlane.h"

#include "engine/GameEngine.h"
#include "ogl/Vertex.h"

SimplePlane::SimplePlane()
{
    vao_ = new ogl::VertexArray();
    vbo_ = new ogl::VertexBuffer();

    ogl::Vertex vertices[4] = {
        {{-10.f,0.f,-10.f},{255,255,255,255},{0.f,0.f},{0.f,1.f,0.f}},
        {{-10.f,0.f, 10.f},{255,255,255,255},{0.f,1.f},{0.f,1.f,0.f}},
        {{ 10.f,0.f,-10.f},{255,255,255,255},{1.f,0.f},{0.f,1.f,0.f}},
        {{ 10.f,0.f, 10.f},{255,255,255,255},{1.f,1.f},{0.f,1.f,0.f}},
    };
    vbo_->SetData(sizeof(ogl::Vertex)*4, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_->AddBuffer(*vbo_, vbl);

    groundTexture_ = engine::GameEngine::Get().GetTextureManager().GetTexture("res/textures/ground.jpg");
}

SimplePlane::~SimplePlane()
{
    delete vao_;
    delete vbo_;
    engine::GameEngine::Get().GetTextureManager().UnloadTexture("res/textures/ground.jpg");
}

void SimplePlane::Render()
{
    vao_->Bind();
    groundTexture_->Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}