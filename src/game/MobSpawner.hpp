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

namespace game
{
    // An object that generates MobSprites according to a set frequency, chance, and number existing
    //  MobSprites in proximity to the spawner.
    class MobSpawner
    {
    public:
        // ctor
        MobSpawner(const MobType& mt, float frequency, const glm::vec2 &pos, float percentChance=100.0f);
        // dtor
        virtual ~MobSpawner();
        // Updates the spawn timer and has a chance of spawning a MobSprite and storing it in the spawned parameter.
        //  The caller has ownership of the pointer if it isn't set to nullptr, as in the case of no spawn occurring.
        //  TODO: Configuration is now a singleton and doesn't need to be a parameter.
        //  The existingMobs is a list of all mobs in the world so that the spawner can check their distance.
        //  TODO: The existingMobs parameter should be const&?
        void Update(float dtime, MobSprite*& spawned, Configuration& config, std::vector<MobSprite*>& existingMobs);
        // Gets the MobType object associated with mobs that are spawned by this spawner.
        MobType GetMobType() const { return mobType_; }
        // Gets the spawn rate in seconds
        inline float GetFrequency() const { return frequency_; }
        // Sets the spawn rate in seconds
        inline void SetFrequency(float f) { frequency_ = f; }
        // Gets a number between 0 and 100 indicating a percentage of chance to spawn when the timer reaches
        //  frequency. No matter the chance, mob spawning is still limited by proximity of existing mobs.
        inline float GetPercentChance() const { return percentChance_; }
        // Sets the chance 0-100 of a mob to spawn when the timer reaches...
        inline void SetPercentChance(float c) { percentChance_ = c; }
        // Gets the map position of the spawner
        inline glm::vec2 GetPosition() const { return position_; }
        // Sets the map position of the spawner
        inline void SetPosition(const glm::vec2& pos) { position_ = pos; }
    private:
        // The type of mob that is spawned by the spawner.
        MobType mobType_;
        // The frequency in seconds at which an attempt to spawn a mob is made.
        float frequency_;
        // When timer_ is greater or equal to frequency_, it is reset and there is a chance to spawn a mob
        float timer_ = 0.0f;
        // A number between 0 and 100 indicated the percentage of likelihood for a mob to spawn.
        float percentChance_;
        // position of spawner on map. determines initial location of mobs that are spawned
        glm::vec2 position_; 
    };

}