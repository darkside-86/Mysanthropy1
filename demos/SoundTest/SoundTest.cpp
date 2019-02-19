// SoundTest.cpp
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

#include "SoundTest.h"

#include <vector>

#include "engine/GameEngine.h"
#include "engine/ui/Root.h"

SoundTest::SoundTest()
{

}

SoundTest::~SoundTest()
{

}

void SoundTest::Update(float dtime)
{

}

void SoundTest::Render(engine::GraphicsContext& gc)
{

}

bool SoundTest::Initialize()
{
    engine::ui::Root::Get()->Initialize();
    uiScript_ = luaL_newstate();
    luaL_openlibs(uiScript_);
    luaBindings_ = new engine::ui::LuaBindings(uiScript_);

    std::vector<const char*> CORE_UI_LIB = {
        "ui/fonts.lua", "ui/keycodes.lua", "ui/window.lua"
    };
    try 
    {
        int errCode = 0;
        for(auto luaFile : CORE_UI_LIB)
        {
            errCode = luaL_dofile(uiScript_, luaFile);
            if(errCode != 0) throw errCode;
        }
    }
    catch(int err)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "Lua error %d: %s", err, lua_tostring(uiScript_, -1));
        lua_pop(uiScript_, 1);
    }
    lua_pushstring(uiScript_, "SoundTest");
    lua_pushlightuserdata(uiScript_, this);
    lua_settable(uiScript_, LUA_REGISTRYINDEX);

    engine::GameEngine::Get().GetSoundManager().PlayMusic("res/music/test.ogg");

    return true;
}

void SoundTest::Cleanup()
{
    delete luaBindings_;
    lua_close(uiScript_);
}