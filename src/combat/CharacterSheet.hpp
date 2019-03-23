// CharacterSheet.hpp
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

#include <vector>
#include <unordered_map>

#include "Attribute.hpp"
#include "CombatClassEntry.hpp"
#include "Equipment.hpp"
#include "game/Inventory.hpp"
#include "StatusEffect.hpp"

namespace combat 
{

    // Defines a generic set of attributes
    class CharacterSheet
    {
    public:
        // ctor
        CharacterSheet(int level, bool player, const CombatClassEntry& cce);
        // dtor
        virtual ~CharacterSheet();

        // get the combat class entry
        inline const CombatClassEntry& GetCombatClassEntry() const { return combatClassEntry_; }

        // Get a raw stat according to character, gear, and buffs. Use below functions
        //  for calculating anything meaningful.
        int CalculateAttribute(Attribute attr);

        // Calculate and return max health
        int GetMaxHealth();
        // Max mana
        int GetMaxMana();
        // Calculate and return armor percentage
        float GetArmorPercent();
        // calculate and return chance to melee/range crit (0-100)
        float GetPhysicalCritChance();
        // calculate and return chance to dodge
        float GetDodgeChance();
        // calculate leech rating.
        float GetLeechPercent();
        // calculate increased or decreased speed
        float GetSpeedPercent();
        // calculate spell crit
        float GetSpellCritChance();
        // get melee ap
        int GetMeleeAttackPower();
        // get ranged ap
        int GetRangedAttackPower();
        // get spell ap/healing
        int GetSpellPower();
        // get a resistance of a damage type (physical returns 0--must use armor and dodge)
        int GetResistance(OutputType t);

        // Attempt to equip an item. For weapons, rings, or trinkets, slot1 controls which slot.
        //  For example, slot1=0 for a weapon would attempt to equip the weapon in the right hand.
        bool EquipItem(const EquipmentEntry& equipment, int durability, game::Inventory& inv, bool slot1=false);
        // Unequip an item in a slot
        bool UnequipItem(EquipmentEntry::Slot slot, game::Inventory& inv, bool slot1 = false);

        std::vector<StatusEffect*>& GetStatusEffects() { return statusEffects_; }

        // get the level
        inline int GetLevel() const { return level_; }
        // sets level value and recalculates core stats
        void SetLevel(int level);
    private:
        // combat class entry, needed to calculate primary stat values per level
        const CombatClassEntry& combatClassEntry_;
        // indicates the level which determines base stats
        int level_ = 1;
        // whether or not attribute sheet belongs to player
        bool player_ = false;
        // primary attributes (built in to character sheet)
        int strength_ = 5,
            agility_ = 5,
            intellect_ = 5,
            dexterity_ = 5,
            wisdom_ = 5,
            knowledge_ = 5,
            vitality_ = 5;
        // Equipment slots. null = no equipment
        Equipment   *headSlot_ = nullptr,
                    *neckSlot_ = nullptr,
                    *shoulderSlot_ = nullptr,
                    *chestSlot_ = nullptr,
                    *beltSlot_ = nullptr,
                    *leggingsSlot_ = nullptr,
                    *handsSlot_ = nullptr,
                    *shoesSlot_ = nullptr,
                    *ring0Slot_ = nullptr,
                    *ring1Slot_ = nullptr,
                    *trinket0Slot_ = nullptr,
                    *trinket1Slot_ = nullptr,
                    *weaponRight_ = nullptr,
                    *weaponLeft_ = nullptr; // if 2h weapon, remains null
        // list of all status effects. placed here to calculate de/buffs
        std::vector<StatusEffect*> statusEffects_;
        // cache of calculated values only needed updating if equipment is changed or level up
        std::unordered_map<Attribute,int> cachedAttributes_;
    };

}