// CombatClassTable.cpp
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

#include "CombatClassTable.hpp"
#include "engine/GameEngine.hpp"

namespace combat
{
    CombatClassTable::CombatClassTable()
    {
        lua_State* L = luaL_newstate();
        // store *this
        lua_pushstring(L, "CombatClassTable");
        lua_pushlightuserdata(L, this);
        lua_settable(L, LUA_REGISTRYINDEX);
        // set globals
        lua_pushcfunction(L, lua_CombatClass);
        lua_setglobal(L, "COMBAT_CLASS");
        // read database
        int ok = luaL_dofile(L, "res/config/combatclasses.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "%s: Unable to read combat class database -- %s", __FUNCTION__, lua_tostring(L, -1));
            lua_pop(L, 1);
        }

        lua_close(L);
    }

    CombatClassTable::~CombatClassTable()
    {

    }

    const CombatClassEntry* CombatClassTable::GetEntry(std::string name) const
    {
        const auto& found = entries_.find(name);
        if(found == entries_.end())
            return nullptr;
        return &found->second;
    }

    // argument = { name:string, title:string, tooltip:string, playable:boolean, strength:number,
    //              agility:number, intellect:number, dexterity:number, wisdom:number,
    //              knowledge:number, vitality:number, abilityList: [string...]}
    int CombatClassTable::lua_CombatClass(lua_State* L)
    {
        // get *this
        lua_pushstring(L, "CombatClassTable");
        lua_gettable(L, LUA_REGISTRYINDEX);
        CombatClassTable* tbl = (CombatClassTable*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        CombatClassEntry entry;
        // read name
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        entry.name = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        // read title
        lua_pushstring(L, "title");
        lua_gettable(L, 1);
        entry.title = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        // read tooltip
        lua_pushstring(L, "tooltip");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.tooltip = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        // read playable
        lua_pushstring(L, "playable");
        lua_gettable(L, 1);
        entry.playable = lua_toboolean(L, -1);
        lua_pop(L, 1);
        // read strength
        lua_pushstring(L, "strength");
        lua_gettable(L, 1);
        entry.strength = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read agility
        lua_pushstring(L, "agility");
        lua_gettable(L, 1);
        entry.agility = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read intellect
        lua_pushstring(L, "intellect");
        lua_gettable(L, 1);
        entry.intellect = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read dexterity
        lua_pushstring(L, "dexterity");
        lua_gettable(L, 1);
        entry.dexterity = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read wisdom
        lua_pushstring(L, "wisdom");
        lua_gettable(L, 1);
        entry.wisdom = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read knowledge
        lua_pushstring(L, "knowledge");
        lua_gettable(L, 1);
        entry.knowledge = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read vitality
        lua_pushstring(L, "vitality");
        lua_gettable(L, 1);
        entry.vitality = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read abilityList
        lua_pushstring(L, "abilities");
        lua_gettable(L, 1);
        for(size_t i = 1; i <= lua_rawlen(L, -1); ++i)
        {
            lua_pushinteger(L, i);
            lua_gettable(L, -2);
            entry.abilityList.push_back(luaL_checkstring(L, -1));
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        tbl->entries_[entry.name] = entry;
        return 0;
    }
}