// TileGame.cpp
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

#include "TileEditor.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <vector>

#include "engine/GameEngine.h"
#include "engine/ui/Root.h"
#include "ogl/ErrorCheck.h"

TileEditor::TileEditor()
{
}

TileEditor::~TileEditor()
{
}

bool TileEditor::Initialize()
{
    engine::ui::Root::Get()->Initialize();
    uiScript_ = luaL_newstate();
    luaL_openlibs(uiScript_);
    luaBindings_ = new engine::ui::LuaBindings(uiScript_);

    std::vector<const char*> CORE_UI_LIB = {
        "ui/lib/fonts.lua", "ui/lib/keycodes.lua", "ui/lib/Window.lua", "ui/TileEditor.lua"
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
    lua_pushstring(uiScript_, "TileEditor");
    lua_pushlightuserdata(uiScript_, this);
    lua_settable(uiScript_, LUA_REGISTRYINDEX);

    return true;
}

void TileEditor::Cleanup()
{
    delete luaBindings_;
    lua_close(uiScript_);
}

void TileEditor::Update(float dtime)
{
    engine::ui::Root::Get()->Update(dtime);
}

void TileEditor::Render(engine::GraphicsContext& gc)
{
    ogl::Program& program = gc.GetProgram();
    glm::mat4 projection(1.f);
    glm::mat4 view(1.f);

    program.Use();
    gc.SetUseTexture(true);
    gc.SetUseColorBlending(false);
    projection = glm::ortho(0.f, (float)engine::GameEngine::Get().GetWidth(),
            (float)engine::GameEngine::Get().GetHeight(), 0.f);
    program.SetUniform<glm::mat4>("u_projection", projection);
    program.SetUniform<glm::mat4>("u_view", view);

    engine::ui::Root::Get()->Render(gc);
}
