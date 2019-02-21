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
    // The tilemap takes ownership of tileSet pointer if constructed this way.
    TileMap(TileSet* tileSet, int width, int height);
    TileMap(const std::string& path);
    virtual ~TileMap();
    void Draw(int x, int y, ogl::Program& program);
    void Render(int x, int y, ogl::Program &program);
    void SaveToFile(const std::string& path);
    void LoadFromFile(const std::string& path);
    TileSet* GetTileSet() { return tileSet_; }
    Tile GetTile(int ix, int iy);
    void SetTile(int ix, int iy, const Tile& tile);
private:
    void SetupRender(); 
    TileSet* tileSet_ = nullptr;
    int width_ = 0;
    int height_ = 0;
    Tile* tiles_ = nullptr;
    ogl::VertexArray* vao_ = nullptr;
    ogl::VertexBuffer vbo_;
};