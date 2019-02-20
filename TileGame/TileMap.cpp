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

TileMap::TileMap(TileSet* tileSet, int width, int height) 
    : tileSet_(tileSet), width_(width), height_(height)
{
    tiles_ = new Tile [width_ * height_];
}

TileMap::~TileMap()
{
    delete [] tiles_;
}

void TileMap::Draw(int x, int y, ogl::Program& program)
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
}

Tile TileMap::GetTile(int ix, int iy)
{
    int index = iy*width_ + ix;
    if(index >= width_*height_ || index < 0)
        return {0,0};
    return tiles_[index];
}

void TileMap::SetTile(int ix, int iy, const Tile& tile)
{
    int index = iy * width_ + ix;
    if(index >= width_*height_ || index < 0)
        return;
    tiles_[index] = tile;
}