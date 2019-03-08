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

namespace game
{
    Configuration& Configuration::Get()
    {
        static Configuration singleton;
        return singleton;
    }

    Configuration::Configuration()
    {
        // something has to be hardcoded so that the rest can be configurable
        //
        const std::string configFilePath = "res/config/gameconfig.lua";

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
    }

    int Configuration::lua_ItemEntry(lua_State *L)
    {
        // get "this" pointer
        lua_pushstring(L, "Configuration");
        lua_gettable(L, LUA_REGISTRYINDEX);
        Configuration* config = (Configuration*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        ItemEntry luaItemEntry;

        // read name field
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        luaItemEntry.name = lua_tostring(L, -1);
        lua_pop(L, 1);
        // read hidden field
        lua_pushstring(L, "hidden");
        lua_gettable(L, 1);
        if(lua_isnil(L, -1))
        {
            luaItemEntry.hidden = false; // default to false if no value listed
            // which is the default behavior of nil to boolean but here is it explicit
            // for clarity.
        }
        else
        { 
            luaItemEntry.hidden = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);
        // read texture field
        lua_pushstring(L, "texture");
        lua_gettable(L, 1);
        luaItemEntry.texture = lua_tostring(L, -1);
        lua_pop(L, 1);
        // read foodstuff field
        lua_pushstring(L, "foodstuff");
        lua_gettable(L, 1);
        luaItemEntry.foodstuff = (int)lua_tointeger(L, -1); // nil as 0 is fine and expected
        lua_pop(L, 1);

        config->itemEntries_.push_back(luaItemEntry);

        return 0;
    }

}