// MobSpawner.cpp
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
#include "MobSpawner.hpp"

MobSpawner::MobSpawner(const MobType& mt, float frequency, const glm::vec3 &pos, float percentChance)
    : mobType_(mt), frequency_(frequency), percentChance_(percentChance), position_(pos)
{

}

MobSpawner::~MobSpawner()
{

}

// caller takes ownership of non-null pointer
void MobSpawner::Update(float dtime, MobSprite*& spawned, Configuration& config)
{
    spawned = nullptr;
    timer_ += dtime;
    if(timer_ >= frequency_)
    {
        timer_ -= frequency_;
        // attempt to spawn
        auto& rng = engine::GameEngine::Get().GetRNG();
        float roll = 100.0f * (float)rng() / (float)rng.max();
        if(roll < percentChance_)
        {
            spawned = new MobSprite(mobType_, config);
            spawned->SetPosition(position_);
        }
    }
}