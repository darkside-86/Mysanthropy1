// SaveData.h
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

#include <ctime>
#include <functional>
#include <string>
#include <vector>

#include "Inventory.h"
#include "Player.h"
#include "PlayerCommand.h"

// encapsulates writing and reading a persistent state of the game to files
class SaveData
{
public:
    SaveData(Inventory& inv, Player& player);
    virtual ~SaveData();
    void AddHarvestCommand(const HarvestCommand& command);
    void AddFarmCommand(const FarmCommand& command);
    void ForEachHarvestCommand(const std::function<void(const HarvestCommand&)>& fn);
    void ForEachFarmCommand(const std::function<void(const FarmCommand&)>& fn);
    inline MoveCommand GetMoveCommand() { return moveCommand_; }
    inline void SetMoveCommand(const MoveCommand& command) 
        { moveCommand_ = command; }
    void WriteToFile(const std::string& fileName);
    bool ReadFromFile(const std::string& fileName);
    inline time_t GetTimeStamp() const { return timeStamp_; }
private:
    void Cleanup();
    // major version of savegame file
    static constexpr unsigned char MAJOR_VERSION = 0;
    // minor version of savegame file
    static constexpr unsigned char MINOR_VERSION = 3;
    // file extension of savegame file
    static constexpr char* FILE_EXT = ".sav";
    // location of savegame files
    static constexpr char* FILE_DIR = "TileGame/saves/";

    // list of entity harvests
    std::vector<HarvestCommand> harvestCommands_;
    // list of farming occurences
    std::vector<FarmCommand> farmCommands_;
    // only one move command so far for main character location to be saved
    MoveCommand moveCommand_; 
    // Inventory data
    Inventory& inventory_;
    // Player data (just the main character for now)
    Player& player_;
    // timestamp of last save
    time_t timeStamp_;
};