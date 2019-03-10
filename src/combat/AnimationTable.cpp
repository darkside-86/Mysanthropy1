// AnimationTable.cpp
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

#include "AnimationTable.hpp"
#include "engine/GameEngine.hpp"

namespace combat
{
    const AnimationTable& AnimationTable::Get()
    {
        static const AnimationTable singleton;
        return singleton;
    }

    const std::unordered_map<std::string, AnimationEntry*>& AnimationTable::GetEntries() const
    {
        return entries_;
    }

    AnimationTable::AnimationTable()
    {
        script_ = luaL_newstate();
        // store *this
        lua_pushstring(script_, "AnimationTable");
        lua_pushlightuserdata(script_, this);
        lua_settable(script_, LUA_REGISTRYINDEX);
        // set global functions
        lua_pushcfunction(script_, lua_CombatAnimation);
        lua_setglobal(script_, "COMBAT_ANIMATION");
        // load the database contents into memory by running data file
        int ok = luaL_dofile(script_, "res/config/animations.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "Failed to read combat animation database: %s", lua_tostring(script_, -1));
            lua_pop(script_, 1);
        }

        lua_close(script_);
        script_ = nullptr;
    }
    
    AnimationTable::~AnimationTable()
    {
        for(auto& pair: entries_)
        {
            delete pair.second;
        }
    }
    
    int AnimationTable::lua_CombatAnimation(lua_State* L)
    {
        // get *this
        lua_pushstring(L, "AnimationTable");
        lua_gettable(L, LUA_REGISTRYINDEX);
        AnimationTable* at = (AnimationTable*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        // argument is a table with fields: name : string, width : int, height : int,
        //  frames : array<string>, type : string, speed : float, duration : float
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        std::string name = lua_tostring(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "width");
        lua_gettable(L, 1);
        int width = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "height");
        lua_gettable(L, 1);
        int height = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "frames");
        lua_gettable(L, 1);
        std::vector<std::string> frames;
        for(int i=1; i <= lua_rawlen(L, -1); ++i)
        {
            lua_rawgeti(L, -1, i);
            frames.push_back(lua_tostring(L, -1));
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        lua_pushstring(L, "type");
        lua_gettable(L, 1);
        AnimationEntry::Type type = AnimationEntry::StringToType(lua_tostring(L, -1));
        lua_pop(L, 1);

        lua_pushstring(L, "speed");
        lua_gettable(L, 1);
        float speed = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "duration");
        lua_gettable(L, 1);
        float duration = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        if(at->entries_.count("name") == 0)
        {
            at->entries_.insert({name, new AnimationEntry(name, width, height,
                frames, type, speed, duration)});
        }
        else
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "Duplicate animation database entry `%s'", name.c_str());
        }

        return 0;
    }
}