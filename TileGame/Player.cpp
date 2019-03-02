// Player.cpp
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

#include "Player.h"

Player::Player() 
{
}

Player::~Player()
{

}

void Player::SetLevel(int level)
{
    level_ = level;
    float mult = pow(experienceScale_, (level_-1));
    maxExperience_ = (int)(mult * (float)baseExp_);
    CalculateStats();
}

int Player::ReduceHealth(int amount)
{
    health_ -= amount;
    if(health_ < 0)
    {
        int overkill = 0 - health_;
        health_ = 0;
        return overkill;
    }
    else
    {
        return 0;
    }
    
}

int Player::AddHealth(int amount)
{
    health_ += amount;
    if(health_ > maxHealth_)
    {
        int overheal = maxHealth_ - health_;
        health_ = maxHealth_;
        return overheal;
    }
    else 
    {
        return 0;
    }
}

bool Player::SetExperience(int exp)
{
    experience_ = exp;
    if(experience_ >= maxExperience_)
    {
        experience_ -= maxExperience_;
        inventory_.SetItemAmount("exp", experience_);
        SetLevel(level_ + 1);
        return true;
    }
    else
    {
        return false;
    }
}

void Player::SetBaseMaxExp(int max)
{
    baseExp_ = max;
}

void Player::CalculateStats()
{
    // TODO:
}