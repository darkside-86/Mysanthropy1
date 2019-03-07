// StatSheet.hpp
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

#include "Configuration.hpp"

// for stat calculations with levels and buffs as input
// for now just base stat calculated logarithmically from level input
// for now just hardcode what is core stats vs other stats until Lua data reflects it
//  e.g. the starting player is a survivalist
class StatSheet
{
public:
    enum STAT { STR, VIT };

    StatSheet(int level, bool player, Configuration& configuration);
    virtual ~StatSheet();

    // accessor of configuration
    inline Configuration& GetConfiguration() { return configuration_; }
    // accessor methods for displaying stats in a character sheet, not for combat calculation
    inline int GetLevel() const { return level_; }
    inline int GetStrength() const { return strength_; }
    inline int GetVitality() const { return vitality_; }

    // core stat calculation function
    void SetLevel(int level);

    // combat stat calculators
    int GetAttackPower();
    int GetMaxHealth();

    // determines what is core or other stat
    bool IsCoreStat(const STAT stat);
private:
    Configuration& configuration_;
    int level_;
    bool player_;
    int strength_;
    int vitality_;
    std::vector<STAT> coreStats_;
    std::vector<STAT> otherStats_;
};
