// PlayerCombatUnit.hpp
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

#include "CombatUnit.hpp"

namespace combat
{
    class PlayerCombatUnit : public CombatUnit 
    {
    public:
        PlayerCombatUnit(int level, int exp, const AbilityTable& abilities);
        virtual ~PlayerCombatUnit();
        // calculate stats for level
        void SetLevel(int level);
        // return true if a level was gained
        bool AddExperience(int exp);
        // access numbers for display
        inline int GetMaxExperience() const { return maxExperience_; }
        inline int GetCurrentExperience() const { return currentExperience_; }
        // TODO: add generic ability cooldown percentage method
        float GetRemainingRightCooldownAsValue();
        float GetRemainingLeftCooldownAsValue();
    private:
        int maxExperience_;
        int currentExperience_;
    };
}