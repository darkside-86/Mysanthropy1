// TileMap.h
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
#pragma once

#include <string>
#include <vector>

#include "ogl/Program.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"
#include "TileSet.h"

struct Tile
{
    unsigned short ix=0;
    unsigned short iy=0;
};

class TileMap
{
public:
    TileMap(int tileWidth, int tileHeight, const std::string& tilesetPath, int width, int height);
    TileMap(const std::string& path);
    virtual ~TileMap();
    // void Draw(int x, int y, ogl::Program& program); // deprecated. for debugging only
    void Render(int x, int y, ogl::Program &program);
    void SaveToFile(const std::string& path);
    void LoadFromFile(const std::string& path);
    TileSet* GetTileSet() { return tileSet_; }
    int GetWidth() { return width_; }
    int GetHeight() { return height_; }
    Tile GetTile(int ix, int iy, bool layer1 = false);
    void SetTile(int ix, int iy, const Tile& tile, bool layer1=false);
    void FillWithTile(const Tile& tile, bool layer1=false);
private:
    void SetupRender(); 
    TileSet* tileSet_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    Tile* layer0_ = nullptr;
    Tile* layer1_ = nullptr;
    // layer 0
    ogl::VertexArray* vao_ = nullptr;
    ogl::VertexBuffer vbo_;
    // layer 1
    ogl::VertexArray* vao1_ = nullptr;
    ogl::VertexBuffer vbo1_;
};