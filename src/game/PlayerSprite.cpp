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

#include "combat/CombatClassTable.hpp"
#include "engine/GameEngine.hpp"
#include "PlayerSprite.hpp"

namespace game
{

    PlayerSprite::PlayerSprite(ogl::Texture* texture, int w, int h, int level, int exp, bool isBoy) 
            : Sprite(texture,w,h), isBoy_(isBoy)
    {      
        const combat::CombatClassEntry* classEntry = combat::CombatClassTable::Get().GetEntry("survivalist");
        if(classEntry == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                "Failed to create player due to missing combat class!");
            return;
        }
        combatUnit_ = new combat::PlayerCombatUnit(level, exp, *classEntry);
    }

    PlayerSprite::~PlayerSprite()
    {
        delete combatUnit_;
    }

    void PlayerSprite::Update(float dtime)
    {
        Sprite::Update(dtime);
        combatUnit_->SetLocation({
            position.x + (float)width_ / 2.f,
            position.y + (float)height_ / 2.f
        });
        combatUnit_->Update(dtime);
    }

}