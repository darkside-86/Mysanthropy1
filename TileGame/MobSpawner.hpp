// MobSpawner.hpp
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

#include <glm/glm.hpp>

#include "Configuration.hpp"
#include "MobSprite.hpp"

class MobSpawner
{
public:
    MobSpawner(const MobType& mt, float frequency, const glm::vec3 &pos, float percentChance=100.0f);
    virtual ~MobSpawner();
    // caller takes ownership of non-null pointer
    void Update(float dtime, MobSprite*& spawned, Configuration& config);
    MobType GetMobType() const { return mobType_; }
    inline float GetFrequency() const { return frequency_; }
    inline void SetFrequency(float f) { frequency_ = f; }
    inline float GetPercentChance() const { return percentChance_; }
    inline void SetPercentChance(float c) { percentChance_ = c; }
    inline glm::vec3 GetPosition() const { return position_; }
    inline void SetPosition(const glm::vec3& pos) { position_ = pos; }
private:
    MobType mobType_;
    float frequency_;
    float timer_ = 0.0f;
    float percentChance_;
    // position of spawner on map. determines location of mobs that are spawned
    glm::vec3 position_; 
};