// Sprite.h
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

#include "engine/GameObject.h"
#include "ogl/Texture.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"

class Sprite : public engine::GameObject
{
public:
    Sprite(ogl::Texture* img);
    virtual ~Sprite();
    virtual void Update(float dtime) override;
    virtual void Render(ogl::Program& program) override;
private:
    int width_ = 0, height_ = 0;
    ogl::Texture* anim0_;
    ogl::VertexArray vao_;
    ogl::VertexBuffer vbo_;

};