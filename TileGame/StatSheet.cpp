// StatSheet.cpp
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

#include "StatSheet.hpp"

StatSheet::StatSheet(int level, bool player, Configuration& configuration) 
    : configuration_(configuration), level_(level), player_(player)
{
    coreStats_.push_back(STAT::STR);
    coreStats_.push_back(STAT::VIT);
    SetLevel(level);
}

StatSheet::~StatSheet()
{

}

void StatSheet::SetLevel(int level)
{
    level_ = level;

    int CORE_STATS; // set to 5 in config file
    configuration_.GetVar("CORE_STATS", CORE_STATS);
    if(player_)
    {
        int OTHER_STATS; // also set to 5 in config for now
        configuration_.GetVar("OTHER_STATS", OTHER_STATS);
        float CORESTAT_SCALE;
        configuration_.GetVar("CORESTAT_SCALE", CORESTAT_SCALE);
        float OTHERSTAT_SCALE;
        configuration_.GetVar("OTHERSTAT_SCALE", OTHERSTAT_SCALE);

        float coreMultiplier = pow(CORESTAT_SCALE, level - 1);
        float otherMultiplier = pow(OTHERSTAT_SCALE, level - 1);
        // calculate strength
        float str;
        if(IsCoreStat(STAT::STR))
            str = coreMultiplier * CORE_STATS;
        else 
            str = otherMultiplier * OTHER_STATS;
        strength_ = (int)str;
        // calculate vitality
        float vit;
        if(IsCoreStat(STAT::VIT))
            vit = coreMultiplier * CORE_STATS;
        else 
            vit = otherMultiplier * OTHER_STATS;
        vitality_ = (int)vit;
    }
    else 
    {
        float MOB_STAT_SCALE;
        configuration_.GetVar("MOB_STAT_SCALE", MOB_STAT_SCALE);
        float mobMultiplier = pow(MOB_STAT_SCALE, level - 1);
        strength_ = (int)(mobMultiplier * CORE_STATS);
        vitality_ = (int)(mobMultiplier * CORE_STATS);
    }
}

int StatSheet::GetAttackPower()
{
    return strength_ + (level_ - 1); // for now
}

int StatSheet::GetMaxHealth()
{
    return 5 * vitality_ + (level_ - 1); // for now
}

bool StatSheet::IsCoreStat(const STAT stat)
{
    for(int i=0; i < coreStats_.size(); ++i)
    {
        if(coreStats_[i] == stat)
            return true;
    }
    return false;
}