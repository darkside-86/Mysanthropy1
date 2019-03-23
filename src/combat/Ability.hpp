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
#include <vector>

#include "Formula.hpp"
#include "WeaponType.hpp"

namespace combat
{
    // Entry into ability database
    class Ability
    { public:
        // enums
        enum class CastType { Instant, Casted, Channeled };
        enum class TargetType { Enemy, Friendly, Self };
        enum class RangeType { Target, Conal, Area };

        // ctor
        Ability() {}
        // name. Redundant but super-helpful
        std::string name = "";
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
        // type of target
        TargetType targetType = TargetType::Enemy;
        // range type
        RangeType rangeType = RangeType::Target;
        // weapons required (empty list for any)
        std::vector<WeaponType> weaponsRequired;
        // function that calculates the amount of damage, or healing
        Formula formula;
        // level requirement of unit to use ability
        int level = 1;
        // associated animation
        std::string animation = "";
        // optional item and
        std::string itemCostName = "";
        //  item cost number
        int itemCostCount = 0;
        // dispel options
        std::vector<OutputType> dispels;
    };

    typedef std::unordered_map<std::string,Ability> AbilityList;
}