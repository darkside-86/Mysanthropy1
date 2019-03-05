// Configuration.cpp
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

#include "Configuration.hpp"
#include "engine/GameEngine.hpp"

Configuration::Configuration(const std::string configFilePath)
{
    scripting_ = luaL_newstate();
    luaL_openlibs(scripting_);

    // store "this" pointer in registry
    lua_pushstring(scripting_, "Configuration");
    lua_pushlightuserdata(scripting_, this);
    lua_settable(scripting_, LUA_REGISTRYINDEX);

    // global functions
    lua_pushcfunction(scripting_, Configuration::lua_ItemEntry);
    lua_setglobal(scripting_, "ITEM_ENTRY");

    // run configuration file
    int ok = luaL_dofile(scripting_, configFilePath.c_str());
    if(ok != LUA_OK)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
            "%s: %s", __FUNCTION__, lua_tostring(scripting_, -1));
        lua_pop(scripting_, 1);
    }
}

Configuration::~Configuration()
{
    lua_close(scripting_);
    for (auto each : luaItemEntries_)
    {
        delete [] each.name;
        delete [] each.texture;
    }
}

float Configuration::GetBasePlayerSpeed()
{
    lua_getglobal(scripting_, "PLAYER_SPEED");
    float speed = (float)lua_tonumber(scripting_, -1);
    lua_pop(scripting_, 1);
    return speed;
}

void Configuration::GetTileSpawnPoint(int &x, int &y)
{
    lua_getglobal(scripting_, "SPAWN_POINT");
    lua_pushinteger(scripting_, 1);
    lua_gettable(scripting_, -2);
    x = (int)lua_tonumber(scripting_, -1);
    lua_pop(scripting_, 1);
    lua_pushinteger(scripting_, 2);
    lua_gettable(scripting_, -2);
    y = (int)lua_tonumber(scripting_, -1);
    lua_pop(scripting_, 2);
}

std::string Configuration::GetBoySurvivalistSprite()
{
    lua_getglobal(scripting_, "BOY_SURV");
    std::string sprite = lua_tostring(scripting_, -1);
    lua_pop(scripting_, 1);
    return sprite;
}

std::string Configuration::GetGirlSurvivalistSprite()
{
    lua_getglobal(scripting_, "GIRL_SURV");
    std::string sprite = lua_tostring(scripting_, -1);
    lua_pop(scripting_, 1);
    return sprite;   
}

float Configuration::GetExperienceScale()
{
    lua_getglobal(scripting_, "EXPERIENCE_SCALE");
    float experienceScale = (float)lua_tonumber(scripting_, -1);
    lua_pop(scripting_,1);
    return experienceScale == 0 ? 1.33f : experienceScale;
}

int Configuration::GetBaseExperience()
{
    lua_getglobal(scripting_, "BASE_EXP");
    int baseExp = (int)lua_tonumber(scripting_, -1);
    lua_pop(scripting_,1);
    return baseExp == 0 ? 250 : baseExp;
}

float Configuration::GetCoreStatScale()
{
    lua_getglobal(scripting_, "CORESTAT_SCALE");
    float coreStatScale = (float)lua_tonumber(scripting_, -1);
    lua_pop(scripting_, 1);
    return coreStatScale == 0 ? 1.11f : coreStatScale;
}

float Configuration::GetOtherStatScale()
{
    lua_getglobal(scripting_, "OTHERSTAT_SCALE");
    float coreStatScale = (float)lua_tonumber(scripting_, -1);
    lua_pop(scripting_, 1);
    return coreStatScale == 0 ? 1.11f : coreStatScale;
}

float Configuration::GetMobStatScale()
{
    lua_getglobal(scripting_, "MOB_STAT_SCALE");
    float mobStatScale = (float)lua_tonumber(scripting_, -1);
    lua_pop(scripting_, 1);
    return mobStatScale == 0 ? 1.13f : mobStatScale;
}

void Configuration::AddItemEntries(Inventory& inv)
{
    for(auto each : luaItemEntries_)
    {
        ogl::Texture* texture = engine::GameEngine::Get().GetTextureManager().GetTexture(each.texture);
        inv.AddItemEntry(each.name, texture, each.hidden, each.foodstuff);
    }
}

int Configuration::lua_ItemEntry(lua_State *L)
{
    // get "this" pointer
    lua_pushstring(L, "Configuration");
    lua_gettable(L, LUA_REGISTRYINDEX);
    Configuration* config = (Configuration*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    LUA_ITEM_ENTRY luaItemEntry;

    // read name field
    lua_pushstring(L, "name");
    lua_gettable(L, 1);
    const char* name = lua_tostring(L, -1);
    luaItemEntry.name = new char [strlen(name)+1];
    strcpy_s(luaItemEntry.name, strlen(name)+1, name);
    lua_pop(L, 1);
    // read hidden field
    lua_pushstring(L, "hidden");
    lua_gettable(L, 1);
    if(lua_isnil(L, -1))
    {
        luaItemEntry.hidden = false; // default to false if no value listed
    }
    else
    { 
        luaItemEntry.hidden = lua_toboolean(L, -1);
    }
    lua_pop(L, 1);
    // read texture field
    lua_pushstring(L, "texture");
    lua_gettable(L, 1);
    const char* texture = lua_tostring(L, -1);
    luaItemEntry.texture = new char [strlen(texture)+1];
    strcpy_s(luaItemEntry.texture, strlen(texture)+1, texture);
    lua_pop(L, 1);
    // read foodstuff field
    lua_pushstring(L, "foodstuff");
    lua_gettable(L, 1);
    luaItemEntry.foodstuff = (int)lua_tointeger(L, -1); // nil as 0 is fine and expected
    lua_pop(L, 1);

    config->luaItemEntries_.push_back(luaItemEntry);

    return 0;
}