// PlayerSprite.cpp
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

#include "PlayerSprite.h"

PlayerSprite::PlayerSprite(ogl::Texture* texture, int w, int h) : Sprite(texture,w,h)
{
    std::unordered_map<std::string, CombatAbility> playerAbilities;
    playerAbilities["attack"] = {0, 32, true, []() { return 1; }};
    combatUnit_ = new CombatUnit(false, playerAbilities);
}

PlayerSprite::~PlayerSprite()
{
    delete combatUnit_;
}

void PlayerSprite::Update(float dtime)
{
    Sprite::Update(dtime);
    combatUnit_->SetLocation(position_);
}