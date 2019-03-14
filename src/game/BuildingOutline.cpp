// BuildingOutline.cpp
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

#include "BuildingOutline.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ogl/Vertex.hpp"

namespace game
{
    BuildingOutline::BuildingOutline(int width, int height)
    {
        // define the vertex data
        float w = (float)width;
        float h = (float)height;
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
        // load the two textures and set the current one to the green one
        placeableTexture_ = new ogl::Texture("res/textures/building_outline.png");
        errorTexture_ = new ogl::Texture("res/textures/building_outline_error.png");
        currentTexture_ = placeableTexture_;
    }   
     
    BuildingOutline::~BuildingOutline()
    {
        delete placeableTexture_;
        delete errorTexture_;
    }
    
    void BuildingOutline::SetToError()
    {
        currentTexture_ = errorTexture_;
    }
    
    void BuildingOutline::ClearError()
    {
        currentTexture_ = placeableTexture_;
    }
    
    void BuildingOutline::DrawOnScreenAt(int x, int y, ogl::Program& program)
    {
        // first set up program's model matrix
        program.Use();
        program.SetUniform("u_model", glm::translate(glm::mat4(1.f), glm::vec3(
            (float)x, float(y), 0.0f)));
        // bind the texture in use
        currentTexture_->Bind();
        // bind ogl objects
        vao_.Bind();
        // draw the quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}