// CombatClassEntry.hpp
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

namespace combat 
{
    // entry to database of classes for determining base stat calculation
    class CombatClassEntry
    {
    public:
        // database key
        std::string name;
        // title for UI
        std::string title;
        // tooltip for UI
        std::string tooltip;
        // whether or not player can be this class
        bool playable;
        // primary stat multipliers
        float strength;
        float agility;
        float intellect;
        float dexterity;
        float wisdom;
        float knowledge;
        float vitality;
        // list of abilities
        std::vector<std::string> abilityList;
    };
}