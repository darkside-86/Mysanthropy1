// BattleSystem.hpp
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
#include <vector>

#include "MobSprite.hpp"
#include "PlayerSprite.hpp"
#include "Target.hpp"

// determines what mobs are engaged in combat with player and runs AI to select moves
// against player
class BattleSystem
{
public:
    BattleSystem();
    virtual ~BattleSystem();
    void AddMob(MobSprite* mobSprite);
    void RemoveMob(MobSprite* mobSprite);
    void AddPlayer(PlayerSprite* playerSprite);
    // returns combat log result
    std::string UsePlayerAbility(const std::string& abilityName, Target& target);
    // returns array of combat log results
    std::vector<std::string> CalculateMoves();
private:
    std::vector<MobSprite*> mobSprites_;
    PlayerSprite* playerSprite_;
};