// Configuration.h
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

#include <string>
#include <vector>

#include <lua/lua.hpp>

#include "Inventory.h"

struct LUA_ITEM_ENTRY
{
    char* name = nullptr;
    bool hidden = false;
    char* texture = nullptr;
};

// handles configuration of game rule data
class Configuration
{
public:
    Configuration(const std::string configFilePath="TileGame/gameconfig.lua");
    virtual ~Configuration();

    float GetBasePlayerSpeed();
    void GetTileSpawnPoint(int &x, int &y);
    std::string GetBoySurvivalistSprite();
    std::string GetGirlSurvivalistSprite();
    float GetExperienceScale();
    float GetCoreStatScale();
    float GetOtherStatScale();
    float GetMobStatScale();
    int GetBaseExperience();
    void AddItemEntries(Inventory& inv);
private:
    static int lua_ItemEntry(lua_State *L);
    std::vector<LUA_ITEM_ENTRY> luaItemEntries_;
    lua_State* scripting_ = nullptr;
};