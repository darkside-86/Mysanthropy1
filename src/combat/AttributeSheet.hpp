// AttributeSheet.hpp
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

#include "Attributes.hpp"

namespace combat 
{

    // for stat calculations with levels and buffs as input
    // for now just base stat calculated logarithmically from level input
    // for now just hardcode what is core stats vs other stats until Lua data reflects it
    //  e.g. the starting player is a survivalist
    class AttributeSheet
    {
    public:
        AttributeSheet(int level, bool player);
        virtual ~AttributeSheet();

        // accessor methods for displaying stats in a character sheet, not for combat calculation
        inline int GetLevel() const { return level_; }
        inline int GetStrength() const { return strength_; }
        inline int GetAgility() const { return agility_; }
        inline int GetIntellect() const { return intellect_; }
        inline int GetDexterity() const { return dexterity_; }
        inline int GetWisdom() const { return wisdom_; }
        inline int GetKnowledge() const { return knowledge_; }
        inline int GetVitality() const { return vitality_; }

        // core and other attribute calculation function
        void SetLevel(int level);

        // derived attribute calculators
        int GetMeleeAttackPower() const;
        int GetRangedAttackPower() const;
        // TODO: GetDodgeRating
        // TODO: GetMaxMagic
        // TODO: GetMeleeCritRating
        // TODO: GetHitRating
        // TODO: GetRangedCritRating
        // TODO: GetSpellCritRating
        // TODO: GetSpellPower
        // TODO: GetMagicRegen (MP per second)
        int GetMaxHealth() const;

        // gear/buff only stats
        // TODO: GetArmorRating
        // TODO: GetMagicResistance (each school)

        // determines what is core or other stat
        bool IsCoreAttribute(const Attribute attr) const;
    private:
        // indicates the level which determines base stats
        int level_;
        // whether or not attribute sheet belongs to player
        bool player_;
        // primary attributes (built in to character sheet)
        int strength_;
        int agility_;
        int intellect_;
        int dexterity_;
        int wisdom_;
        int knowledge_;
        int vitality_;
        std::vector<Attribute> coreAttributes_;
    };

}