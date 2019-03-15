// Persistence.hpp
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

#include "Building.hpp"
#include "BuildingEntry.hpp"
#include "BuildingTable.hpp"
#include "PlayerCommand.hpp"

namespace game
{
    // encapsulates writing and reading a persistent state of the game to files
    class Persistence
    {
    public:
        // serializable representation of an item
        class ItemData
        { public:
            // database primary key for the associated item database entry
            std::string name;
            // the amount of the item
            int count;
            // the durability of the item in case it's equipment. otherwise will be 0
            int durability;
        };
        // serializable representation of player info (location is separate)
        struct PLAYER_DATA
        {
            // the amount of experience between current level and next level. (NOT the players entire
            //  accumulated exp, as that could easily be calculated according to the scale)
            int experience;
            // Experience level
            int level;
            // 1 for boy, 0 for girl
            int boy; 
        };
        // serializable representation of a building placed by user
        class BuildingData
        { public:
            std::string name; // primary key to building database
            int x, y; // location on map
            // chars for booleans for definitive size in file
            char harvesting=false, farming=false, crafting=false;
            Building::HarvestData harvestData;
            Building::FarmData farmData;
            Building::CraftData craftData;
        };
        // file extension of savegame file
        static constexpr char* FILE_EXT = ".sav";
        // location of savegame files.
        static constexpr char* FILE_DIR = "saves/";
        
        // ctor
        Persistence();
        // dtor
        virtual ~Persistence();
        // Adds a harvest command to the list of harvest commands.
        void AddHarvestCommand(const HarvestCommand& command);
        // Adds a farm command to the list of farm commands
        void AddFarmCommand(const FarmCommand& command);
        // Deprecated. TODO: replace this nonsense with a const& return function.
        void ForEachHarvestCommand(const std::function<void(const HarvestCommand&)>& fn);
        // Deprecated. TODO: replace this nonsense with a const& return function.
        void ForEachFarmCommand(const std::function<void(const FarmCommand&)>& fn);
        // Gets the player location command (there is only one for now)
        inline LocationCommand GetLocationCommand() { return locationCommand_; }
        // Sets the player location command
        inline void SetLocationCommand(const LocationCommand& command) 
            { locationCommand_ = command; }
        // Writes the information to a file and then clears the data. The caller will have to manually 
        //  refill data with the above functions for another write.
        void WriteToFile(const std::string& fileName);
        // Discards existing data and reads it in from a file. fileName should not specify a folder or extension
        //  as those are added to the string
        bool ReadFromFile(const std::string& fileName);
        // Gets the system time value contained in a read file. fileName should not specify a folder or extension
        //  as those are added to the string
        inline time_t GetTimeStamp() const { return timeStamp_; }
        // writes the player data object info to the Save File internal object. To use before calling WriteToFile
        void SavePlayerData(const PLAYER_DATA& data);
        // Loads data read about player
        PLAYER_DATA GetPlayerData();
        // writes an array of buildings into internal buildingdata array
        void SaveBuildingData(const std::vector<Building*>& buildings);
        // converts building data array into building sprites. Caller owns pointers
        std::vector<Building*> GenerateBuildings(const BuildingTable& db);
        // Inventory should access this to fill and read its item count data
        inline std::vector<ItemData>& GetItemData() { return items_; }
        // Discards all information read from file. Automatically called between writing and reading files
        void ClearData();

    private:
        // major version of savegame file
        static constexpr unsigned char MAJOR_VERSION = 0;
        // minor version of savegame file
        static constexpr unsigned char MINOR_VERSION = 1;
        // list of entity harvests
        std::vector<HarvestCommand> harvestCommands_;
        // list of farming occurences
        std::vector<FarmCommand> farmCommands_;
        // only one move command so far for main character location to be saved
        LocationCommand locationCommand_; 
        // items
        std::vector<ItemData> items_;
        // player info
        PLAYER_DATA playerData_;
        // buildings
        std::vector<BuildingData> buildingData_;
        // timestamp of last read save
        time_t timeStamp_;
    };

}