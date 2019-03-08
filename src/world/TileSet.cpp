// TileSet.cpp
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

#include "engine/GameEngine.hpp"
#include "ogl/Vertex.hpp"
#include "TileSet.hpp"

namespace world
{

    TileSet::TileSet(const std::string& fpath, int tileWidth, int tileHeight) 
        : pathToTexture_(fpath), tileWidth_(tileWidth), tileHeight_(tileHeight)
    {
        texture_ = new ogl::Texture(fpath, false, false);
        if(texture_->GetWidth() % tileWidth_ != 0)
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "Tile width is not a multiple of texture width!");
        else 
            numTilesX_ = texture_->GetWidth() / tileWidth_;
        
        if(texture_->GetWidth() % tileHeight_ != 0)
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "Tile height is not multiple of texture height!");
        else 
            numTilesY_ = texture_->GetHeight() / tileHeight_;

        float w = (float)tileWidth_;
        float h = (float)tileHeight_;
        float s = 1.0f / (float)numTilesX_;
        float t = 1.0f / (float)numTilesY_;
        // set up vao and vbo for rendering individual tiles
        ogl::Vertex vertices[6] = {
            {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
            {{0.0f,    h, 0.0f}, {255,255,255,255}, {0.0f,    t}, {0.0f, 0.0f, 0.0f}},
            {{   w,    h, 0.0f}, {255,255,255,255}, {   s,    t}, {0.0f, 0.0f, 0.0f}},

            {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},
            {{   w,    h, 0.0f}, {255,255,255,255}, {   s,    t}, {0.0f, 0.0f, 0.0f}},
            {{   w, 0.0f, 0.0f}, {255,255,255,255}, {   s, 0.0f}, {0.0f, 0.0f, 0.0f}},
        };
        vbo_.SetData(sizeof(ogl::Vertex)*6, vertices, GL_STATIC_DRAW);
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        vao_.AddBuffer(vbo_, vbl);
    }

    TileSet::~TileSet()
    {
        delete texture_;
    }

    void TileSet::GetNumTiles(int& numx, int& numy)
    {
        numx = numTilesX_;
        numy = numTilesY_;
    }

    void TileSet::DrawTile(int x, int y, float scaleX, float scaleY, ogl::Program& program, int ix, int iy)
    {
        float w = (float)tileWidth_;
        float h = (float)tileHeight_;
        float s0 = (float)ix / (float)numTilesX_;
        float t0 = (float)iy / (float)numTilesY_;
        float s1 = (float)(ix+1)  / (float)numTilesX_;
        float t1 = (float)(iy+1) / (float)numTilesY_;
        ogl::Vertex vertices[6] = {
            {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {  s0,   t0}, {0.0f, 0.0f, 0.0f}},
            {{0.0f,    h, 0.0f}, {255,255,255,255}, {  s0,   t1}, {0.0f, 0.0f, 0.0f}},
            {{   w,    h, 0.0f}, {255,255,255,255}, {  s1,   t1}, {0.0f, 0.0f, 0.0f}},

            {{0.0f, 0.0f, 0.0f}, {255,255,255,255}, {  s0,   t0}, {0.0f, 0.0f, 0.0f}},
            {{   w,    h, 0.0f}, {255,255,255,255}, {  s1,   t1}, {0.0f, 0.0f, 0.0f}},
            {{   w, 0.0f, 0.0f}, {255,255,255,255}, {  s1,   t0}, {0.0f, 0.0f, 0.0f}},
        };
        vbo_.SubData(0, sizeof(ogl::Vertex)*6, vertices);

        program.Use();
        vao_.Bind();
        texture_->Bind();
        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3((float)x, (float)y, 0.f));
        model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.f));
        program.SetUniform<glm::mat4>("u_model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}