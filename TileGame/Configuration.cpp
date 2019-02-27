// Configuration.cpp
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

#include "Configuration.h"

#include "engine/GameEngine.h"

Configuration::Configuration(const std::string configFilePath)
{
    scripting_ = luaL_newstate();
    luaL_openlibs(scripting_);
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
