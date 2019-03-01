// PlayerCommand.h
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2018
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

class PlayerCommand
{
public:
    enum ACTION { HARVEST, MOVE };

    PlayerCommand(ACTION a) : action(a) {}
    virtual ~PlayerCommand() {}
    ACTION action;
};

class HarvestCommand : public PlayerCommand 
{
public:
    HarvestCommand(int tx, int ty, int c) : PlayerCommand(ACTION::HARVEST),
        targetX(tx), targetY(ty), count(c) {}
    virtual ~HarvestCommand() {}

    int targetX, targetY, count;
};

class MoveCommand : public PlayerCommand
{
public:
    MoveCommand(int x, int y) : PlayerCommand(ACTION::MOVE), 
        locationX(x), locationY(y) {}
    virtual ~MoveCommand() {}
    
    int locationX, locationY;
};