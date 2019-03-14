// BuildingTable.cpp
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

#include "BuildingTable.hpp"
#include "engine/GameEngine.hpp"

namespace game 
{
    BuildingTable::BuildingTable()
    {
        // create lua state to read database from lua file
        lua_State* L = luaL_newstate();
        // store this in lua registry
        lua_pushstring(L, "BuildingTable");
        lua_pushlightuserdata(L, this);
        lua_settable(L, LUA_REGISTRYINDEX);
        // set global function for building entry
        lua_pushcfunction(L, lua_Building);
        lua_setglobal(L, "BUILDING");
        // read database file
        int ok = luaL_dofile(L, "res/config/buildings.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "%s: Unable to read buildings database -- %s", __FUNCTION__, lua_tostring(L, -1));
        }
        // close lua state now that database is read into C++ data structures
        lua_close(L);
    }

    BuildingTable::~BuildingTable()
    {

    }
    
    int BuildingTable::lua_Building(lua_State* L)
    {
        // retrieve building table object from registry
        lua_pushstring(L, "BuildingTable");
        lua_gettable(L, LUA_REGISTRYINDEX);
        BuildingTable* bt = (BuildingTable*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        // the entry to be filled in.
        BuildingEntry entry;

        // name : string
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        entry.name = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        // title : string
        lua_pushstring(L, "title");
        lua_gettable(L, 1);
        entry.title = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        // tooltip : string
        lua_pushstring(L, "tooltip");
        lua_gettable(L, 1);
        entry.tooltip = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        // texture : string
        lua_pushstring(L, "texture");
        lua_gettable(L, 1);
        entry.texture = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        // width : integer
        lua_pushstring(L, "width");
        lua_gettable(L, 1);
        entry.width = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        // height : integer
        lua_pushstring(L, "height");
        lua_gettable(L, 1);
        entry.height = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        // required : [ [string,integer]... ] 
        lua_pushstring(L, "required");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            size_t i = 1;
            size_t numRequired = lua_rawlen(L, -1);
            for(; i <= numRequired; ++i)
            {
                lua_pushinteger(L, i);
                lua_gettable(L, -2);
                
                BuildingEntry::Requirement req;
                lua_pushinteger(L, 1);
                lua_gettable(L, -2);
                req.name = luaL_checkstring(L, -1);
                lua_pop(L, 1);
                lua_pushinteger(L, 2);
                lua_gettable(L, -2);
                req.count = (int)lua_tointeger(L, -1);
                lua_pop(L, 1);
                
                entry.required.push_back(req);
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);

        // collision : [integer, integer, integer, integer]
        lua_pushstring(L, "collision");
        lua_gettable(L, 1);
        lua_pushinteger(L, 1); // left
        lua_gettable(L, -2);
        entry.collision.left = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_pushinteger(L, 2); // top
        lua_gettable(L, -2);
        entry.collision.top = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_pushinteger(L, 3); // right
        lua_gettable(L, -2);
        entry.collision.right = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_pushinteger(L, 4); // bottom
        lua_gettable(L, -2);
        entry.collision.bottom = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_pop(L, 1);

        // time : integer
        lua_pushstring(L, "time");
        lua_gettable(L, 1);
        entry.time = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        // level : integer
        lua_pushstring(L, "level");
        lua_gettable(L, 1);
        entry.level = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);

        // harvesting : nil or { maxClicks : integer, time : integer, 
        //                       drops : [ [string, integer, number]... ], 
        //                       completed : nil or string }
        lua_pushstring(L, "harvesting");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            BuildingEntry::HarvestingInfo harvesting;
            lua_pushstring(L, "maxClicks");
            lua_gettable(L, -2);
            harvesting.maxClicks = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_pushstring(L, "time");
            lua_gettable(L, -2);
            harvesting.time = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_pushstring(L, "drops"); // can't be nil--no point in a harvesting field without item drops
            lua_gettable(L, -2);
            size_t i = 1;
            size_t numDrops = lua_rawlen(L, -1);
            for(; i <= numDrops; ++i)
            {
                BuildingEntry::Drop drop;
                lua_pushinteger(L, i);
                lua_gettable(L, -2);

                lua_pushinteger(L, 1);
                lua_gettable(L, -2);
                drop.item = luaL_checkstring(L, -1);
                lua_pop(L, 1);
                lua_pushinteger(L, 2);
                lua_gettable(L, -2);
                drop.count = (int)lua_tointeger(L, -1);
                lua_pop(L, 1);
                lua_pushinteger(L, 3);
                lua_gettable(L, -2);
                drop.chance = (float)lua_tonumber(L, -1);
                lua_pop(L, 1);

                harvesting.drops.push_back(drop);
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            lua_pushstring(L, "completed");
            lua_gettable(L, -2);
            if(!lua_isnil(L, -1))
                harvesting.completed = lua_tostring(L, -1);
            lua_pop(L, 1);

            entry.harvesting = new BuildingEntry::HarvestingInfo(harvesting);
        }
        lua_pop(L, 1);

        // farming : nil or { maxFarms: integer, time: integer, frequency: integer, pending: string,
        //                    drops = [ [string, integer, number]... ], completed: nil or string }
        lua_pushstring(L, "farming");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            BuildingEntry::FarmingInfo farming;
            lua_pushstring(L, "maxFarms");
            lua_gettable(L, -2);
            farming.maxFarms = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_pushstring(L, "time");
            lua_gettable(L, -2);
            farming.time = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_pushstring(L, "frequency");
            lua_gettable(L, -2);
            farming.frequency = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_pushstring(L, "drops");
            lua_gettable(L, -2);
            size_t i=1;
            size_t numDrops = lua_rawlen(L, -1);
            for(; i <= numDrops; ++i)
            {
                BuildingEntry::Drop drop;
                lua_pushinteger(L, i);
                lua_gettable(L, -2);

                lua_pushinteger(L, 1);
                lua_gettable(L, -2);
                drop.item = lua_tostring(L, -1);
                lua_pop(L, 1);
                lua_pushinteger(L, 2);
                lua_gettable(L, -2);
                drop.count = (int)lua_tointeger(L, -1);
                lua_pop(L, 1);
                lua_pushinteger(L, 3);
                lua_gettable(L, -2);
                drop.chance = (float)lua_tonumber(L, -1);
                lua_pop(L, 1);
                
                farming.drops.push_back(drop);
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            lua_pushstring(L, "completed");
            lua_gettable(L, -2);
            if(!lua_isnil(L, -1))
                farming.completed = lua_tostring(L, -1);
            lua_pop(L, 1);
            entry.farming = new BuildingEntry::FarmingInfo(farming);
        }
        lua_pop(L, 1);

        // crafting : nil or [ {sources: [ [string, integer]... ], time:integer, pending:string or nil,
        //                      completed: string or nil, results: [ [string,integer,number]... ] }... ]
        lua_pushstring(L, "crafting");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            size_t i=1;
            size_t numCrafts = lua_rawlen(L, -1);
            for(; i <= numCrafts; ++i)
            {
                lua_pushinteger(L, i);
                lua_gettable(L, -2);
                BuildingEntry::CraftingInfo crafting;

                lua_pushstring(L, "sources");
                lua_gettable(L, -2);
                if(!lua_isnil(L, -1))
                {
                    size_t j = 1;
                    size_t numSources = lua_rawlen(L, -1);
                    for(; j <= numSources; ++j)
                    {
                        lua_pushinteger(L, j);
                        lua_gettable(L, -2);
                        BuildingEntry::Requirement source;

                        lua_pushinteger(L, 1);
                        lua_gettable(L, -2);
                        source.name = luaL_checkstring(L, -1);
                        lua_pop(L, 1);
                        lua_pushinteger(L, 2);
                        lua_gettable(L, -2);
                        source.count = (int)lua_tointeger(L, -1);
                        lua_pop(L, 1);

                        crafting.sources.push_back(source);
                        lua_pop(L, 1);
                    }
                }
                lua_pop(L, 1);

                lua_pushstring(L, "time");
                lua_gettable(L, -2);
                crafting.time = (int)lua_tointeger(L, -1);
                lua_pop(L, 1);

                lua_pushstring(L, "pending");
                lua_gettable(L, -2);
                if(!lua_isnil(L, -1))
                    crafting.pending = lua_tostring(L, -1);
                lua_pop(L, 1);

                lua_pushstring(L, "completed");
                lua_gettable(L, -2);
                if(!lua_isnil(L, -1))
                    crafting.completed = lua_tostring(L, -1);
                lua_pop(L, 1);

                lua_pushstring(L, "results"); // has to be non-nil can't craft "nothing"
                lua_gettable(L, -2);
                size_t j = 1;
                size_t numResults = lua_rawlen(L, -1);
                for(; j <= numResults; ++j)
                {
                    lua_pushinteger(L, j);
                    lua_gettable(L, -2);
                    BuildingEntry::Drop result;
                    lua_pushinteger(L, 1);
                    lua_gettable(L, -2);
                    result.item = lua_tostring(L, -1);
                    lua_pop(L, 1);
                    lua_pushinteger(L, 2);
                    lua_gettable(L, -2);
                    result.count = (int)lua_tointeger(L, -1);
                    lua_pop(L, 1);
                    lua_pushinteger(L, 3);
                    lua_gettable(L, -2);
                    result.chance = (float)lua_tonumber(L, -1);
                    lua_pop(L, 1);
                    crafting.results.push_back(result);
                    lua_pop(L, 1);
                }
                lua_pop(L, 1);

                entry.crafting.push_back(crafting);
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);

        // removing : nil or { time:integer, drops:[[string,integer,number]...] }
        lua_pushstring(L, "removing");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            BuildingEntry::RemovingInfo removing;
            lua_pushstring(L, "time");
            lua_gettable(L, -2);
            removing.time = (int)lua_tointeger(L, -1);
            lua_pop(L, 1);
            lua_pushstring(L, "drops"); // can't be nil--no drop removals should be instant and not have
            lua_gettable(L, -2);        //  a removal field in the first place
            size_t i=1;
            size_t numDrops = lua_rawlen(L, -1);
            for(; i <= numDrops; ++i)
            {
                lua_pushinteger(L, i);
                lua_gettable(L, -2);
                BuildingEntry::Drop drop;
                lua_pushinteger(L, 1);
                lua_gettable(L, -2);
                drop.item = lua_tostring(L, -1);
                lua_pop(L, 1);
                lua_pushinteger(L, 2);
                lua_gettable(L, -2);
                drop.count = (int)lua_tointeger(L, -1);
                lua_pop(L, 1);
                lua_pushinteger(L, 3);
                lua_gettable(L, -2);
                drop.chance = (float)lua_tonumber(L, -1);
                lua_pop(L, 1);
                removing.drops.push_back(drop);
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
            entry.removing = new BuildingEntry::RemovingInfo(removing);
        }
        lua_pop(L, 1);

        bt->buildingEntries_.insert(std::pair<const std::string, const BuildingEntry>(
            entry.name, entry));

        return 0;
    }
}