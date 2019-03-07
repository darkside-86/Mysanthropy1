// Configuration.hpp
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

#include <string>
#include <vector>

#include <lua/lua.hpp>

#include "engine/GameEngine.hpp"
#include "Inventory.hpp"

struct LUA_ITEM_ENTRY
{
    char* name = nullptr;
    bool hidden = false;
    char* texture = nullptr;
    int foodstuff = 0;
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

    // direct var access
    template <typename T>
    void GetVar (const std::string& name, T& value) 
    {
        static_assert(false); // not handled
    }

    template <>
    void GetVar(const std::string& name, std::string& value) 
    {
        lua_getglobal(scripting_, name.c_str());
        if(lua_isnil(scripting_, -1) || !lua_isstring(scripting_, -1))
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s is not a string!", name.c_str());
            value = "";
        }
        else 
        {
            value = lua_tostring(scripting_, -1);
        }
        lua_pop(scripting_, 1);
    }

    template <>
    void GetVar(const std::string& name, int& value)
    {
        lua_getglobal(scripting_, name.c_str());
        if(lua_isnil(scripting_, -1) || !lua_isinteger(scripting_, -1))
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s is not an integer!", name.c_str());
            value = 0;
        }
        else 
        {
            value = (int)lua_tointeger(scripting_, -1);
        }
        lua_pop(scripting_, 1);
    }

    template <>
    void GetVar(const std::string& name, float& value)
    {
        lua_getglobal(scripting_, name.c_str());
        if(lua_isnil(scripting_, -1) || !lua_isnumber(scripting_, -1))
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s is not a float!", name.c_str());
            value = 0.f;
        }
        else 
        {
            value = (float)lua_tonumber(scripting_, -1);
        }
        lua_pop(scripting_, 1);
    }

    // TODO: ones for vector<int>

private:
    static int lua_ItemEntry(lua_State *L);
    std::vector<LUA_ITEM_ENTRY> luaItemEntries_;
    lua_State* scripting_ = nullptr;
};