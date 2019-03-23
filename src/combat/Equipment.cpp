// Equipment.cpp
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

#include "Equipment.hpp"
#include "game/ItemTable.hpp"

namespace combat
{
    Equipment::Equipment(const EquipmentEntry& entry, int remainingDurability) 
        : equipmentEntry_(entry), remainingDurability_(remainingDurability)
    {
        
    }

    void Equipment::SetRemainingDurability(int dur)
    {
        remainingDurability_ = dur;
        if(remainingDurability_ < 0)
            remainingDurability_ = 0;
    }

    int Equipment::GetAttribute(const Attribute attr) const
    {
        switch(attr)
        {
        case Attribute::Strength: return equipmentEntry_.strength;
        case Attribute::Agility: return equipmentEntry_.agility;
        case Attribute::Intellect: return equipmentEntry_.intellect;
        case Attribute::Dexterity: return equipmentEntry_.dexterity;
        case Attribute::Wisdom: return equipmentEntry_.wisdom;
        case Attribute::Knowledge: return equipmentEntry_.knowledge;
        case Attribute::Armor: return equipmentEntry_.armor;
        case Attribute::Weapon: return equipmentEntry_.weaponDamage;
        case Attribute::Speed: return equipmentEntry_.speed;
        case Attribute::Leech: return equipmentEntry_.leech;
        case Attribute::EarthResistance: return equipmentEntry_.earthResist;
        case Attribute::AirResistance: return equipmentEntry_.airResist;
        case Attribute::FireResistance: return equipmentEntry_.fireResist;
        case Attribute::FrostResistance: return equipmentEntry_.frostResist;
        case Attribute::ShadowResistance: return equipmentEntry_.shadowResist;
        case Attribute::HolyResistance: return equipmentEntry_.holyResist;
        case Attribute::NatureResistance: return equipmentEntry_.natureResist;
        case Attribute::LifeResistance: return equipmentEntry_.lifeResist;
        default: return 0;
        }
    }
}