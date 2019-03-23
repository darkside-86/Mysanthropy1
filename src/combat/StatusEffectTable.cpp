// StatusEffectTable.cpp
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

#include "engine/GameEngine.hpp"
#include "StatusEffectTable.hpp"

namespace combat
{
    StatusEffectTable::StatusEffectTable()
    {
        lua_State* L = luaL_newstate();
        // store this in registry
        lua_pushstring(L, "StatusEffectTable");
        lua_pushlightuserdata(L, this);
        lua_settable(L, LUA_REGISTRYINDEX);
        // set global function
        lua_pushcfunction(L, lua_StatusEffect);
        lua_setglobal(L, "STATUS_EFFECT");
        // read the database file
        int ok = luaL_dofile(L, "res/config/statuseffects.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "%s: Unable to read status effect database -- %s", __FUNCTION__, lua_tostring(L, -1));
            lua_pop(L, 1);
        }
        lua_close(L);
    }

    StatusEffectTable::~StatusEffectTable()
    {

    }

    const StatusEffectEntry* StatusEffectTable::GetEntry(const std::string& name) const
    {
        const auto& found = statusEffects_.find(name);
        if(found == statusEffects_.end())
            return nullptr;
        return &found->second;
    }

    // parameter: {name:string, title:string, tooltip:string, hidden:boolean
    //              tickRate:nil or 0, offensive:boolean,
    //              group:nil or string, groupUnique: boolean, maxStacks:integer, formula:string,
    //              adds: nil or {number, string...}, mults: nil or {number, string...}, 
    //              outputType:string, dispel: boolean
    int StatusEffectTable::lua_StatusEffect(lua_State* L)
    {
        // get *this
        lua_pushstring(L, "StatusEffectTable");
        lua_gettable(L, LUA_REGISTRYINDEX);
        StatusEffectTable* tbl = (StatusEffectTable*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        StatusEffectEntry entry;
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
        entry.tooltip = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        // read icon
        lua_pushstring(L, "icon");
        lua_gettable(L, 1);
        entry.icon = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        // read hidden
        lua_pushstring(L, "hidden");
        lua_gettable(L, 1);
        entry.hidden = lua_toboolean(L, -1);
        lua_pop(L, 1);
        // read tickRate (optional)
        lua_pushstring(L, "tickRate");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.tickRate = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read offensive
        lua_pushstring(L, "offensive");
        lua_gettable(L, 1);
        entry.offensive = lua_toboolean(L, -1);
        lua_pop(L, 1);
        // read group
        lua_pushstring(L, "group");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.group = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        // read groupUnique
        lua_pushstring(L, "groupUnique");
        lua_gettable(L, 1);
        entry.groupUnique = lua_toboolean(L, -1);
        lua_pop(L, 1);
        // read optional harmful
        lua_pushstring(L, "harmful");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.harmful = lua_toboolean(L, -1);
        lua_pop(L, 1);
        // read maxStacks
        lua_pushstring(L, "maxStacks");
        lua_gettable(L, 1);
        entry.maxStacks = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read formula
        lua_pushstring(L, "formula");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.formula = Formula(lua_tostring(L, -1));
        lua_pop(L, 1);
        // read adds
        lua_pushstring(L, "adds");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            for(size_t i = 1; i < lua_rawlen(L, -1); i += 2)
            {
                float lower, upper;
                lua_pushinteger(L, i);
                lua_gettable(L, -2);
                  lua_pushinteger(L, 1);
                  lua_gettable(L, -2);
                  lower = (float)lua_tonumber(L, -1);
                  lua_pop(L, 1);
                  lua_pushinteger(L, 2);
                  lua_gettable(L, -2);
                  if(!lua_isnil(L, -1))
                    upper = (float)lua_tonumber(L, -1);
                  else upper = lower;
                  lua_pop(L, 2);
                lua_pop(L, 1);
                lua_pushinteger(L, i+1);
                lua_gettable(L, -2);
                Attribute attr = StringToAttribute(lua_tostring(L, -1));
                lua_pop(L, 1);
                entry.adds.push_back({NumericRange(lower,upper), attr});
            }
        }
        lua_pop(L, 1);
        // read mults
        lua_pushstring(L, "mults");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            for(size_t i = 1; i < lua_rawlen(L, -1); i += 2)
            {
                float lower, upper;
                lua_pushinteger(L, i);
                lua_gettable(L, -2);
                  lua_pushinteger(L, 1);
                  lua_gettable(L, -2);
                  lower = (float)lua_tonumber(L, -1);
                  lua_pop(L, 1);
                  lua_pushinteger(L, 2);
                  lua_gettable(L, -2);
                  if(!lua_isnil(L, -1))
                    upper = (float)lua_tonumber(L, -1);
                  else upper = lower;
                  lua_pop(L, 2);
                lua_pop(L, 1);
                lua_pushinteger(L, i+1);
                lua_gettable(L, -2);
                Attribute attr = StringToAttribute(lua_tostring(L, -1));
                lua_pop(L, 1);
                entry.mults.push_back({NumericRange(lower,upper), attr});
            }
        }
        lua_pop(L, 1);
        // outputType
        lua_pushstring(L, "outputType");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.outputType = StringToOutputType(lua_tostring(L, -1));
        lua_pop(L, 1);
        // dispel
        lua_pushstring(L, "dispel");
        lua_gettable(L, 1);
        entry.dispel = lua_toboolean(L, -1);
        lua_pop(L, 1);

        tbl->statusEffects_[entry.name] = entry;
        return 0;
    }
}