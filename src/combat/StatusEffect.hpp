// StatusEffect.hpp
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

#include "OutputType.hpp"
#include "StatusEffectEntry.hpp"

#include <vector>

namespace combat
{
    class CombatUnit;

    class StatusEffect
    {
    public:
        class OutputTick { public: int amount; OutputType type; };
        class Added { public: int amount; Attribute attribute; };
        class Multiplied { public: float coefficient; Attribute attribute; };

        StatusEffect(const StatusEffectEntry& entry, int duration, CombatUnit& source);

        void Update(float dtime);
        inline bool IsActive() const { return remainingDuration_ >= 0.f; }

        inline const std::vector<Added>& GetAdded() const { return added_; }
        inline const std::vector<Multiplied>& GetMult() const { return mult_; }
        inline const std::vector<OutputTick>& GetOutputTicks() const { return outputTicks_; }
        inline const StatusEffectEntry& GetEntry() const { return entry_; }
        inline bool TimeForTick() const { return tickFlag_; }
    private:
        // link to entry in database
        const StatusEffectEntry& entry_;
        // Remaining duration in seconds
        float remainingDuration_;
        // keeps track of when to set tick flag
        float tickCounter_ = 0;
        // the tick flag
        bool tickFlag_ = false;
        // this cannot be a numerical range but is calculated with formulas and combat units once applied
        //  this is necessary because status effects should remain even if the applier is destroyed, e.g.
        //  cases such as: an ally applies a HoT for 10 seconds but dies before the HoT expires. Or a mob
        //  applies a 1 minute curse but dies before the curse duration runs out.
        std::vector<OutputTick> outputTicks_;
        // the calculated enhancement values for buffs/debuffs
        std::vector<Added> added_;
        std::vector<Multiplied> mult_;
    };
}