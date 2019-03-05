// CombatUnit.h
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2018
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

#include <functional>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

// buffs/debuffs
class CombatEffect
{ public:
    int damagePerSecond; // negative for healing
};

// combat ability, damage, healing, or de/buffs.
class CombatAbility
{ public:
    // the minimum distance in logical pixels for the ability to be performed on a unit
    int minRange;
    // the maximum distance allowed for an ability to be performed
    int maxRange;
    // whether or not ability can be used on hostile/neutral targets
    bool offensive;
    // function that calculates the amount of damage, or negative numbers for healing
    std::function<int()> calculateBaseDamage;
    // TODO: handle buffs/debuffs
};

typedef std::unordered_map<std::string,CombatAbility> CombatAbilityList;

class CombatUnit
{
public:
    CombatUnit(bool attackable, const std::unordered_map<std::string, CombatAbility>& abilities);
    virtual ~CombatUnit();
    inline void SetLocation(const glm::vec3& loc) { location_ = loc; }
    inline void SetMaxHealth(const int h) { maxHealth_ = h; }
    // return value = excess healing (a negative number) or overkill or 0
    int UseAbility(CombatUnit& other, const std::string& abilityName);
private:
    // determines whether or not an ability is in range
    glm::vec3 location_ = {0.f, 0.f, 0.f};
    // the current health of the unit
    int currentHealth_ = 0;
    // the max health of the unit.
    int maxHealth_ = 0;
    // list of abilities that can be performed
    const std::unordered_map<std::string, CombatAbility> abilities_;
    // whether or not offensive abilities can be used against this CU
    bool attackable_;
};
