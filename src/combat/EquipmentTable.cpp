// EquipmentTable.cpp
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
#include "EquipmentTable.hpp"

namespace combat
{

    EquipmentTable::EquipmentTable()
    {
        lua_State* L = luaL_newstate();
        // set *this
        lua_pushstring(L, "EquipmentTable");
        lua_pushlightuserdata(L, this);
        lua_settable(L, LUA_REGISTRYINDEX);
        // set globals
        lua_pushcfunction(L, lua_Equipment);
        lua_setglobal(L, "EQUIPMENT");
        // read database file
        int ok = luaL_dofile(L, "res/config/equipment.lua");
        if(ok != LUA_OK)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "%s: Unable to read equipment database -- %s", __FUNCTION__, lua_tostring(L, -1));
            lua_pop(L, 1);
        }
        lua_close(L);
    }
        
    EquipmentTable::~EquipmentTable()
    {

    }

    const EquipmentEntry* EquipmentTable::GetEntry(const std::string& name) const
    {
        const auto& found = entries_.find(name);
        if(found == entries_.end())
            return nullptr;
        return &found->second;
    }

    // argument = {name:string, uniqueEquipped:boolean, slot:string,
    //              weaponType:nil or string, armorType:nil or string,
    //              strength:number, agility:number, intellect:number, dexterity:number, wisdom:number,
    //              knowledge:number, vitality:number, armor:number, weaponDamage:number,
    //              speed:number, leech:number, earthResist:number, airResist:number, fireResist:number,
    //              frostResist:number, shadowResist:number, holyResist:number, natureResist:number,
    //              lifeResist:number, statusEffects:nil or [string...], procChance:number, 
    //              procFrequency:number, proc:string or nil }
    int EquipmentTable::lua_Equipment(lua_State* L)
    {
        // get *this
        lua_pushstring(L, "EquipmentTable");
        lua_gettable(L, LUA_REGISTRYINDEX);
        EquipmentTable* tbl = (EquipmentTable*)lua_touserdata(L, -1);
        lua_pop(L, 1);

        EquipmentEntry entry;
        // read name
        lua_pushstring(L, "name");
        lua_gettable(L, 1);
        entry.name = luaL_checkstring(L, -1);
        lua_pop(L, 1);
        // read uniqueEquipped
        lua_pushstring(L, "uniqueEquipped");
        lua_gettable(L, 1);
        entry.uniqueEquipped = lua_toboolean(L, -1);
        lua_pop(L, 1);
        // read slot
        lua_pushstring(L, "slot");
        lua_gettable(L, 1);
        entry.slot = EquipmentEntry::StringToSlot(luaL_checkstring(L, -1));
        lua_pop(L, 1);
        // read weapontype
        lua_pushstring(L, "weaponType");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.weaponType = StringToWeaponType(luaL_checkstring(L, -1));
        lua_pop(L, 1);
        // read armortype
        lua_pushstring(L, "armorType");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.armorType = EquipmentEntry::StringToArmorType(luaL_checkstring(L, -1));
        lua_pop(L, 1);
        // read strength
        lua_pushstring(L, "strength");
        lua_gettable(L, 1);
        entry.strength = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read agility
        lua_pushstring(L, "agility");
        lua_gettable(L, 1);
        entry.agility = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read intellect
        lua_pushstring(L, "intellect");
        lua_gettable(L, 1);
        entry.intellect = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read dexterity
        lua_pushstring(L, "dexterity");
        lua_gettable(L, 1);
        entry.agility = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read wisdom
        lua_pushstring(L, "wisdom");
        lua_gettable(L, 1);
        entry.wisdom = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read knowledge
        lua_pushstring(L, "knowledge");
        lua_gettable(L, 1);
        entry.knowledge = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read vitality
        lua_pushstring(L, "vitality");
        lua_gettable(L, 1);
        entry.vitality = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read armor
        lua_pushstring(L, "armor");
        lua_gettable(L, 1);
        entry.armor = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read weaponDamage
        lua_pushstring(L, "weaponDamage");
        lua_gettable(L, 1);
        entry.weaponDamage = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read speed
        lua_pushstring(L, "speed");
        lua_gettable(L, 1);
        entry.speed = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read leech
        lua_pushstring(L, "leech");
        lua_gettable(L, 1);
        entry.leech = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read earthResist
        lua_pushstring(L, "earthResist");
        lua_gettable(L, 1);
        entry.earthResist = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read airResist
        lua_pushstring(L, "airResist");
        lua_gettable(L, 1);
        entry.airResist = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read fireResist
        lua_pushstring(L, "fireResist");
        lua_gettable(L, 1);
        entry.fireResist = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read frostResist
        lua_pushstring(L, "frostResist");
        lua_gettable(L, 1);
        entry.frostResist = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read shadowResist
        lua_pushstring(L, "shadowResist");
        lua_gettable(L, 1);
        entry.shadowResist = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read holyResist
        lua_pushstring(L, "holyResist");
        lua_gettable(L, 1);
        entry.holyResist = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read natureResist
        lua_pushstring(L, "natureResist");
        lua_gettable(L, 1);
        entry.natureResist = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read lifeResist
        lua_pushstring(L, "lifeResist");
        lua_gettable(L, 1);
        entry.lifeResist = (int)lua_tointeger(L, -1);
        lua_pop(L, 1);
        // read statusEffects
        lua_pushstring(L, "statusEffects");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1))
        {
            for(size_t i=1; i <= lua_rawlen(L, -1); ++i)
            {
                lua_pushinteger(L, i);
                lua_gettable(L, -2);
                entry.statusEffects.push_back(lua_tostring(L, -1));
                lua_pop(L, 1);
            }
        }
        lua_pop(L, 1);
        // read proc chance
        lua_pushstring(L, "procChance");
        lua_gettable(L, 1);
        entry.procChance = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read procFrequency
        lua_pushstring(L, "procFrequency");
        lua_gettable(L, 1);
        entry.procFrequency = (int)lua_tonumber(L, -1);
        lua_pop(L, 1);
        // read proc
        lua_pushstring(L, "proc");
        lua_gettable(L, 1);
        if(!lua_isnil(L, -1)) entry.proc = lua_tostring(L, -1);
        lua_pop(L, 1);

        tbl->entries_[entry.name] = entry;
        return 0;
    }

}