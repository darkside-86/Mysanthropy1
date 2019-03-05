// PlayerData.cpp
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

#include "PlayerData.hpp"

PlayerData::PlayerData() 
{
}

PlayerData::~PlayerData()
{

}

void PlayerData::SetLevel(int level)
{
    level_ = level;
    float mult = pow(experienceScale_, (level_-1));
    maxExperience_ = (int)(mult * (float)baseExperience_);
    CalculateStats();
}

bool PlayerData::SetExperience(int exp)
{
    experience_ = exp;
    if(experience_ >= maxExperience_)
    {
        experience_ -= maxExperience_;
        SetLevel(level_ + 1);
        return true;
    }
    else
    {
        return false;
    }
}

void PlayerData::SetBaseMaxExp(int max)
{
    baseExperience_ = max;
}

void PlayerData::CalculateStats()
{
    // TODO:
}