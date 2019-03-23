// Attribute.cpp
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

#include "Attribute.hpp"

namespace combat
{
    Attribute StringToAttribute(const std::string str)
    {
        if(str=="STR") return Attribute::Strength;
        else if(str == "AGI") return Attribute::Agility;
        else if(str == "INT") return Attribute::Intellect;
        else if(str == "DEX") return Attribute::Dexterity;
        else if(str == "WIS") return Attribute::Wisdom;
        else if(str == "KNO") return Attribute::Knowledge;
        else if(str == "VIT") return Attribute::Vitality;
        else if(str == "ARM") return Attribute::Armor;
        else if(str == "WPN") return Attribute::Weapon;
        else if(str == "SPD") return Attribute::Speed;
        else if(str == "LCH") return Attribute::Leech;
        else if(str == "ER") return Attribute::EarthResistance;
        else if(str == "AR") return Attribute::AirResistance;
        else if(str == "FR") return Attribute::FireResistance;
        else if(str == "GR") return Attribute::FrostResistance;
        else if(str == "SR") return Attribute::ShadowResistance;
        else if(str == "HR") return Attribute::HolyResistance;
        else if(str == "NR") return Attribute::NatureResistance;
        else if(str == "LR") return Attribute::LifeResistance;
        else if(str == "DCH") return Attribute::DodgeChance;
        else if(str == "CCH") return Attribute::CriticalChance;
        else if(str == "MAP") return Attribute::MeleeAttackPower;
        else if(str == "RAP") return Attribute::RangedAttackPower;
        else if(str == "SP") return Attribute::SpellPower;
        else if(str == "LCP") return Attribute::LeechPercent;
        else if(str == "HP") return Attribute::MaxHealth;
        else if(str == "MP") return Attribute::MaxMana;
        else if(str == "MPF") return Attribute::ManaPer5;
        else if(str == "LVL") return Attribute::Level;
        else if(str == "PAV") return Attribute::PhysicalAvoidance;
        else if(str == "SAV") return Attribute::SpellAvoidance;
        else if(str == "MOB") return Attribute::Mobility;
        else if(str == "DIS") return Attribute::Disabled;
        else if(str == "FRD") return Attribute::Feared;
        else if(str == "WW") return Attribute::WaterWalking;
        else if(str == "STL") return Attribute::Stealthed;

        else return Attribute::Level; // never reached
    }
    
    std::string AttributeToString(const Attribute attr)
    {
        switch(attr)
        {
        case Attribute::Strength:           return "STR";
        case Attribute::Agility:            return "AGI";
        case Attribute::Intellect:          return "INT";
        case Attribute::Dexterity:          return "DEX";
        case Attribute::Wisdom:             return "WIS";
        case Attribute::Knowledge:          return "KNO";
        case Attribute::Vitality:           return "VIT";
        case Attribute::Armor:              return "ARM";
        case Attribute::Weapon:             return "WPN";
        case Attribute::Speed:              return "SPD";
        case Attribute::Leech:              return "LCH";
        case Attribute::EarthResistance:    return "ER";
        case Attribute::AirResistance:      return "AR";
        case Attribute::FireResistance:     return "FR";
        case Attribute::FrostResistance:    return "GR";
        case Attribute::ShadowResistance:   return "SR";
        case Attribute::HolyResistance:     return "HR";
        case Attribute::NatureResistance:   return "NR";
        case Attribute::LifeResistance:     return "LR";
        case Attribute::DodgeChance:        return "DCH";
        case Attribute::CriticalChance:     return "CCH";
        case Attribute::MeleeAttackPower:   return "MAP";
        case Attribute::RangedAttackPower:  return "RAP";
        case Attribute::SpellPower:         return "SP";
        case Attribute::LeechPercent:       return "LCP";
        case Attribute::MaxHealth:          return "HP";
        case Attribute::MaxMana:            return "MP";
        case Attribute::ManaPer5:           return "MPF";
        case Attribute::Level:              return "LVL";
        case Attribute::PhysicalAvoidance:  return "PAV";
        case Attribute::SpellAvoidance:     return "SAV";
        case Attribute::Mobility:           return "MOB";
        case Attribute::Disabled:           return "DIS";
        case Attribute::Feared:             return "FRD";
        case Attribute::WaterWalking:       return "WW";
        case Attribute::Stealthed:          return "STL";
        }

        return "LVL"; // never reached
    }
}