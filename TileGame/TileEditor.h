// TileEditor.h
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

#include "engine/Game.h"
#include "engine/ui/LuaBindings.h"
#include "Image2D.h"
#include "TileMap.h"
#include "TileSet.h"

class TileEditor : public engine::Game 
{
public:
    TileEditor();
    virtual ~TileEditor();
    bool Initialize() override;
    void Cleanup() override;
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
private:
    void SetTileToSelected(int mouseX, int mouseY);
    static int lua_SaveMap(lua_State* L);
    static int lua_LoadMap(lua_State* L);
    static int lua_NewMap(lua_State* L);
    static int lua_FillWithSelection(lua_State* L);
    static int lua_SetSelectedLayer(lua_State* L);
    static int lua_SetCollisionLayer(lua_State* L);
    lua_State*  uiScript_;
    engine::ui::LuaBindings* luaBindings_;
    TileSet*    tileSet_;
    TileMap*    tileMap_;
    int selectedIX_ = 0;
    int selectedIY_ = 0;
    int selectedLayer_ = 0;
    bool collisionLayerSelected_ = false;
    int cameraX_=0, cameraY_=0;
};