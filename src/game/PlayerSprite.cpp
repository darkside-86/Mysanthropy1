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

#include "combat/AbilityTables.hpp"
#include "engine/GameEngine.hpp"
#include "PlayerSprite.hpp"

namespace game
{

    PlayerSprite::PlayerSprite(ogl::Texture* texture, int w, int h, int level, int exp, bool isBoy) 
            : Sprite(texture,w,h), isBoy_(isBoy)
    {
        /*combat::AbilityTable playerAbilities;
        combat::Formula playerAttackFormula("!<(1,1.1)MAP,Physical;");
        // a simple test ability. TODO: read player abilities from a lua file. And level reqs for abilities
        playerAbilities["attack"] = combat::Ability("attack", 0, 32, 
            true, 1.0f, false, combat::Ability::CastType::Instant,
            0.0f, playerAttackFormula);*/
        
        const auto& lists = combat::AbilityTables::Get().GetLists();
        const auto& found = lists.find("player_survivalist");
        if(found == lists.end())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "Unable to locate player abilities!");
        }
        combatUnit_ = new combat::PlayerCombatUnit(level, exp, found->second);
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

}