// MobSprite.hpp
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

#include <glm/glm.hpp>

#include "engine/GameEngine.hpp"
#include "Sprite.hpp"
#include "CombatUnit.hpp"
#include "Configuration.hpp"

class MobType
{ public:
    enum AGGRO_TYPE { HOSTILE, NEUTRAL };
    // name of combat unit
    std::string name;
    // level range
    int levelLower, levelUpper;
    // default animation frame to use for missing animations
    std::string defaultAnimation;
    // front moving animation
    std::vector<std::string> frAnimTextureList;
    // back moving animation
    std::vector<std::string> bkAnimTextureList;
    // left moving animation
    std::vector<std::string> lfAnimTextureList;
    // right moving animation
    std::vector<std::string> rtAnimTextureList;
    // animation frame change frequency
    float animSpeed;
    // size of sprite
    int width, height;
    // regular speed of the mob
    float speed;
    // distance from player at which mob "leashes" or returns to pre-aggro position
    float leash;
    // collision data
    BOX collisionBox;
    // how mob aggros
    AGGRO_TYPE aggroType;
    // list of abilities mob will be able to use
    CombatAbilityList combatAbilityList;
    // TODO: much more!!!

    int GenerateLevel() const {
        auto& rng = engine::GameEngine::Get().GetRNG();
        int range = levelUpper - levelLower + 1;
        return rng() % range + levelLower;
    }
};

class MobSprite : public Sprite 
{
public:
    // todo: Set up stat sheet upon creation
    MobSprite(const MobType& mobType, Configuration& config);
    // dtor
    virtual ~MobSprite();
    // get the combat unit of the mob
    inline CombatUnit& GetCombatUnit() { return *combatUnit_; }
    // update. 
    void Update(float dtime) override;
    // get aggro type
    inline MobType::AGGRO_TYPE GetAggroType() const { return aggroType_; }
private:
    // animation speed for each time animation is set
    float animSpeed_;
    // max velocity (without de/buffs) that mob can move
    float speed_;
    // leash distance
    float leash_;
    // patrol timer
    float patrolTimer_ = 0.0f;
    // how often to change path
    static constexpr float PATROL_TIME = 3.0f;
    // pre-aggro position
    glm::vec3 originalPosition_;
    // the combat unit
    CombatUnit* combatUnit_;
    // the aggro type
    MobType::AGGRO_TYPE aggroType_;
};