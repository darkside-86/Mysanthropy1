// TileMap.cpp
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

#include "TileMap.h"

#include <fstream>

#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"

TileMap::TileMap(int tileWidth, int tileHeight, const std::string& tilesetPath, int width, int height)
    : width_(width), height_(height)
{
    tileSet_ = new TileSet(tilesetPath, tileWidth, tileHeight);
    layer0_ = new Tile [width_ * height_];
    SetupRender();
}

TileMap::TileMap(const std::string& path)
{
    LoadFromFile(path);
}

TileMap::~TileMap()
{
    delete [] layer0_;
    delete tileSet_;
    delete vao_;
}

/*void TileMap::Draw(int x, int y, ogl::Program& program)
{
    int drawx = x;
    int drawy = y;
    for(int iy = 0; iy < height_; ++iy)
    {
        for(int ix = 0; ix < width_; ++ix)
        {
            Tile& tile = tiles_[iy*width_ + ix];
            tileSet_->DrawTile(drawx, drawy, 1.f, 1.f, program, tile.ix, tile.iy);
            drawx += tileSet_->GetTileWidth();
        }
        drawx = x;
        drawy += tileSet_->GetTileHeight();
    }
}*/

Tile TileMap::GetTile(int ix, int iy)
{
    int index = iy*width_ + ix;
    if(index >= width_*height_ || index < 0)
        return {0,0};
    return layer0_[index];
}

void TileMap::SetTile(int ix, int iy, const Tile& tile)
{
    int index = iy * width_ + ix;
    if(index >= width_*height_ || index < 0)
        return;
    layer0_[index] = tile;
    SetupRender();
}

void TileMap::SaveToFile(const std::string& path)
{
    std::ofstream out;
    out.open(path, std::ios::binary);
    if(!out.is_open())
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "%s: Unable to open `%s' for writing", __FUNCTION__, path.c_str());
        return;
    }

    const unsigned char MAJOR_VERSION = 0;
    const unsigned char MINOR_VERSION = 1;
    const std::string TILESET_PATH = tileSet_->GetPathToTexture();
    out.write((char*)&MAJOR_VERSION, sizeof(unsigned char));
    out.write((char*)&MINOR_VERSION, sizeof(unsigned char));
    auto size = TILESET_PATH.size();
    out.write((char*)&size, sizeof(size));
    out.write(TILESET_PATH.c_str(), TILESET_PATH.size());
    int tileWidth=tileSet_->GetTileWidth(), tileHeight=tileSet_->GetTileHeight();
    out.write((char*)&tileWidth, sizeof(tileWidth));
    out.write((char*)&tileHeight, sizeof(tileHeight));
    out.write((char*)&width_, sizeof(width_));
    out.write((char*)&height_, sizeof(height_));
    for(int i=0; i < width_*height_; ++i)
    {
        out.write((char*)&layer0_[i], sizeof(Tile));
    }

    out.close();
}

void TileMap::LoadFromFile(const std::string& path)
{
    // cleanup old data
    delete tileSet_;
    delete [] layer0_;
    tileSet_ = nullptr;
    layer0_ = nullptr;
    width_ = 0;
    height_ = 0;
    
    std::ifstream in;
    in.open(path, std::ios::binary);
    // first read major, minor version
    unsigned char majorV, minorV;
    in.read((char*)&majorV, sizeof(majorV));
    in.read((char*)&minorV, sizeof(minorV));
    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
            "%s: TileMap version %d.%d", __FUNCTION__, majorV, minorV);
    // read path to tileset
    size_t size;
    in.read((char*)&size, sizeof(size));
    char* szPathToTileset = new char[size+1];
    szPathToTileset[size] = 0;
    in.read(szPathToTileset, size);
    std::string pathToTileset = szPathToTileset;
    delete [] szPathToTileset;
    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
            "%s: Path to tileset: `%s'", __FUNCTION__, pathToTileset.c_str());
    // read tile width and height for tileset
    int tileWidth, tileHeight;
    in.read((char*)&tileWidth, sizeof(tileWidth));
    in.read((char*)&tileHeight, sizeof(tileHeight));
    // create tileset with info
    tileSet_ = new TileSet(pathToTileset, tileWidth, tileHeight);
    // read width and height of map
    in.read((char*)&width_, sizeof(width_));
    in.read((char*)&height_, sizeof(height_));
    layer0_ = new Tile[width_*height_];
    for(int i=0; i < width_*height_; ++i)
    {
        Tile tile;
        in.read((char*)&tile, sizeof(tile));
        layer0_[i] = tile;
    }
    in.close();
    SetupRender();
}

void TileMap::SetupRender()
{
    ogl::Vertex* vertices;
    // 6 vertices per tile
    int numVertices = 6 * width_ * height_;
    vertices = new ogl::Vertex[numVertices];
    int vi = 0;
    int ti = 0;
    float w = (float)tileSet_->GetTileWidth();
    float h = (float)tileSet_->GetTileHeight();
    int ntx, nty;
    tileSet_->GetNumTiles(ntx, nty);
    float tw = 1.f / (float)ntx;
    float th = 1.f / (float)nty;
    float x = 0.0f;
    float y = 0.0f;
    while(vi < numVertices)
    {
        float s0 = (float)layer0_[ti].ix / (float)ntx;
        float s1 = s0 + tw;
        float t0 = (float)layer0_[ti].iy / (float)nty;
        float t1 = t0 + th;
        vertices[0+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
        vertices[1+vi] = {{x,y+h,0.f}, {255,255,255,255}, {s0, t1}, {0.f, 0.f, 1.f}}; 
        vertices[2+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
        vertices[3+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
        vertices[4+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
        vertices[5+vi] = {{x+w,y,0.f}, {255,255,255,255}, {s1, t0}, {0.f, 0.f, 1.f}}; 
        vi += 6;
        ti += 1;
        x += w;
        if(ti > 0 && ti % width_ == 0)
        {
            y += h;
            x = 0.f;
        }
    }
    vbo_.SetData(sizeof(ogl::Vertex)*numVertices, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    delete vao_;
    vao_ = new ogl::VertexArray();
    vao_->AddBuffer(vbo_, vbl);   
    delete [] vertices;
}

void TileMap::Render(int x, int y, ogl::Program& program)
{
    if(vao_ == nullptr)
        return;
    program.Use();
    vao_->Bind();
    tileSet_->GetTexture()->Bind();
    glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3((float)x, (float)y, 0.f));
    program.SetUniform<glm::mat4>("u_model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * width_ * height_);
}
