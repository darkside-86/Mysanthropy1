// AdvancedTutorial.h
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

#include <lua/lua.hpp>

#include "engine/Camera.h"
#include "engine/Game.h"
#include "engine/ui/LuaBindings.h"
#include "GrassObject.h"
#include "LightingTutorial/SimpleCube.h"
#include "ogl/FrameBuffer.h"
#include "ogl/Program.h"
#include "ScreenQuad.h"
#include "SimplePlane.h"
#include "Skybox.h"

class AdvancedTutorial : public engine::Game 
{
public:
    AdvancedTutorial();
    virtual ~AdvancedTutorial();
    bool Initialize() override;
    void Cleanup() override;
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
private:
    SimpleCube* simpleCube_;
    ogl::Program program_;
    ogl::Program colorProgram_;
    ogl::Program screenProgram_;
    ogl::Program skyboxProgram_;
    engine::Camera camera_;
    ogl::Texture* cubeTexture_;
    SimplePlane* simplePlane_;
    lua_State*  scripting_;
    engine::ui::LuaBindings* luaBindings_;
    static constexpr int NUM_GRASSES = 5;
    GrassObject* grasses_[NUM_GRASSES];
    ogl::FrameBuffer* fbo_;
    ScreenQuad* screenQuad_;
    Skybox* skybox_;
};