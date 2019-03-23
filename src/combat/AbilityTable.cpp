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

#include "AbilityTable.hpp"
#include "engine/GameEngine.hpp"
#include "WeaponType.hpp"

namespace combat
{

    const AbilityTable& AbilityTable::Get()
    {
        static AbilityTable singleton;
        return singleton;
    }

    const Ability* AbilityTable::GetAbility(const std::string& name) const
    {
        const auto& found = allAbilities_.find(name);
        if(found == allAbilities_.end())
            return nullptr;
        return &found->second;
    }
    
    AbilityTable::AbilityTable()
    {
        lua_State* script = luaL_newstate();
        lua_pushstring(script, "AbilityTable");
        lua_pushlightuserdata(script, this);
        lua_settable(script, LUA_REGISTRYINDEX);
        lua_pushcfunction(script, lua_Ability);
        lua_setglobal(script, "ABILITY");
        int ok = luaL_dofile(script, "res/config/abilities.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "%s: Problem parsing ability database: %s", __FUNCTION__, lua_tostring(script, -1));
            lua_pop(script, 1);
        }
        
        lua_close(script);
    }

    AbilityTable::~AbilityTable()
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
//     targetType = "friendly" or "enemy" or "self"
//     rangeType = nil or "target" or "conal" or "area"
//     weaponsRequired = nil or {"...",...}
//     formula = "..",
//     animation = "...",
//     itemCostName = "someitem",
//     itemCostCount = 1
// }
    int AbilityTable::lua_Ability(lua_State* L)
    {
        // get object
        lua_pushstring(L, "AbilityTable");
        lua_gettable(L, LUA_REGISTRYINDEX);
        AbilityTable* at = (AbilityTable*)lua_touserdata(L, -1);
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
    //  value is either "instant" -> Ability::CastType::Instant etc.
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
    // targetType : string
        lua_pushstring(L, "targetType");
        lua_gettable(L, 1);
        std::string ttstr = luaL_checkstring(L, -1);
        if(ttstr=="friendly")
            ability.targetType = Ability::TargetType::Friendly;
        else if(ttstr=="enemy")
            ability.targetType = Ability::TargetType::Enemy;
        else 
            ability.targetType = Ability::TargetType::Self;
        lua_pop(L, 1);
    // rangeType : string
        lua_pushstring(L, "rangeType");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            std::string rangestr = luaL_checkstring(L, -1);
            if(rangestr == "target")
                ability.rangeType = Ability::RangeType::Target;
            else if(rangestr == "conal")
                ability.rangeType = Ability::RangeType::Conal;
            else if(rangestr == "area")
                ability.rangeType = Ability::RangeType::Area;
        }
        lua_pop(L, 1);
    // weaponsRequired : array
        lua_pushstring(L, "weaponsRequired");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            size_t numWpns = lua_rawlen(L, -1);
            for(size_t i=1; i <= numWpns; ++i)
            {
                lua_pushinteger(L,i);
                lua_gettable(L, -2);
                std::string wpnStr = luaL_checkstring(L, -1);
                lua_pop(L, 1);
                if(wpnStr=="unarmed") ability.weaponsRequired.push_back(WeaponType::Unarmed);
                else if(wpnStr=="dagger") ability.weaponsRequired.push_back(WeaponType::Dagger);
                else if(wpnStr=="sword1h") ability.weaponsRequired.push_back(WeaponType::Sword1h);
                else if(wpnStr=="mace1h") ability.weaponsRequired.push_back(WeaponType::Mace1h);
                else if(wpnStr=="sword2h") ability.weaponsRequired.push_back(WeaponType::Sword2h);
                else if(wpnStr=="mace2h") ability.weaponsRequired.push_back(WeaponType::Mace2h);
                else if(wpnStr=="axe2h") ability.weaponsRequired.push_back(WeaponType::Axe2h);
                else luaL_error(L, "Invalid weapont type %s", wpnStr.c_str());    
            }
        }
        lua_pop(L, 1);
    // formula : string
        lua_pushstring(L, "formula");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
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
    // read optional dispels table
        lua_pushstring(L, "dispels");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            for(size_t i = 1; i <= lua_rawlen(L, -1); ++i)
            {
                lua_pushinteger(L, i);
                lua_gettable(L, -2);
                std::string disp = luaL_checkstring(L, -1);
                lua_pop(L, 1);
                ability.dispels.push_back(StringToOutputType(disp));
            }
        }
        lua_pop(L, 1);

        at->allAbilities_[name] = ability;
        // at->allAbilities_.insert(std::pair<std::string, Ability>(name, ability));

        return 0;
    }
}