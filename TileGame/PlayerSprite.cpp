// PlayerSprite.cpp
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

#include "PlayerSprite.hpp"

PlayerSprite::PlayerSprite(ogl::Texture* texture, int w, int h, int level, int exp, bool isBoy,
    Configuration& configuration) 
        : Sprite(texture,w,h), isBoy_(isBoy)
{
    CombatAbilityList playerAbilities;
    // a simple test ability. 3rd field is false for testing. otherwise it would make
    //  no sense for the player to attack themselves.
    playerAbilities["attack"] = { 0, 32, false, 0.0f, 0.0f, true, 
        [](CombatUnit& cu) -> Damage { 
            return {Damage::DAMAGE_TYPE::PHYSICAL, cu.GetStatSheet().GetAttackPower()}; 
        }
    };
    combatUnit_ = new PlayerCombatUnit(configuration, level, exp, playerAbilities);

}

PlayerSprite::~PlayerSprite()
{
    delete combatUnit_;
}

void PlayerSprite::Update(float dtime)
{
    Sprite::Update(dtime);
    // todo: define combat location as exact center of sprite
    combatUnit_->SetLocation(position_);
    combatUnit_->Update(dtime);
}