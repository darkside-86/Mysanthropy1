// PlayerSprite.hpp
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

#include "combat/PlayerCombatUnit.hpp"
#include "Sprite.hpp"

namespace game
{
    // Represents the player on screen. 
    class PlayerSprite : public Sprite 
    {
    public:
        // ctor. Initialize with savedata loaded values for level and exp
        PlayerSprite(ogl::Texture* texture, int w, int h, int level, int exp, bool isBoy);
        // dtor
        virtual ~PlayerSprite();
        // update
        void Update(float dtime) override;
        // returns reference to the combat unit
        inline combat::PlayerCombatUnit& GetPlayerCombatUnit() { return *combatUnit_; }
        // gets avatar
        inline bool IsBoy() const { return isBoy_; }
    private:
        // delete copy constructor and =
        PlayerSprite(const PlayerSprite&) = delete;
        // see above
        void operator=(const PlayerSprite&) = delete;
        // the player combat unit, holds experience info and manages level ups
        combat::PlayerCombatUnit* combatUnit_;
        // determines avatar of player
        bool isBoy_;
    };
}
