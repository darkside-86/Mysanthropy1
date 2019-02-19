// GeomTutorial.h
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

#include <lua/lua.hpp>

#include "engine/ui/LuaBindings.h"
#include "ogl/Program.h"
#include "Sprite.h"

class InstanceTutorial : public engine::Game
{
public:
    InstanceTutorial();
    virtual ~InstanceTutorial();
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
    bool Initialize() override;
    void Cleanup() override;
private:
    static constexpr int NUM_SPRITES = 4096;
    lua_State* scripting_;
    engine::ui::LuaBindings* luaBindings_;
    Sprite* sprites_[NUM_SPRITES];
    ogl::Program spriteProgram_;
};