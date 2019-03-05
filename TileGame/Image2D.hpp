// Image2D.hpp
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

struct PIXEL
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

class Image2D
{
public:
    Image2D(int width, int height);
    virtual ~Image2D();
    void Draw(int x, int y, float scaleX, float scaleY, ogl::Program& program);
    ogl::Texture* GetTexture() { return texture_; }
    void WriteToPNG(const std::string& path);
private:
    ogl::Texture *texture_ = nullptr;
    ogl::VertexArray vao_;
    ogl::VertexBuffer vbo_;
};