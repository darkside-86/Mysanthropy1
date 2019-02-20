// TileSet.h
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

#include "ogl/Program.h"
#include "ogl/Texture.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"

class TileSet
{
public:
    TileSet(const std::string& fpath, int tileWidth, int tileHeight);
    ~TileSet();
    void GetNumTiles(int& numx, int& numy);
    void DrawTile(int x, int y, float scaleX, float scaleY, ogl::Program& program, int ix, int iy);
    std::string GetPathToTexture() { return pathToTexture_; }
    ogl::Texture* GetTexture() { return texture_; }
    int GetTileWidth() { return tileWidth_; }
    int GetTileHeight() { return tileHeight_; }
private:
    std::string pathToTexture_;
    ogl::Texture* texture_;
    int tileWidth_;
    int tileHeight_;
    int numTilesX_ = 0;
    int numTilesY_ = 0;
    ogl::VertexArray vao_;
    ogl::VertexBuffer vbo_;
};