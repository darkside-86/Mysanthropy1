// Harvesting.hpp
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

#include <vector>
#include <unordered_map>

#include "PlayerCommand.hpp"

namespace game
{
    class EntityLocationKey
    { public:
        int x=0, y=0;
        bool operator==(const EntityLocationKey& ec) const 
            { return x==ec.x && y == ec.y; }
    };
}

// needed to use EntityLocationKey as an unordered_map key
namespace std { template <> struct hash<game::EntityLocationKey> {
    // trivial hash function for two 32-bit ints, just simply store them in 64-bits
    size_t operator() (const game::EntityLocationKey& ec) const {
        return ((size_t)ec.x<<32)|ec.y;
    }
}; }

namespace game
{
    // manages harvesting and farming of static entities and buildings placed by user on the map
    class Harvesting
    {
    public:
        Harvesting();
        virtual ~Harvesting();
        // Sets a new harvest command in the list of harvest commands written to saved game
        void SetHarvestCommand(int x, int y, int clicks);
        // Sets a new farm command in the list of farm commands written to saved game
        void SetFarmCommand(int x, int y, const FarmCommand& fc);
        // Removes an individual farm command such as when a farmable is ready for pickup
        void RemoveFarmCommand(int x, int y);
        // convert harvestCommands_ to a vector of HarvestCommand and return the result
        std::vector<HarvestCommand> GetHarvestCommands();
        // convert farmCommands_ to a vector of FarmCommand and return the result
        std::vector<FarmCommand> GetFarmCommands();
        // TODO: methods for handling buildings
    private:
        // harvest commands. value is number of clicks
        std::unordered_map<EntityLocationKey, int> harvestCommands_;
        // farm commands
        std::unordered_map<EntityLocationKey, FarmCommand> farmCommands_;
    };
}
