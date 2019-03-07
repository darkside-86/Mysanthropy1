// SaveData.hpp
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
#include <functional>
#include <string>
#include <vector>

#include "Inventory.hpp"
#include "PlayerCommand.hpp"

// file structs
struct ITEM_DATA
{
    char* name;
    int count;
};

struct PLAYER_DATA
{
    int experience;
    int level;
    int boy; // 1 for boy, 0 for girl
};

// encapsulates writing and reading a persistent state of the game to files
class SaveData
{
public:
    SaveData();
    virtual ~SaveData();
    void AddHarvestCommand(const HarvestCommand& command);
    void AddFarmCommand(const FarmCommand& command);
    void ForEachHarvestCommand(const std::function<void(const HarvestCommand&)>& fn);
    void ForEachFarmCommand(const std::function<void(const FarmCommand&)>& fn);
    inline LocationCommand GetLocationCommand() { return locationCommand_; }
    inline void SetLocationCommand(const LocationCommand& command) 
        { locationCommand_ = command; }
    void WriteToFile(const std::string& fileName);
    bool ReadFromFile(const std::string& fileName);
    inline time_t GetTimeStamp() const { return timeStamp_; }
    // writes the player data object info to the Save File internal object. To use before calling WriteToFile
    void SavePlayerData(const PLAYER_DATA& data);
    // writes the inventory data to the internal save file. To be used before calling WriteToFile
    void SaveInventoryData(const Inventory& inv);
    // Loads data read about player
    PLAYER_DATA LoadPlayerData();
    // Fills in inventory information. To be called after ReadFromFile
    void LoadInventoryData(Inventory& inv);
    // Discards all information read from file. Automatically called between writing and reading files
    void ClearData();
private:
    // major version of savegame file
    static constexpr unsigned char MAJOR_VERSION = 0;
    // minor version of savegame file
    static constexpr unsigned char MINOR_VERSION = 1;
    // file extension of savegame file
    static constexpr char* FILE_EXT = ".sav";
    // location of savegame files. TODO: change this
    static constexpr char* FILE_DIR = "TileGame/saves/";
    // list of entity harvests
    std::vector<HarvestCommand> harvestCommands_;
    // list of farming occurences
    std::vector<FarmCommand> farmCommands_;
    // only one move command so far for main character location to be saved
    LocationCommand locationCommand_; 
    // items
    std::vector<ITEM_DATA> items_;
    // player info
    PLAYER_DATA playerData_;
    // timestamp of last save
    time_t timeStamp_;
};