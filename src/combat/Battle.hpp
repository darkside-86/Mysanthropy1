// Battle.hpp
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

#include "AnimationSystem.hpp"
#include "game/MobSprite.hpp"
#include "game/PlayerSprite.hpp"
#include "game/Target.hpp"
#include "ogl/Program.hpp"

namespace combat
{

    // determines what mobs are engaged in combat with player and runs AI to select moves
    // against player. Also contains AnimationSystem for managing spell animations
    class Battle
    {
    public:
        // ctor
        Battle();
        // dtor
        virtual ~Battle();
        // update the animation
        inline void UpdateAnimations(float dtime) { animationSystem_.Update(dtime); }
        // render the animations
        inline void RenderAnimations(int camX, int camY, ogl::Program& program)
            { animationSystem_.Render(camX, camY, program); }
        // Adds a mob to the list of in-combat mobs if mob not already in list
        void AddMob(game::MobSprite* mobSprite);
        // Removes a mob from the combat list. 
        void RemoveMob(game::MobSprite* mobSprite);
        // Sets the player sprite
        void AddPlayer(game::PlayerSprite* playerSprite);
        // returns combat log result
        std::string UsePlayerAbility(const std::string& abilityName, game::Target& target);
        // returns array of combat log results
        std::vector<std::string> CalculateMoves();
        // pick random attack
        const Ability PickRandomAbility(CombatUnit& unit);
    private:
        // list of all mobs engaged in combat with player
        std::vector<game::MobSprite*> mobSprites_;
        // the player
        game::PlayerSprite* playerSprite_;
        // animation system
        AnimationSystem animationSystem_;
    };

}