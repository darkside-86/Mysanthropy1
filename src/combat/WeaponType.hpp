// WeaponType.hpp
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

namespace combat
{
    // enumerates possible weapon types for equipment and ability weapon requirements
    enum class WeaponType 
    { 
        Unarmed, // includes fist weapons
        Dagger, 
        Sword1h, 
        Mace1h, 
        Sword2h, 
        Mace2h, 
        Axe2h 
    };

    WeaponType StringToWeaponType(const std::string& str);
    std::string WeaponTypeToString(WeaponType t);
}