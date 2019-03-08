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
        inline int GetVitality() const { return vitality_; }

        // core and other attribute calculation function
        void SetLevel(int level);

        // derived attribute calculators
        int GetMeleeAttackPower();
        int GetRangedAttackPower();
        int GetMaxHealth();

        // determines what is core or other stat
        bool IsCoreAttribute(const Attribute attr);
    private:
        int level_;
        bool player_;
        int strength_;
        int agility_;
        int vitality_;
        std::vector<Attribute> coreAttributes_;
        std::vector<Attribute> otherAttributes_;
    };

}