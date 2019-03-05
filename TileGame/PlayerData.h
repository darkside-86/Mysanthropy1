// PlayerData.h
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

#include "Inventory.h"

// encapsulates player data such as stats and level
class PlayerData
{
public:
    PlayerData();
    virtual ~PlayerData();

    inline bool IsBoy() const { return isBoy_; }
    inline void SetBoy(bool b) { isBoy_ = b; }
    
    inline int GetLevel() const { return level_; }
    void SetLevel(int level);
    // returns true if user gets a level increase
    bool SetExperience(int exp);
    inline int GetExperience() const { return experience_; }
    inline int GetMaxExperience() const { return maxExperience_; }
    inline void SetStatScale(float v) { statScale_ = v; }
    inline void SetExperienceScale(float v) { experienceScale_ = v; }
    void SetBaseMaxExp(int max); // required experience from level 1 to 2
    void CalculateStats();
private:
    bool isBoy_ = true;
    // experience level - read from savedata configuration
    int level_ = 1;
    // experience for current level. Set by savegame configuration
    int experience_ = 0; 
    // experience required to gain a level. set by configuration
    int maxExperience_ = 251; 
    int baseExperience_ = 251;
    // logarithmic multiplier for base stats
    float statScale_;
    // logarithmic multipler for experience levels
    float experienceScale_; 
};