// PlayerCombatUnit.cpp
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

#include "PlayerCombatUnit.hpp"

PlayerCombatUnit::PlayerCombatUnit(Configuration& config, int level, int exp, const CombatAbilityList& abilities)
    : CombatUnit(config, false, level, abilities, "player")
{
    SetLevel(level);
    currentExperience_ = exp;
}

PlayerCombatUnit::~PlayerCombatUnit()
{

}

void PlayerCombatUnit::SetLevel(int level)
{
    GetStatSheet().SetLevel(level);
    auto &config = GetStatSheet().GetConfiguration();
    float EXPERIENCE_SCALE;
    config.GetVar("EXPERIENCE_SCALE", EXPERIENCE_SCALE);
    float BASE_EXP;
    config.GetVar("BASE_EXP", BASE_EXP);
    float multiplier = pow(EXPERIENCE_SCALE, level - 1);
    maxExperience_ = (int)(multiplier * BASE_EXP);
}

bool PlayerCombatUnit::AddExperience(int exp)
{
    currentExperience_ += exp;
    int currentLevel = GetStatSheet().GetLevel();
    if(currentExperience_ >= maxExperience_)
    {
        currentExperience_ -= maxExperience_;
        SetLevel(currentLevel + 1);
        return true;
    }
    return false;
}