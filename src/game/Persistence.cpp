// Persistence.cpp
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

#include <fstream>

#include "engine/GameEngine.hpp"
#include "Persistence.hpp"

namespace game
{

    Persistence::Persistence() : locationCommand_(0,0)
    {

    }

    Persistence::~Persistence()
    {
    }

    void Persistence::AddHarvestCommand(const HarvestCommand& command)
    {
        harvestCommands_.push_back(command);
    }

    void Persistence::AddFarmCommand(const FarmCommand& command)
    {
        farmCommands_.push_back(command);
    }

    void Persistence::ForEachHarvestCommand(const std::function<void(const HarvestCommand&)>& fn)
    {
        for(auto each : harvestCommands_)
        {
            fn(each);
        }
    }

    void Persistence::ForEachFarmCommand(const std::function<void(const FarmCommand&)>& fn)
    {
        for(auto each : farmCommands_)
        {
            fn(each);
        }
    }

    void Persistence::WriteToFile(const std::string& fileName)
    {
        std::ofstream out;
        const std::string path = std::string(FILE_DIR) + fileName + FILE_EXT;
        out.open(path, std::ios::binary);
        if(!out.is_open())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Unable to open `%s' for writing", __FUNCTION__, path.c_str());
            return;
        }
        // first write major and minor version
        out.write((char*)&MAJOR_VERSION, sizeof(MAJOR_VERSION));
        out.write((char*)&MINOR_VERSION, sizeof(MINOR_VERSION));
        // next write inventory.
        // first the number of entries.
        size_t numEntries = items_.size();
        out.write((char*)&numEntries, sizeof(numEntries));
        // then each entry
        for(const auto& item : items_)
        {
            // to write string to file we write the length then the string without the 0 at the end
            size_t sz = item.name.size();
            out.write((char*)&sz, sizeof(sz));
            out.write((char*)&item.name[0], sz);
            // write the item count
            out.write((char*)&item.count, sizeof(item.count));
            // write the item durability
            out.write((char*)&item.durability, sizeof(item.durability));
        }
        // write the move command
        out.write((char*)&locationCommand_.locationX, sizeof(locationCommand_.locationX));
        out.write((char*)&locationCommand_.locationY, sizeof(locationCommand_.locationY));
        // write each harvest command starting with the number of harvest commands
        size_t numHarvestCommands = harvestCommands_.size();
        out.write((char*)&numHarvestCommands, sizeof(numHarvestCommands));
        for(auto command : harvestCommands_)
        {
            // x y and count
            out.write((char*)&command.targetX, sizeof(command.targetX));
            out.write((char*)&command.targetY, sizeof(command.targetY));
            out.write((char*)&command.count, sizeof(command.count));
        }
        // write each farm command starting with the number of farm commands
        size_t numFarmCommands = farmCommands_.size();
        out.write((char*)&numFarmCommands, sizeof(numFarmCommands));
        for(auto command : farmCommands_)
        {
            // x, y, (char)readyToFarm, (time_t)farmTime
            out.write((char*)&command.targetX, sizeof(command.targetX));
            out.write((char*)&command.targetY, sizeof(command.targetY));
            out.write((char*)&command.readyToFarm, sizeof(command.readyToFarm));
            out.write((char*)&command.farmedTime, sizeof(command.farmedTime));
        }
        // write player experience and current level and gender
        out.write((char*)&playerData_, sizeof(playerData_));
        // write the number of buildings and each building data
        size_t numBuildings = buildingData_.size();
        out.write((char*)&numBuildings, sizeof(numBuildings));
        for(size_t i=0; i < numBuildings; ++i)
        {
            size_t strLen = buildingData_[i].name.size();
            out.write((char*)&strLen, sizeof(strLen));
            out.write((char*)&buildingData_[i].name[0], strLen);
            out.write((char*)&buildingData_[i].x, sizeof(buildingData_[i].x));
            out.write((char*)&buildingData_[i].y, sizeof(buildingData_[i].y));
            // the write of each info struct depends on the associated boolean
            out.write((char*)&buildingData_[i].harvesting, sizeof(buildingData_[i].harvesting));
            if(buildingData_[i].harvesting)
            {
                out.write((char*)&buildingData_[i].harvestData.remainingClicks,
                            sizeof(buildingData_[i].harvestData.remainingClicks));
            }
            out.write((char*)&buildingData_[i].farming, sizeof(buildingData_[i].farming));
            if(buildingData_[i].farming)
            {
                out.write((char*)&buildingData_[i].farmData.remainingFarms,
                            sizeof(buildingData_[i].farmData.remainingFarms));
                out.write((char*)buildingData_[i].farmData.lastFarmTime, 
                            sizeof(buildingData_[i].farmData.lastFarmTime));
            }
            out.write((char*)&buildingData_[i].crafting, sizeof(buildingData_[i].crafting));
            if(buildingData_[i].crafting)
            {
                out.write((char*)&buildingData_[i].craftData.craftingBegan,
                            sizeof(buildingData_[i].craftData.craftingBegan));
                strLen = buildingData_[i].craftData.itemCrafting.size();
                out.write((char*)&strLen, sizeof(strLen));
                out.write((char*)&buildingData_[i].craftData.itemCrafting[0], strLen);
            }
        }
        // write the current system time.
        time_t currentTime = time(nullptr);
        out.write((char*)&currentTime, sizeof(currentTime));
        out.close();
        ClearData(); // discard data after writing. caller will have to manually refill data for another save
    }

    bool Persistence::ReadFromFile(const std::string& fileName)
    {
        // destroy existing data just in case.
        ClearData();

        std::ifstream in;
        std::string path = std::string(FILE_DIR) + fileName + FILE_EXT;
        in.open(path, std::ios::binary);
        if(!in.is_open())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "%s: Unable to open `%s' for reading", __FUNCTION__, path.c_str());
            return false;
        }
        unsigned char majorVersion, minorVersion;
        // read and announce version numbers.
        in.read((char*)&majorVersion, sizeof(majorVersion));
        in.read((char*)&minorVersion, sizeof(minorVersion));
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
            "%s: Loading save file version %d.%d", __FUNCTION__, majorVersion, minorVersion);
        size_t numEntries;
        // number of item entries
        in.read((char*)&numEntries, sizeof(numEntries));
        for(int i=0; i < numEntries; ++i)
        {
            ItemData ie;
            // first, name
            size_t strLen;
            in.read((char*)&strLen, sizeof(strLen));
            ie.name.resize(strLen);
            in.read((char*)&ie.name[0], strLen);
            // then count.
            in.read((char*)&ie.count, sizeof(ie.count));
            // then durability
            in.read((char*)&ie.durability, sizeof(ie.durability));
            // Add the entry.
            items_.push_back(ie);
        }
        // read the move command.
        in.read((char*)&locationCommand_.locationX, sizeof(locationCommand_.locationX));
        in.read((char*)&locationCommand_.locationY, sizeof(locationCommand_.locationY));
        // read number of harvest commands.
        size_t numHarvestCommands;
        in.read((char*)&numHarvestCommands, sizeof(numHarvestCommands));
        // read each harvest command.
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
            "%s: Number of harvest commands: %d", __FUNCTION__, numHarvestCommands);
        for(size_t i=0; i < numHarvestCommands; ++i)
        {
            // x, y, then count
            HarvestCommand cmd(0, 0, 0);
            in.read((char*)&cmd.targetX, sizeof(cmd.targetX));
            in.read((char*)&cmd.targetY, sizeof(cmd.targetY));
            in.read((char*)&cmd.count, sizeof(cmd.count));
            harvestCommands_.push_back(cmd);
        }
        // read number of farm commands
        size_t numFarmCommands;
        in.read((char*)&numFarmCommands, sizeof(numFarmCommands));
        // read each farm command.
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
            "%s: Number of farm commands: %d", __FUNCTION__, numFarmCommands);
        for(size_t i=0; i < numFarmCommands; ++i)
        {
            FarmCommand fc(0,0,false,0);
            in.read((char*)&fc.targetX, sizeof(fc.targetX));
            in.read((char*)&fc.targetY, sizeof(fc.targetY));
            in.read((char*)&fc.readyToFarm, sizeof(fc.readyToFarm));
            in.read((char*)&fc.farmedTime, sizeof(fc.farmedTime));
            farmCommands_.push_back(fc);
        }

        // read player experience and then current level and then gender
        in.read((char*)&playerData_, sizeof(playerData_));
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
            "PLAYER GENDER = %s", playerData_.boy ? "boy" : "girl");
        // read buildings
        size_t numBuildings;
        in.read((char*)&numBuildings, sizeof(numBuildings));
        for(size_t i=0; i < numBuildings; ++i)
        {
            BuildingData data;
            size_t strLen;
            in.read((char*)&strLen, sizeof(strLen));
            data.name.resize(strLen);
            in.read((char*)&data.name[0], strLen);
            in.read((char*)&data.x, sizeof(data.x));
            in.read((char*)&data.y, sizeof(data.y));
            in.read((char*)&data.harvesting, sizeof(data.harvesting));
            if(data.harvesting)
            {
                in.read((char*)&data.harvestData.remainingClicks, sizeof(data.harvestData.remainingClicks));
            }
            in.read((char*)&data.farming, sizeof(data.farming));
            if(data.farming)
            {
                in.read((char*)&data.farmData.remainingFarms, sizeof(data.farmData.remainingFarms));
                in.read((char*)&data.farmData.lastFarmTime, sizeof(data.farmData.lastFarmTime));
            }
            in.read((char*)&data.crafting, sizeof(data.crafting));
            if(data.crafting)
            {
                in.read((char*)&data.craftData.craftingBegan, sizeof(data.craftData.craftingBegan));
                in.read((char*)&strLen, sizeof(strLen));
                data.craftData.itemCrafting.resize(strLen);
                in.read((char*)&data.craftData.itemCrafting[0], strLen);
            }
            buildingData_.push_back(data);
        }
        // read timestamp
        in.read((char*)&timeStamp_, sizeof(timeStamp_));
        in.close();
        return true;
    }

    void Persistence::ClearData()
    {
        locationCommand_.locationX = 0;
        locationCommand_.locationY = 0;
        harvestCommands_.clear();
        farmCommands_.clear();
        items_.clear();
        playerData_ = {0,0,0};
        buildingData_.clear();
    }

    void Persistence::SavePlayerData(const PLAYER_DATA& data)
    {
        playerData_ = data;
    }

    Persistence::PLAYER_DATA Persistence::GetPlayerData()
    {
        return playerData_;
    }

    void Persistence::SaveBuildingData(const std::vector<Building*>& buildings)
    {
        buildingData_.clear();
        for(const auto& each : buildings)
        {
            BuildingData data;
            data.name = each->GetEntry().name;
            data.x = (int)each->position.x;
            data.y = (int)each->position.y;
            if(each->GetEntry().harvesting)
            {
                data.harvesting = true;
                data.harvestData.remainingClicks = each->GetRemainingHarvests();
            }
            if(each->GetEntry().farming)
            {
                data.farming = true;
                data.farmData.lastFarmTime = each->GetLastFarmTime();
                data.farmData.remainingFarms = each->GetRemainingFarms();
            }
            if(each->GetEntry().crafting.size() != 0)
            {
                data.crafting = true;
                data.craftData.craftingBegan = each->GetLastCraftStartTime();
                data.craftData.itemCrafting = each->GetItemCrafting();
            }
            buildingData_.push_back(data);
        }
    }
    
    std::vector<Building*> Persistence::GenerateBuildings(const BuildingTable& db)
    {
        std::vector<Building*> buildings;
        for(const auto& each : buildingData_)
        {
            // Building ctor is very picky so we have to validate our information first
            const auto& findEntry = db.GetBuildingEntries().find(each.name);
            if(findEntry == db.GetBuildingEntries().end())
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                    "%s: Possible save game corruption: building `%s' is not in database!",
                    __FUNCTION__, each.name.c_str());
            }
            const BuildingEntry& entry = findEntry->second;
            const Building::HarvestData* harvestData = nullptr;
            if(entry.harvesting) harvestData = &each.harvestData;
            const Building::FarmData* farmData = nullptr;
            if(entry.farming) farmData = &each.farmData;
            const Building::CraftData* craftData = nullptr;
            if(entry.crafting.size() != 0) craftData = &each.craftData;
            // TODO: check boolean flags in each against pointer results.
            Building* newBuilding = new Building(entry, each.x, each.y, harvestData, farmData, craftData);
            buildings.push_back(newBuilding);
        }
        return buildings;
    }
}