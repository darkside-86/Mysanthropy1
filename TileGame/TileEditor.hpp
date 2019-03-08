// TileEditor.hpp
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2019
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

#include "engine/Game.hpp"
#include "engine/ui/LuaBindings.hpp"
#include "Entity.hpp"
#include "Image2D.hpp"
#include "MobSpawner.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "TileSet.hpp"

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
    enum CLICK_ACTION { NONE, PLACING_ENTITY, REMOVING_ENTITY, SELECTING_ENTITY, PLACING_SPAWNER, 
        REMOVING_SPAWNER, SELECTING_SPAWNER };
    void SetTileToSelected(int mouseX, int mouseY);
    void UpdateHoverData(int mouseX, int mouseY);
    void UpdateMapName(const std::string& path);
    void SetupSelection(int index);
    void SetupSpawnPlacing(int index, float freq, float chance);
    void CleanupEntities();
    void RemoveEntity(int index, int x, int y);
    void RemoveSpawner(int index, int x, int y);
    void UpdateEntitySelection(int index);
    void UpdateSpawnerSelection(int index, float freq, float chance);
    static TileEditor* GetTileEditorObject(lua_State* L);
    static int lua_SaveMap(lua_State* L);
    static int lua_LoadMap(lua_State* L);
    static int lua_NewMap(lua_State* L);
    static int lua_FillWithSelection(lua_State* L);
    static int lua_SetSelectedLayer(lua_State* L);
    static int lua_SetCollisionLayer(lua_State* L);
    static int lua_GetScriptPath(lua_State* L);
    static int lua_SetScriptPath(lua_State* L);
    static int lua_PlaceEntity(lua_State* L);
    static int lua_RemoveEntity(lua_State* L);
    static int lua_SelectEntity(lua_State* L);
    static int lua_PlaceSpawner(lua_State* L);
    static int lua_RemoveSpawner(lua_State* L);
    static int lua_SelectSpawner(lua_State* L);
    lua_State*  uiScript_;
    engine::ui::LuaBindings* luaBindings_;
    TileSet*    tileSet_;
    TileMap*    tileMap_;
    int selectedIX_ = 0;
    int selectedIY_ = 0;
    int hoverIX_ = 0;
    int hoverIY_ = 0;
    int selectedLayer_ = 0;
    CLICK_ACTION clickAction_ = CLICK_ACTION::NONE;
    Entity* entityToPlace_ = nullptr;
    MobSpawner* mobSpawnerToPlace_ = nullptr;
    int entityID_ = 0;
    int mobSpawnerID_ = 0;
    std::vector<Entity*> entities_;
    std::vector<MobSpawner*> mobSpawners_;
    bool collisionLayerSelected_ = false;
    int cameraX_=0, cameraY_=0;
    Sprite* skullIcon_; // represents spawners
};