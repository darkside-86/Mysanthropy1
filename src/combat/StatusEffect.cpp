// StatusEffect.cpp
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

#include "StatusEffect.hpp"

namespace combat
{
    StatusEffect::StatusEffect(const StatusEffectEntry& entry, int duration, CombatUnit& source)
        : entry_(entry)
    {
        // formula is ONLY for tick healing/damage
        for(const auto& expr : entry_.formula.expressions)
        {
            Output out = expr.RunCalculation(source);
            OutputTick tick;
            tick.amount = out.amount;
            tick.type = out.outputType;
            outputTicks_.push_back(tick);
        }
        remainingDuration_ = (float)duration;
        // process multiplied attributes
        for(const auto& mult : entry_.mults)
        {
            mult_.push_back({mult.range.Next(), mult.attribute});
        }
        // process added attributes
        for(const auto& add : entry_.adds)
        {
            added_.push_back({(int)add.range.Next(), add.attribute});
        }
    }

    void StatusEffect::Update(float dtime)
    {
        tickCounter_ += dtime;
        remainingDuration_ -= dtime;
        tickFlag_ = false;
        if(remainingDuration_ >= 0.f && tickCounter_ >= (float)entry_.tickRate)
        {
            tickFlag_ = true; 
            tickCounter_ = 0.f;
        }
    }
}