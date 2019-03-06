// CombatAbilityLists.cpp
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

#include "CombatAbilityLists.hpp"

const CombatAbilityLists& CombatAbilityLists::Get()
{
    static CombatAbilityLists singleton;
    return singleton;
}
   
CombatAbilityLists::CombatAbilityLists()
{
    // here construct all combat ability lists for all mobs.
    CombatAbilityList redcrab;
    redcrab["attack"] = { 0, 32, false, 0.0f, 0.0f, true, 
        [](const CombatUnit& cu) { return 1; } };
    lists_["redcrab"] = redcrab;
}

CombatAbilityLists::~CombatAbilityLists()
{

}