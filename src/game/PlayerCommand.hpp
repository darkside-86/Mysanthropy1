// PlayerCommand.hpp
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

#include <ctime>

namespace game 
{
    // Base class for user actions that modify the map or location of the player between savegame loads
    class PlayerCommand
    {
    public:
        // light RTTI info
        enum TYPE { HARVEST, LOCATION, FARM };

        PlayerCommand(TYPE t) : type(t) {}
        virtual ~PlayerCommand() {}
        TYPE type;
    };

    // Represents one click-harvest of a static harvestable object
    class HarvestCommand : public PlayerCommand 
    {
    public:
        HarvestCommand(int tx, int ty, int c) : PlayerCommand(TYPE::HARVEST),
            targetX(tx), targetY(ty), count(c) {}
        virtual ~HarvestCommand() {}

        int targetX, targetY, count;
    };

    // Sets the location of the player upon loading the savegame
    class LocationCommand : public PlayerCommand
    {
    public:
        LocationCommand(int x, int y) : PlayerCommand(TYPE::LOCATION), 
            locationX(x), locationY(y) {}
        virtual ~LocationCommand() {}

        int locationX, locationY;
    };

    // Represents a farming of a farmable object and stores the time of the farm action
    class FarmCommand : public PlayerCommand
    {
    public:
        FarmCommand(int x, int y, bool rtf, time_t ftime) : PlayerCommand(TYPE::FARM),
            targetX(x), targetY(y), readyToFarm(rtf), farmedTime(ftime) {}
        FarmCommand() : PlayerCommand(TYPE::FARM), targetX(0), targetY(0), readyToFarm(false),
            farmedTime(0) {}
        virtual ~FarmCommand() {}
        int targetX; int targetY; unsigned char readyToFarm; time_t farmedTime;
    };

    // TODO: building command

}