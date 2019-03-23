// Attribute.hpp
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
    // all attributes. Enum'd together for the purpose of combat number calculations
    enum class Attribute
    {
        // Character sheet built-ins //////////////////////////////////////////

        // Contributes to melee attack power. 1 MAP is added per 1 STR
        Strength,
        // Contributes to ranged attack power. 1 RAP is added per 1 AGI
        Agility,
        // Increases max MP and spell crit. Spell crit chance is calculated as 
        //  INT = (0.85^(LVL-1))% chance. INT also provides 5 MP max per point.
        Intellect,      
        // Melee/Ranged crit DEX=(0.85^(LVL-1))% and DEX=(0.8^(LVL-1))% dodge
        Dexterity,      
        // Increase mana regen by WIS = 1 Mana Per 5 seconds.
        Wisdom,
        // Increases spell power. KNO = 1 SP         
        Knowledge,
        // Increases health. VIT = (5 Max HP) + ((1-1) * LVL)
        Vitality,        

        // Equipment stats ////////////////////////////////////////////////////
        // -- can include above also:

        // Percentage of physical damage prevented. ARM = (0.85^(LVL-1))% physical dmg reduction
        Armor,
        // Increases Weapon damage
        Weapon,
        // Increases max speed by (0.91^(LVL-1)%
        Speed,
        // Percent of damage done returned as health to attacker. (0.91^(LVL-1))
        Leech,
        // Decreases earth damage taken by (0.85^(LVL-1))% and reducing earth healing by 1 per
        EarthResistance,
        // Decreases air damage taken by (0.85^(LVL-1))% and reducing air healing by 1 per
        AirResistance,
        // Decreases fire damage taken by (0.85^(LVL-1))% and reducing fire healing by 1 per
        FireResistance,
        // Decreases frost damage taken by (0.85^(LVL-1))% and reducing frost healing by 1 per
        FrostResistance,
        // Decreases shadow damage taken by (0.85^(LVL-1))% and reducing shadow healing by 1 per
        ShadowResistance,
        // Decreases holy damage taken by (0.85^(LVL-1))% and reducing holy healing by 1 per
        HolyResistance,
        // Decreases nature damage taken by (0.85^(LVL-1))% and reducing nature healing by 1 per
        NatureResistance,
        // Decreases life damage taken by (0.85^(LVL-1))% and reducing life healing by 1 per
        LifeResistance,

        // Derived stats //////////////////////////////////////////////////////
        //--needed because primary stats actually do slightly more than what is described and
        //-- these modifications can ignore formula calculations that derive from stat coefficients
        
        // direct percentage of dodge chance (between 0 and 100)
        DodgeChance,
        // chance to crit
        CriticalChance,
        // melee attack power
        MeleeAttackPower,
        // ranged attack power
        RangedAttackPower,
        // spell attack power
        SpellPower,
        // Leech percent
        LeechPercent,
        // Maximum health
        MaxHealth,
        // maximum mana
        MaxMana,
        // mp5
        ManaPer5,
        // Level
        Level,

        // Status effects /////////////////////////////////////////////////////
        //--can affect all of the above also:

        // Percentage of physical damage completely mitigated
        PhysicalAvoidance,
        // Percentage of all magic damage completely mitigated
        SpellAvoidance,
        // 1 for not stunned, 0 for stunned
        Mobility,
        // 1 for stun CC'd, 0 for not CC'd
        Disabled,
        // 1 for not feared, 0 for feared
        Feared,
        // 1 for water walking, 0 for not
        WaterWalking,
        // 1 for stealthing, 0 for not
        Stealthed,
    };

    
    // converts 3 letter all caps representation of an attribute to equivalent enum
    Attribute StringToAttribute(const std::string str);
    // does the reverse of the above
    std::string AttributeToString(const Attribute attr);

}