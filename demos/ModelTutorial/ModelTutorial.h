// ModelTutorial.h
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
#include "../LightingTutorial/LampObject.h"

#include "ogl/Program.h"
#include "engine/Model.h"

using engine::Model;

class ModelTutorial : public engine::Game 
{
public:
    ModelTutorial();
    virtual ~ModelTutorial();
    bool Initialize() override;
    void Cleanup() override;
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
private:
    engine::Camera* camera_;
    LampObject*     lampObject_;
    ogl::Program*   modelProgram_;
    ogl::Program*   lightProgram_;
    Model*          model_;
};