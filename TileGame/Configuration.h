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

#include <lua/lua.hpp>

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
private:
    lua_State* scripting_ = nullptr;
};