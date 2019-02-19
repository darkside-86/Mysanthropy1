// LightingTutorial.h
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

#include "engine/Game.h"

#include "engine/Camera.h"
#include "ogl/Program.h"
#include "ogl/Texture.h"
#include "LampObject.h"
#include "SimpleCube.h"

class LightingTutorial : public engine::Game 
{
public:
    LightingTutorial();
    virtual ~LightingTutorial();

    bool Initialize() override;
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
    void Cleanup() override;
private:
    LampObject*     lampObject_;
    SimpleCube*     simpleCube_;
    engine::Camera* camera_;
    ogl::Texture*   texture_;
    ogl::Texture*   specular_;
    float xPos_=0.0f, yPos_=0.f, zPos_=0.0f;
};