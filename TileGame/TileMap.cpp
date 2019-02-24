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
    layer1_ = new Tile [width_ * height_];
    collisionLayer_ = new unsigned char [width_ * height_];
    memset(collisionLayer_, 0, width_ * height_);
    SetupRender();
}

TileMap::TileMap(const std::string& path)
{
    LoadFromFile(path);
}

TileMap::~TileMap()
{
    delete [] layer0_;
    delete [] layer1_;
    delete [] collisionLayer_;
    delete tileSet_;
    delete vao_;
    delete vao1_;
    delete collisionVao_;
    delete redTexture_;
}

Tile TileMap::GetTile(int ix, int iy, bool layer1)
{
    int index = iy*width_ + ix;
    if(index >= width_*height_ || index < 0)
        return {0,0};
    return !layer1? layer0_[index] : layer1_[index];
}

void TileMap::SetTile(int ix, int iy, const Tile& tile, bool layer1)
{
    int index = iy * width_ + ix;
    if(index >= width_*height_ || index < 0)
        return;
    if(!layer1)
        layer0_[index] = tile;
    else
        layer1_[index] = tile;
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
    const unsigned char MINOR_VERSION = 3;
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
    for(int i=0; i < width_ * height_; ++i)
    {
        out.write((char*)&layer1_[i], sizeof(Tile));
    }
    for(int i=0; i < width_ * height_; ++i)
    {
        out.write((char*)&collisionLayer_[i], sizeof(unsigned char));
    }

    out.close();
}

void TileMap::LoadFromFile(const std::string& path)
{
    // cleanup old data
    delete tileSet_;
    delete [] layer0_;
    delete [] layer1_;
    delete [] collisionLayer_;
    tileSet_ = nullptr;
    layer0_ = nullptr;
    layer1_ = nullptr;
    collisionLayer_ = nullptr;
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
    layer1_ = new Tile[width_*height_];
    collisionLayer_ = new unsigned char[width_ * height_];
    for(int i=0; i < width_*height_; ++i)
    {
        Tile tile;
        in.read((char*)&tile, sizeof(tile));
        layer0_[i] = tile;
    }
    for(int i=0; i < width_*height_; ++i)
    {
            Tile tile;
            in.read((char*)&tile, sizeof(tile));
            layer1_[i] = tile;
    }
    if(minorV >= 3)
    {
        for(int i=0; i < width_ * height_; ++i)
        {
            unsigned char c;
            in.read((char*)&c, sizeof(c));
            collisionLayer_[i] = c;
        }
    }
    else 
    {
        memset(collisionLayer_, 0, width_ * height_);
    }
    in.close();
    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
            "%s: Finished loading", __FUNCTION__);
    SetupRender();
}

void TileMap::SetupRender()
{
    ogl::Vertex* vertices;
    ogl::Vertex* l1Verts;
    // 6 vertices per tile
    int numVertices = 6 * width_ * height_;
    vertices = new ogl::Vertex[numVertices];
    l1Verts = new ogl::Vertex[numVertices];
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
    // engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
    //        "%s: Working on vertices...", __FUNCTION__);
    while(vi < numVertices)
    {
        float s0 = (float)layer0_[ti].ix / (float)ntx;
        float s1 = s0 + tw;
        float t0 = (float)layer0_[ti].iy / (float)nty;
        float t1 = t0 + th;
        // layer 0
        vertices[0+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
        vertices[1+vi] = {{x,y+h,0.f}, {255,255,255,255}, {s0, t1}, {0.f, 0.f, 1.f}}; 
        vertices[2+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
        vertices[3+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
        vertices[4+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
        vertices[5+vi] = {{x+w,y,0.f}, {255,255,255,255}, {s1, t0}, {0.f, 0.f, 1.f}}; 
        // layer 1
        s0 = (float)layer1_[ti].ix / (float)ntx;
        s1 = s0 + tw;
        t0 = (float)layer1_[ti].iy / (float)nty;
        t1 = t0 + th;
        l1Verts[0+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
        l1Verts[1+vi] = {{x,y+h,0.f}, {255,255,255,255}, {s0, t1}, {0.f, 0.f, 1.f}}; 
        l1Verts[2+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
        l1Verts[3+vi] = {{x,y,0.f}, {255,255,255,255}, {s0, t0}, {0.f, 0.f, 1.f}}; 
        l1Verts[4+vi] = {{x+w,y+h,0.f}, {255,255,255,255}, {s1, t1}, {0.f, 0.f, 1.f}}; 
        l1Verts[5+vi] = {{x+w,y,0.f}, {255,255,255,255}, {s1, t0}, {0.f, 0.f, 1.f}}; 
        vi += 6;
        ti += 1;
        x += w;
        if(ti > 0 && ti % width_ == 0)
        {
            y += h;
            x = 0.f;
        }
    }
    // engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
    //        "%s: ...Done", __FUNCTION__);
    vbo_.SetData(sizeof(ogl::Vertex)*numVertices, vertices, GL_STATIC_DRAW);
    vbo1_.SetData(sizeof(ogl::Vertex)*numVertices, l1Verts, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    delete vao_;
    delete vao1_;
    vao_ = new ogl::VertexArray();
    vao_->AddBuffer(vbo_, vbl);
    vao1_ = new ogl::VertexArray();
    vao1_->AddBuffer(vbo1_, vbl);
    delete [] vertices;
    delete [] l1Verts;

    // set up "X" indicator tile (only needs to be done one time)
    if(collisionVao_ == nullptr)
    {
        redTexture_ = new ogl::Texture("res/textures/red.png");
        ogl::Vertex xverts[] = { // line coords
            {{0.f,0.f,0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
            {{  w,  h,0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
            {{0.f,  h,0.f},{255,255,255,255},{0.f,1.f},{0.f,0.f,1.f}},
            {{w,  0.f,0.f},{255,255,255,255},{1.f,0.f},{0.f,0.f,1.f}},
        };
        collisionVbo_.SetData(sizeof(ogl::Vertex)*4, xverts, GL_STATIC_DRAW);
        collisionVao_ = new ogl::VertexArray();
        ogl::VertexBufferLayout vbl;
        ogl::Vertex::PushLayout(vbl);
        collisionVao_->AddBuffer(collisionVbo_, vbl);
    }
}

void TileMap::Render(int x, int y, ogl::Program& program, float scaleX, float scaleY)
{
    if(vao_ == nullptr)
        return;
    program.Use();
    vao_->Bind();
    tileSet_->GetTexture()->Bind();
    glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3((float)x, (float)y, 0.f));    
    model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.f));
    // model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
    program.SetUniform<glm::mat4>("u_model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6 * width_ * height_);
    vao1_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6 * width_ * height_);
    // RenderCollisionData(x,y,program,scaleX,scaleY);
}

void TileMap::RenderCollisionData(int x, int y, ogl::Program& program, float scaleX, float scaleY)
{
    int ix, iy;
    float w = (float)tileSet_->GetTileWidth() * scaleX;
    float h = (float)tileSet_->GetTileHeight() * scaleY;
    glm::vec3 location = {(float)x, (float)y, 0.f};
    collisionVao_->Bind();
    redTexture_->Bind();
    program.Use();
    glm::mat4 model(1.f);
    for(iy = 0; iy < height_; ++iy)
    {
        for(ix = 0; ix < width_; ++ix)
        {
            if(GetCollisionData(ix, iy) != 0)
            {
                model = glm::translate(glm::mat4(1.f), location);
                model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.f));
                program.SetUniform("u_model", model);
                glDrawArrays(GL_LINES, 0, 4);
            }
            location.x += w;
        }
        location.x = (float)x;
        location.y += h;
    }
}

void TileMap::FillWithTile(const Tile& tile, bool layer1)
{
    for(int i=0; i < width_*height_; ++i)
    {
        if(!layer1)
            layer0_[i] = tile;
        else
            layer1_[i] = tile;
    }
    SetupRender();
}

unsigned char TileMap::GetCollisionData(int ix, int iy)
{
    int index = iy * width_ + ix;
    if(index < 0 || index >= width_ * height_)
        return 0;
    else
        return collisionLayer_[index];
}

void TileMap::SetCollisionData(int ix, int iy, unsigned char value)
{
    int index = iy * width_ + ix;
    if(index >= 0 && index < width_ * height_)
        collisionLayer_[index] = value;
}