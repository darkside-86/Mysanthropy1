// Harvesting.cpp
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

#include "Harvesting.hpp"

namespace game
{
    Harvesting::Harvesting()
    {

    }

    Harvesting::~Harvesting()
    {

    }

    void Harvesting::SetHarvestCommand(int x, int y, int clicks)
    {
        auto found = harvestCommands_.find({x,y});
        if(found != harvestCommands_.end())
        {
            found->second = clicks;
        }
        else 
        {
            harvestCommands_[{x,y}] = clicks;
        }
    }

    void Harvesting::SetFarmCommand(int x, int y, const FarmCommand& fc)
    {
        auto found = farmCommands_.find({x,y});
        if(found != farmCommands_.end())
        {
            found->second = fc;
        }
        else
        {
            farmCommands_[{x,y}] = fc;
        }
    }

    void Harvesting::RemoveFarmCommand(int x, int y)
    {
        auto found = farmCommands_.find({x,y});
        if(found != farmCommands_.end())
            farmCommands_.erase(found);
    }
        
    std::vector<HarvestCommand> Harvesting::GetHarvestCommands()
    {
        std::vector<HarvestCommand> hc;
        for(auto it = harvestCommands_.begin(); it != harvestCommands_.end(); ++it)
        {
            hc.push_back(HarvestCommand(it->first.x, it->first.y, it->second));
        }
        return hc;
    }

    std::vector<FarmCommand> Harvesting::GetFarmCommands()
    {
        std::vector<FarmCommand> commands;
        for(auto it = farmCommands_.begin(); it != farmCommands_.end(); ++it)
        {
            commands.push_back(it->second);
        }
        return commands;
    }
}
