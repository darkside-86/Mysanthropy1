// GeomTutorial.cpp
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

#include "InstanceTutorial.h"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"
#include "engine/ui/Root.h"

InstanceTutorial::InstanceTutorial()
{
    std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/spriteshader.vs");
    std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/spriteshader.fs");
    ogl::Shader vs(GL_VERTEX_SHADER, vsSrc.c_str());
    ogl::Shader fs(GL_FRAGMENT_SHADER, fsSrc.c_str());
    spriteProgram_.CompileShaders(vs, fs);
}

InstanceTutorial::~InstanceTutorial()
{

}

void InstanceTutorial::Update(float dtime)
{
    for(int i=0; i < NUM_SPRITES; ++i)
    {
        sprites_[i]->Update(dtime);
        glm::vec3 pos = sprites_[i]->GetPosition();
        glm::vec3 vel = sprites_[i]->GetVelocity();
        if(pos.x < 0)
        {
            vel.x *= -1.0f;
            pos.x = 0;
        }
        else if(pos.x + sprites_[i]->GetWidth() > engine::GameEngine::Get().GetWidth())
        {
            vel.x *= -1.0f;
            pos.x = (float)(engine::GameEngine::Get().GetWidth() - sprites_[i]->GetWidth());
        }
            
        if(pos.y < 0)
        {
            vel.y *= -1.0f;
            pos.y = 0;
        }
        else if(pos.y + sprites_[i]->GetHeight() > engine::GameEngine::Get().GetHeight())
        {
            vel.y *= -1.0f;
            pos.y = (float)(engine::GameEngine::Get().GetHeight() - sprites_[i]->GetHeight());
        }
        sprites_[i]->SetPosition(pos);
        sprites_[i]->SetVelocity(vel);
    }
    engine::ui::Root::Get()->Update(dtime);
}

void InstanceTutorial::Render(engine::GraphicsContext& gc)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    gc.SetUseColorBlending(false);
    glClearColor(0.1f, 0.1f, 0.6f, 1.f);
    // gc.SetOrthoProjection();
    // gc.SetView(glm::mat4(1.f));
    glm::mat4 projection = glm::ortho(0.f,(float)engine::GameEngine::Get().GetWidth(), 
            (float)engine::GameEngine::Get().GetHeight(), 0.f);
    spriteProgram_.Use();
    spriteProgram_.SetUniform<glm::mat4>("u_projection", projection);
    spriteProgram_.SetUniform<glm::mat4>("u_view", glm::mat4(1.f));

    for(int i=0; i < NUM_SPRITES; ++i)
    {
        sprites_[i]->Render(spriteProgram_);
    }

    engine::ui::Root::Get()->Render(gc);
}

bool InstanceTutorial::Initialize()
{
    engine::ui::Root::Get()->Initialize();
    scripting_ = luaL_newstate();
    luaL_openlibs(scripting_);
    luaBindings_ = new engine::ui::LuaBindings(scripting_);

    std::vector<const char*> CORE_UI_LIB = {
        "ui/fonts.lua", "ui/keycodes.lua", "ui/window.lua", "ui/instancetutorial.lua"
    };
    try 
    {
        int errCode = 0;
        for(auto luaFile : CORE_UI_LIB)
        {
            errCode = luaL_dofile(scripting_, luaFile);
            if(errCode != 0) throw errCode;
        }
    }
    catch(int err)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "Lua error %d: %s", err, lua_tostring(scripting_, -1));
        lua_pop(scripting_, 1);
    }
    lua_pushstring(scripting_, "AdvancedTutorial");
    lua_pushlightuserdata(scripting_, this);
    lua_settable(scripting_, LUA_REGISTRYINDEX);

    ogl::Texture* spriteTexture = engine::GameEngine::Get().GetTextureManager().GetTexture("res/textures/sprite.png");
    if(spriteTexture == nullptr)
        return false;
    auto& rng = engine::GameEngine::Get().GetRNG();
    for(int i=0; i < NUM_SPRITES; ++i)
    {
        sprites_[i] = new Sprite(spriteTexture, 32, 32);
        sprites_[i]->SetPosition({0.0f,0.0f,0.0f});
        sprites_[i]->SetVelocity({rng() % 75, rng() % 75, 0.f});
    }
    return true;
}

void InstanceTutorial::Cleanup()
{
    delete luaBindings_;
    lua_close(scripting_);
    for(int i=0; i < NUM_SPRITES; ++i)
    {
        delete sprites_[i];
    }
}