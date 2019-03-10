// AbilityTables.cpp
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

#include "AbilityTables.hpp"
#include "engine/GameEngine.hpp"

namespace combat
{

    const AbilityTables& AbilityTables::Get()
    {
        static AbilityTables singleton;
        return singleton;
    }
    
    AbilityTables::AbilityTables()
    {
        script_ = luaL_newstate();
        lua_pushstring(script_, "AbilityTables");
        lua_pushlightuserdata(script_, this);
        lua_settable(script_, LUA_REGISTRYINDEX);
        lua_pushcfunction(script_, lua_Ability);
        lua_setglobal(script_, "ABILITY");
        lua_pushcfunction(script_, lua_AbilityTable);
        lua_setglobal(script_, "ABILITY_TABLE");
        int ok = luaL_dofile(script_, "res/config/abilities.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "%s: Problem parsing ability database: %s", __FUNCTION__, lua_tostring(script_, -1));
            lua_pop(script_, 1);
        }
        
        lua_close(script_);
        script_ = nullptr;
    }

    AbilityTables::~AbilityTables()
    {

    }

// ABILITY {
//     name = "basic_attack",
//     minRange = 0,
//     maxRange = 32,
//     offensive = true,
//     cooldown = 1.0,
//     onGCD = false,
//     castType = "instant",
//     castTime = 0,
//     formula = "!<(0.15,0.30)MAP,Physical;",
//     animation = "...",
//     itemCostName = "someitem",
//     itemCostCount = 1
// }
    int AbilityTables::lua_Ability(lua_State* L)
    {
        // get object
        lua_pushstring(L, "AbilityTables");
        lua_gettable(L, LUA_REGISTRYINDEX);
        AbilityTables* at = (AbilityTables*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        std::string name;
        Ability ability;

        // argument is a table. Get the fields. //

    // name
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        name = lua_tostring(L, -1);
        ability.name = name;
        lua_pop(L, 1);
    // min range
        lua_pushstring(L, "minRange");
        lua_gettable(L, 1);
        ability.minRange = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
    // max range
        lua_pushstring(L, "maxRange");
        lua_gettable(L, 1);
        ability.maxRange = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
    // offensive : boolean
        lua_pushstring(L, "offensive");
        lua_gettable(L, 1);
        ability.offensive = (bool)lua_toboolean(L, -1);
        lua_pop(L, 1);
    // cooldown : float
        lua_pushstring(L, "cooldown");
        lua_gettable(L, 1);
        ability.cooldown = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
    // onGCD : boolean
        lua_pushstring(L, "onGCD");
        lua_gettable(L, 1);
        ability.onGCD = (bool)lua_toboolean(L, -1);
        lua_pop(L, 1);
    // castType : string. Needs to be converted to enum
    // value is either "instant" -> Ability::CastType::Instant etc.
        lua_pushstring(L, "castType");
        lua_gettable(L, 1);
        std::string castType = lua_tostring(L, -1);
        lua_pop(L, 1);
        if(castType == "instant") ability.castType = Ability::CastType::Instant;
        else if(castType == "casted") ability.castType = Ability::CastType::Casted;
        else if(castType == "channeled") ability.castType = Ability::CastType::Channeled;
    // castTime : float
        lua_pushstring(L, "castTime");
        lua_gettable(L, 1);
        ability.castTime = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
    // formula : string
        lua_pushstring(L, "formula");
        lua_gettable(L, 1);
        ability.formula = Formula(lua_tostring(L, -1));
        lua_pop(L, 1);
    // level : int
        lua_pushstring(L, "level");
        lua_gettable(L, 1);
        ability.level = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
    // animation : string (optional)
        lua_pushstring(L, "animation");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
            ability.animation = lua_tostring(L, -1);
        else
            ability.animation = "";
        lua_pop(L, 1);
    // item cost name : string (optional)
        lua_pushstring(L, "itemCostName");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
            ability.itemCostName = lua_tostring(L, -1);
        else 
            ability.itemCostName = "";
        lua_pop(L, 1);
    // item cost count : int (optional)
        lua_pushstring(L, "itemCostCount");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
            ability.itemCostCount = (int)lua_tointeger(L, -1);
        else 
            ability.itemCostCount = 0;
        lua_pop(L, 1);

        at->allAbilities_[name] = ability;

        return 0;
    }

// ABILITY_TABLE {
//     name = "redcrab",
//     attacks = { "basic_attack" }
// }
    int AbilityTables::lua_AbilityTable(lua_State* L)
    {
        // get object
        lua_pushstring(L, "AbilityTables");
        lua_gettable(L, LUA_REGISTRYINDEX);
        AbilityTables* at = (AbilityTables*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        // get the lists_ entry name as field of argument
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        std::string name = lua_tostring(L, -1);
        lua_pop(L, 1);

        // get the attacks field, which is an array of string
        lua_pushstring(L, "attacks");
        lua_gettable(L, 1);
        std::vector<std::string> attackList;
        // iterate through table. elements refer to Ability_s placed into
        // allAbilities_ with the previous method.
        for(int i = 1; i <= lua_rawlen(L, -1); ++i)
        {
            lua_rawgeti(L, -1, i);
            attackList.push_back(lua_tostring(L, -1));
            lua_pop(L, 1);
        }
        lua_pop(L, 1);

        for(const auto& eachAttackName : attackList)
        {
            at->lists_[name][eachAttackName] = at->allAbilities_[eachAttackName];
        }

        return 0;
    }

}