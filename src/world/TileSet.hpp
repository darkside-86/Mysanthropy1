// TileSet.hpp
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
#pragma once

#include <string>

#include "ogl/Program.hpp"
#include "ogl/Texture.hpp"
#include "ogl/VertexArray.hpp"
#include "ogl/VertexBuffer.hpp"

namespace world
{

    // Defines a set of tiles for painting a tile map. The tiles are stored on one texture and indexed by
    // column and row
    class TileSet
    {
    public:
        // Constructs a tileset by loading the texture at fpath. The width and height of the tiles are
        // defined by tileWidth and tileHeight. If the texture's width or height are not evenly divisible
        // by the specified tile width and tile height, an error is thrown.
        TileSet(const std::string& fpath, int tileWidth, int tileHeight);
        // Destructor
        ~TileSet();
        // Returns the number of columns and rows in the set
        void GetNumTiles(int& numx, int& numy);
        // Renders an individual tile in 2D space
        void DrawTile(int x, int y, float scaleX, float scaleY, ogl::Program& program, int ix, int iy);
        // Returns file path of the texture used.
        std::string GetPathToTexture() { return pathToTexture_; }
        // Returns the texture containing all the tiles of the set
        ogl::Texture* GetTexture() { return texture_; }
        // Returns in pixels the width of each tile
        int GetTileWidth() { return tileWidth_; }
        // Returns in pixels the height of each tile
        int GetTileHeight() { return tileHeight_; }
    private:
        // File path to texture used
        std::string pathToTexture_;
        // Texture containing all the tiles
        ogl::Texture* texture_;
        // Tile width in pixels
        int tileWidth_;
        // Tile height in pixels
        int tileHeight_;
        // The number of columns of tiles
        int numTilesX_ = 0;
        // The number of rows of tiles
        int numTilesY_ = 0;
        // vertex array object for drawing individual tiles
        ogl::VertexArray vao_;
        // vertex buffer object for drawing individual tiles
        ogl::VertexBuffer vbo_;
    };

}