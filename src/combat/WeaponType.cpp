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

#include "WeaponType.hpp"

namespace combat
{
    WeaponType StringToWeaponType(const std::string& str)
    {
        if(str=="unarmed") return WeaponType::Unarmed;
        else if(str=="dagger") return WeaponType::Dagger;
        else if(str=="sword1h") return WeaponType::Sword1h;
        else if(str=="mace1h") return WeaponType::Mace1h;
        else if(str=="sword2h") return WeaponType::Sword2h;
        else if(str=="mace2h") return WeaponType::Mace2h;
        else if(str=="axe2h") return WeaponType::Axe2h;

        return WeaponType::Unarmed; // default
    }

    std::string WeaponTypeToString(WeaponType t)
    {
        switch(t)
        {
        case WeaponType::Unarmed: return "unarmed";
        case WeaponType::Dagger: return "dagger";
        case WeaponType::Sword1h: return "sword1h";
        case WeaponType::Mace1h: return "mace1h";
        case WeaponType::Sword2h: return "sword2h";
        case WeaponType::Mace2h: return "mace2h";
        case WeaponType::Axe2h: return "axe2h";
        }

        return "unarmed";
    }
}