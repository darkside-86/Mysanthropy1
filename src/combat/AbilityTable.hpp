// AbilityTables.hpp
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
#include <unordered_map>

#include <lua/lua.hpp>

#include "CombatUnit.hpp"

namespace combat
{

    // List of ability tables read from Lua file
    class AbilityTable
    {
    public:
        static const AbilityTable& Get();
        const Ability* GetAbility(const std::string& name) const;
    private:    
        AbilityTable();
        virtual ~AbilityTable();

        static int lua_Ability(lua_State* L);
        
        AbilityList allAbilities_;
    };

}