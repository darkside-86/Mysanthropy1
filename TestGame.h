// TestGame.h
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
#include "engine/ui/Button.h"
#include "engine/ui/Frame.h"
#include "engine/ui/Label.h"
#include "engine/ui/LuaBindings.h"
#include "engine/ui/Root.h"
#include "engine/ui/Slider.h"
#include "engine/ui/TextField.h"
#include "ogl/Texture.h"

#include "TestObject.h"

class TestGame : public engine::Game
{
public:
    TestGame();
    virtual ~TestGame();
    bool Initialize() override;
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
    void Cleanup() override;
    TestObject* GetTestObject() { return testObject_; }

    static int SetR(lua_State* L);
    static int SetG(lua_State* L);
    static int SetB(lua_State* L);
    static int SetA(lua_State* L);
    static int SetAngle(lua_State* L);
private:
    void RandomizeRectColors();
    engine::ui::Root* uiRoot_;
    engine::ui::Frame* frame_;
    engine::ui::Frame* blueButton_;
    engine::ui::Label* helloLabel_;
    engine::ui::Button* closeButton_;
    engine::ui::Slider* slider_;
    engine::ui::TextField* textField_;
    engine::ui::LuaBindings* luaBindings_;
    ogl::Texture groundTexture;
    ogl::Texture frameTexture;
    TestObject* testObject_;
    lua_State* scripting_ = nullptr;
    float angle = 0.f;
};