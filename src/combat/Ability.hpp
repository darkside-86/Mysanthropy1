// Ability.hpp
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
#include <unordered_map>

#include "Formula.hpp"

namespace combat
{
    class Ability
    { public:
        enum class CastType { Instant, Casted, Channeled };
        // convenient constructor function
        Ability(int mr, int mxr, bool of, float cd, bool ong, CastType ct, float ctm, const Formula& f)
            : minRange(mr), maxRange(mxr), offensive(of), cooldown(cd), onGCD (ong), castType(ct),
              castTime(ctm), formula(f) {}
        Ability() {}
        // the minimum distance in logical pixels for the ability to be performed on a unit
        int minRange = 0;
        // the maximum distance allowed for an ability to be performed
        int maxRange = 0;
        // whether or not ability can be used on hostile/neutral targets
        bool offensive = false;
        // cooldown in seconds
        float cooldown = 0.0f;
        // whether or not subject to GCD
        bool onGCD = false;
        // instant, or spell cast, or channeled.
        CastType castType = CastType::Instant;
        // only makes sense for casted or channeled.
        float castTime = 0.0f;
        // function that calculates the amount of damage, or negative numbers for healing
        Formula formula;
        // TODO: handle buffs/debuffs
    };

    // TODO: reimplement as vector
    typedef std::unordered_map<std::string,Ability> AbilityTable;
}