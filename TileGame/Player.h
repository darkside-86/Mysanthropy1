// Player.h
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
class Player
{
public:
    Player();
    virtual ~Player();

    inline int GetLevel() const { return level_; }
    void SetLevel(int level);
    inline int GetHealth() const { return health_; }
    inline int GetMaxHealth() const { return maxHealth_; }
    int ReduceHealth(int amount);
    int AddHealth(int amount);
    // returns true if user gets a level increase
    bool SetExperience(int exp);
    inline int GetExperience() { return experience_; }
    inline int GetMaxExperience() { return maxExperience_; }
    inline Inventory& GetInventory() { return inventory_; }
    inline void SetStatScale(float v) { statScale_ = v; }
    inline void SetExperienceScale(float v) { experienceScale_ = v; }
    void SetBaseMaxExp(int max); // required experience from level 1 to 2
    void CalculateStats();
private:
    int level_ = 1;
    int health_ = 1;
    int maxHealth_ = 1;
    // experience for current level
    int experience_ = 0; 
    // TODO: set level 1-2 experience from configuration
    int maxExperience_ = 500; 
    int baseExp_ = 500;
    // logarithmic multiplier for base stats
    float statScale_;
    // logarithmic multipler for experience levels
    float experienceScale_; 
    Inventory inventory_;
};