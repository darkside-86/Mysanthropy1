// Attributes.hpp
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

namespace combat
{
    // Core character sheet without equipment
    enum class Attribute 
    {
        Strength,       // melee attack power
        Agility,        // ranged attack power and dodge increase
        Intellect,      // max mana and spell crit
        Dexterity,      // (melee,ranged)(crit,hit) increase
        Wisdom,         // mana regeneration rate
        Knowledge,      // spell power increase
        Vitality        // max health
    };

    // calculated from above
    enum class DerivedAttribute
    {
        MeleeAttackPower, 
        RangedAttackPower,
        Dodge, // chance or rating? not sure yet
        MagicPoints,
        MeleeCrit,
        Hit, // chance or rating? not sure yet
        RangedCrit,
        SpellCrit,
        SpellPower,
        MagicRegen,
        HealthPoints
    };

    // special attributes from gear or buffs only
    enum class SpecialAttribute
    {
        Armor, // rating or flat physical damage reduction? not sure yet
        Speed, // increases movement speed
        Mobility, // either 1 or 0. 1=not stunned. 0=stunned.
        Luck, // affects crit, hit rates and loot drops by tiny amount
        MagicResistance,
        EarthResistance,
        AirResistannce,
        FireResistance,
        FrostResistance,
        ShadowResistance,
        HolyResistance,
        NatureResistance,
        LifeResistance,
    };
}