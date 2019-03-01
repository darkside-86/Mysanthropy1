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
    // todo: calculate all the base stats based on logarithmic scale

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

void Player::SetExperience(int exp)
{
    experience_ = exp;
    // TODO: handle when amount goes over max (Level up)
}

void Player::SetBaseMaxExp(int max)
{
    maxExperience_ = max;
    // TODO: calculate based on logarithm scale and current level
}

void Player::CalculateStats()
{

}