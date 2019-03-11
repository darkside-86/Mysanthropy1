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

namespace game
{
    // handles configuration of general game rule data by reading it from a Lua file
    class Configuration
    {
    public:
        // Get singleton
        static Configuration& Get();

        // direct var access
        template <typename T>
        void GetVar (const std::string& name, T& value) 
        {
            static_assert(false); // not handled
        }

        // see above
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

        // see above
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

        // see above
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

        // reads a lua array of ints into the vector<int> paramater. the vector is cleared
        //  before any attempt to extract the Lua value.
        template <>
        void GetVar(const std::string& name, std::vector<int>& tableOfInts)
        {
            tableOfInts.clear();
            lua_getglobal(scripting_, name.c_str());
            if(lua_isnil(scripting_, -1) || !lua_istable(scripting_, -1))
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                    "%s is not a table!", name.c_str());
            }
            else 
            {
                size_t tableLen = lua_rawlen(scripting_, -1);
                for(size_t i=1; i <= tableLen; ++i)
                {
                    lua_rawgeti(scripting_, -1, i);
                    tableOfInts.push_back((int)lua_tointeger(scripting_, -1));
                    lua_pop(scripting_, 1);
                }
            }
            lua_pop(scripting_, 1);
        }

        // reads a lua array of strings into the vector<string> parameter. vector is cleared first
        template <>
        void GetVar(const std::string& name, std::vector<std::string>& tableOfStrings)
        {
            tableOfStrings.clear();
            lua_getglobal(scripting_, name.c_str());
            if(lua_isnil(scripting_, -1) || !lua_istable(scripting_, -1))
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                    "%s is not a table!", name.c_str());
            }
            else 
            {
                size_t tableLen = lua_rawlen(scripting_, -1);
                for(size_t i=1; i <= tableLen; ++i)
                {
                    lua_rawgeti(scripting_, -1, i);
                    tableOfStrings.push_back(lua_tostring(scripting_, -1));
                    lua_pop(scripting_, 1);
                }
            }
            lua_pop(scripting_, 1);
        }

    private:
        // ctor
        Configuration();
        // dtor
        virtual ~Configuration();
        // lua state that holds all the globals accessed with GetVar methods
        lua_State* scripting_ = nullptr;
    };
}