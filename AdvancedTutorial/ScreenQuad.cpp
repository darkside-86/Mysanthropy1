// ScreenQuad.cpp
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

#include "ScreenQuad.h"

#include "ogl/ErrorCheck.h"

ScreenQuad::ScreenQuad()
{
    ogl::Vertex vertices[6] = {
        {{-1.f, 1.f, 0.f},{255,255,255,255},{0.f,1.f},{0.f,0.f,1.f}},
        {{-1.f,-1.f, 0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
        {{ 1.f,-1.f, 0.f},{255,255,255,255},{1.f,0.f},{0.f,0.f,1.f}},

        {{-1.f, 1.f, 0.f},{255,255,255,255},{0.f,1.f},{0.f,0.f,1.f}},
        {{ 1.f,-1.f, 0.f},{255,255,255,255},{1.f,0.f},{0.f,0.f,1.f}},
        {{ 1.f, 1.f, 0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
    };

    vbo_.SetData(sizeof(ogl::Vertex)*6, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_.AddBuffer(vbo_, vbl);
}

ScreenQuad::~ScreenQuad()
{

}

void ScreenQuad::Update(float dtime)
{

}

void ScreenQuad::Render(ogl::Program& program)
{
    OGL_ERROR_CHECK();
    program.Use();
    OGL_ERROR_CHECK();
    vao_.Bind();
    OGL_ERROR_CHECK();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    OGL_ERROR_CHECK();
}

