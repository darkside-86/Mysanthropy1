// TileGame.h
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

#include <vector>

#include <lua/lua.hpp>

#include "engine/Game.h"
#include "engine/ui/LuaBindings.h"
#include "ogl/Texture.h"
#include "TileMap.h"
#include "Sprite.h"

class TileGame : public engine::Game 
{
public:
    TileGame();
    ~TileGame();
    bool Initialize();
    void Cleanup();
    void Update(float dtime);
    void Render(engine::GraphicsContext& gc);
private:
    // load a sprite and animations from lost guardian folder
    Sprite* LoadLGSpr(const std::string& name, int w=0, int h=0);
    // unload textures associated with lost guardian sprite and destroy sprite
    void UnloadLGSpr(Sprite*& sprite, const std::string& name);
    void CleanupLoadedEntities();
    void SetupRenderList();
    void RenderSortPass();
    void RemoveSpriteFromRenderList(const Sprite* sprite);
    void SetupUIScript();
    TileMap* tileMap_;  
    Sprite* testSprite_;
    glm::vec3 camera_ = {0.f,0.f,0.f};
    // list of all sprites to try to render including player. Does not own pointers
    std::vector<Sprite*> renderList_;
    // list of loaded map entities. Owns pointers
    std::vector<Entity*> loadedEntities_;
    lua_State* uiScript_ = nullptr;
    engine::ui::LuaBindings* luaBindings_ = nullptr;
};