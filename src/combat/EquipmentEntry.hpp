// EquipmentEntry.hpp
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

#include "StatusEffectEntry.hpp"
#include "WeaponType.hpp"

namespace combat
{
    // defines an equippable item usually with additive attribute modification
    class EquipmentEntry
    {
    public:
        enum class Slot { Head, Neck, Shoulders, Chest, Belt, 
                          Leggings, Hands, Shoes, Ring, Trinket,
                          Weapon };
        enum class ArmorType { Accessory, Cloth, Leather, Chainmail, Plate };

        // entry in database
        std::string name;
        // whether or not more than one can be equipped at a time
        bool uniqueEquipped = false;
        // The slot the item fits
        Slot slot;
        // Type of weapon (for weapons only)
        WeaponType weaponType;
        // Type of armor (jewelry and armor only)
        ArmorType armorType;
        // The attributes that the equipment adds
        int strength = 0,
            agility = 0,
            intellect = 0,
            dexterity = 0,
            wisdom = 0,
            knowledge = 0,
            vitality = 0;
        // special gear only attributes 
        int armor = 0,
            weaponDamage = 0,
            speed = 0,
            leech = 0,
            earthResist = 0,
            airResist = 0,
            fireResist = 0,
            frostResist = 0,
            shadowResist = 0,
            holyResist = 0,
            natureResist = 0,
            lifeResist = 0;
        // can enhance further by applying buffs. entries into statuseffect db
        std::vector<std::string> statusEffects;
        // has a chance per N second to apply a status effect (proc)
        float procChance = 0.0f; // between 0 and 100
        int procFrequency = 0; // in seconds
        std::string proc; // entry into status effect db

        static Slot StringToSlot(const std::string& str)
        {
            if(str=="head") return Slot::Head;
            else if(str=="neck") return Slot::Neck;
            else if(str=="shoulders") return Slot::Shoulders;
            else if(str=="chest") return Slot::Chest;
            else if(str=="belt") return Slot::Belt;
            else if(str=="leggings") return Slot::Leggings;
            else if(str=="hands") return Slot::Hands;
            else if(str=="shoes") return Slot::Shoes;
            else if(str=="ring") return Slot::Ring;
            else if(str=="trinket") return Slot::Trinket;
            else if(str=="weapon") return Slot::Weapon;

            return Slot::Weapon; // default
        }
        static ArmorType StringToArmorType(const std::string& str)
        {
            if(str=="accessory") return ArmorType::Accessory;
            else if(str=="cloth") return ArmorType::Cloth;
            else if(str=="leather") return ArmorType::Leather;
            else if(str=="chainmail") return ArmorType::Chainmail;
            else if(str=="plate") return ArmorType::Plate;

            return ArmorType::Accessory; //default
        }   
    };
}