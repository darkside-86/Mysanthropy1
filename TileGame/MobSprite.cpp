// MobSprite.cpp
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

#include "engine/GameEngine.hpp"
#include "MobSprite.hpp"

MobSprite::MobSprite(const MobType& mobType, Configuration& config, const glm::vec3& origPos) 
    : Sprite(engine::GameEngine::Get().GetTextureManager().GetTexture(
        std::string("res/textures/sprites/mobs/") + mobType.name + "_"
        + mobType.defaultAnimation), 
    mobType.width, mobType.height), aggroType_(mobType.aggroType), animSpeed_(mobType.animSpeed),
    speed_(mobType.speed), leash_(mobType.leash), originalPosition_(origPos), lootTable_(mobType.lootTable)
{
    auto& tm = engine::GameEngine::Get().GetTextureManager();
    collisionBox_ = mobType.collisionBox;
    combatUnit_ = new CombatUnit(config, false, mobType.GenerateLevel(), mobType.combatAbilityList, mobType.name);

    const std::string prefix = std::string("res/textures/sprites/mobs/") + mobType.name + "_";
    // right orientation animations
    if(mobType.rtAnimTextureList.size() > 0) // missing info = default to anim0
        AddAnimFrame("right_stand", tm.GetTexture(prefix + mobType.rtAnimTextureList[0]));
    for(const auto& each : mobType.rtAnimTextureList)
        AddAnimFrame("right_walk", tm.GetTexture(prefix + each));
    // left orientation animations
    if(mobType.lfAnimTextureList.size() > 0)
        AddAnimFrame("left_stand", tm.GetTexture(prefix + mobType.lfAnimTextureList[0]));
    for(const auto& each : mobType.lfAnimTextureList)
        AddAnimFrame("left_walk", tm.GetTexture(prefix + each));
    // front orientation animations
    if(mobType.frAnimTextureList.size() > 0)
        AddAnimFrame("front_stand", tm.GetTexture(prefix + mobType.frAnimTextureList[0]));
    for(const auto& each : mobType.frAnimTextureList)
        AddAnimFrame("front_walk", tm.GetTexture(prefix + each));
    // back orientation animations
    if(mobType.bkAnimTextureList.size() > 0)
        AddAnimFrame("back_stand", tm.GetTexture(prefix + mobType.bkAnimTextureList[0]));
    for(const auto& each : mobType.bkAnimTextureList)
        AddAnimFrame("back_walk", tm.GetTexture(prefix + each));
}

MobSprite::~MobSprite()
{
    delete combatUnit_;
}

void MobSprite::Update(float dtime)
{
    Sprite::Update(dtime);
    // set combat unit
    combatUnit_->SetLocation(position_);
    combatUnit_->Update(dtime);
    // TODO: check leash among many other things
    patrolTimer_ += dtime;
    if(patrolTimer_ > PATROL_TIME)
    {
        patrolTimer_ -= PATROL_TIME;
        auto& rng = engine::GameEngine::Get().GetRNG();
        velocity_.x = speed_;
        velocity_.y = 0.0f;
        float rAngle = (float)(rng() % 360);
        engine::GameEngine::Get().Rotate2DVector(velocity_.x, velocity_.y, rAngle);
    }
    // check animation
    if(velocity_.x < 0)
    {
        if(GetCurrentAnim() != "left_walk")
            SetCurrentAnim("left_walk", animSpeed_);
    }
    else if(velocity_.x > 0)
    {
        if(GetCurrentAnim() != "right_walk")
            SetCurrentAnim("right_walk", animSpeed_);
    }
    else if(velocity_.y < 0)
    {
        if(GetCurrentAnim() != "back_walk")
            SetCurrentAnim("back_walk", animSpeed_);
    }
    else if(velocity_.y > 0)
    {
        if(GetCurrentAnim() != "front_walk")
            SetCurrentAnim("front_walk", animSpeed_);
    } 
}