// SimpleCube.cpp
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

#include "SimpleCube.h"

#include <fstream>
#include <string>

#include "engine/GameEngine.h"
#include "ogl/ErrorCheck.h"
#include "ogl/Shader.h"

SimpleCube::SimpleCube()
{
    vao_ = new ogl::VertexArray();
    vbo_ = new ogl::VertexBuffer();

    ogl::Vertex vertices[36] = {
        // {{-0.5f, 0.5f, -0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, -1.0f}} // TOP LEFT BACK
        // {{-0.5f,-0.5f, -0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f, -1.0f}} // BTM LEFT BACK
        // {{ 0.5f,-0.5f, -0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, -1.0f}} // BTM RGHT BACK
        // {{ 0.5f, 0.5f, -0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f, -1.0f}} // TOP RGHT BACK
        
        // {{-0.5f, 0.5f,  0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, -1.0f}} // TOP LEFT FRNT
        // {{-0.5f,-0.5f,  0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f, -1.0f}} // BTM LEFT FRNT
        // {{ 0.5f,-0.5f,  0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, -1.0f}} // BTM RGHT FRNT
        // {{ 0.5f, 0.5f,  0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f, -1.0f}} // TOP RGHT FRNT
        
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, -1.0f}}, // TOP LEFT BACK
        {{-0.5f,-0.5f,-0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f, -1.0f}}, // BTM LEFT BACK
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, -1.0f}}, // BTM RGHT BACK
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, -1.0f}}, // TOP LEFT BACK
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, -1.0f}}, // BTM RGHT BACK
        {{ 0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f, -1.0f}}, // TOP RGHT BACK

        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f,  1.0f}}, // TOP LEFT FRNT
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f,  1.0f}}, // BTM LEFT FRNT
        {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f,  1.0f}}, // BTM RGHT FRNT
        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f,  1.0f}}, // TOP LEFT FRNT
        {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f,  1.0f}}, // BTM RGHT FRNT
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f,  1.0f}}, // TOP RGHT FRNT

        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 1.f, 0.0f}}, // TOP LEFT BACK
        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 1.f, 0.0f}}, // TOP LEFT FRNT
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 1.f, 0.0f}}, // TOP RGHT FRNT
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 1.f, 0.0f}}, // TOP LEFT BACK
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 1.f, 0.0f}}, // TOP RGHT FRNT
        {{ 0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 1.f, 0.0f}}, // TOP RGHT BACK

        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {1.f, 0.f, 0.0f}}, // TOP RGHT FRNT
        {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {1.f, 0.f, 0.0f}}, // BTM RGHT FRNT
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {1.f, 0.f, 0.0f}}, // BTM RGHT BACK
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {1.f, 0.f, 0.0f}}, // TOP RGHT FRNT
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {1.f, 0.f, 0.0f}}, // BTM RGHT BACK
        {{ 0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {1.f, 0.f, 0.0f}}, // TOP RGHT BACK

        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {-1.f, 0.f, 0.0f}}, // TOP LEFT FRNT
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {-1.f, 0.f, 0.0f}}, // BTM LEFT FRNT
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {-1.f, 0.f, 0.0f}}, // TOP LEFT BACK
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {-1.f, 0.f, 0.0f}}, // BTM LEFT FRNT
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {-1.f, 0.f, 0.0f}}, // TOP LEFT BACK
        {{-0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {-1.f, 0.f, 0.0f}}, // BTM LEFT BACK

        {{-0.5f,-0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f,-1.f, 0.0f}}, // BTM LEFT BACK
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f,-1.f, 0.0f}}, // BTM LEFT FRNT
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f,-1.f, 0.0f}}, // BTM RGHT BACK
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f,-1.f, 0.0f}}, // BTM LEFT FRNT
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f,-1.f, 0.0f}}, // BTM RGHT BACK
        {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f,-1.f, 0.0f}}, // BTM RGHT FRNT
    };

    vbo_->SetData(sizeof(ogl::Vertex)*36, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_->AddBuffer(*vbo_, vbl);   
}

SimpleCube::~SimpleCube()
{
    delete vao_;
    delete vbo_;
}

void SimpleCube::Update(float dtime)
{

}

void SimpleCube::Render(ogl::Program& program)
{
    vbo_->Bind();
    vao_->Bind();
    //program_->Use();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}