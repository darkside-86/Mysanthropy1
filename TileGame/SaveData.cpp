// SaveData.cpp
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

#include "SaveData.h"

#include <fstream>

#include "engine/GameEngine.h"

SaveData::SaveData(Inventory& inv, Player& player) : moveCommand_(0,0), inventory_(inv), player_(player)
{

}

SaveData::~SaveData()
{
}

void SaveData::AddHarvestCommand(const HarvestCommand& command)
{
    harvestCommands_.push_back(command);
}

void SaveData::AddFarmCommand(const FarmCommand& command)
{
    farmCommands_.push_back(command);
}

void SaveData::ForEachHarvestCommand(const std::function<void(const HarvestCommand&)>& fn)
{
    for(auto each : harvestCommands_)
    {
        fn(each);
    }
}

void SaveData::ForEachFarmCommand(const std::function<void(const FarmCommand&)>& fn)
{
    for(auto each : farmCommands_)
    {
        fn(each);
    }
}

void SaveData::WriteToFile(const std::string& fileName)
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
    size_t numEntries = inventory_.GetNumEntries();
    out.write((char*)&numEntries, sizeof(numEntries));
    // then each entry
    inventory_.ForEachItemEntry([&out](const std::string& k, const ITEM_ENTRY& entry){
        // name first
        std::string itemName = entry.item->GetName();
        size_t sz = itemName.size();
        out.write((char*)&sz, sizeof(sz));
        out.write(itemName.c_str(), sz);
        // then count
        int count = entry.count;
        out.write((char*)&count, sizeof(count));
    });
    // write the move command
    out.write((char*)&moveCommand_.locationX, sizeof(moveCommand_.locationX));
    out.write((char*)&moveCommand_.locationY, sizeof(moveCommand_.locationY));
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
    // write player experience and current level.
    int exp = player_.GetExperience();
    int level = player_.GetLevel();
    out.write((char*)&exp, sizeof(exp));
    out.write((char*)&level, sizeof(level));
    // finally write the current system time.
    time_t currentTime = time(nullptr);
    out.write((char*)&currentTime, sizeof(currentTime));
    out.close();
}

bool SaveData::ReadFromFile(const std::string& fileName)
{
    // destroy existing data.
    Cleanup();

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
        // first, name
        size_t strLen;
        in.read((char*)&strLen, sizeof(strLen));
        char* szName = new char[strLen+1];
        szName[strLen] = 0;
        in.read(szName, strLen);
        std::string name = szName;
        delete [] szName;
        // then count.
        int count;
        in.read((char*)&count, sizeof(count));
        // Add the entry.
        inventory_.AddItemByName(name, count);
    }
    // read the move command.
    in.read((char*)&moveCommand_.locationX, sizeof(moveCommand_.locationX));
    in.read((char*)&moveCommand_.locationY, sizeof(moveCommand_.locationY));
    // read number of harvest commands.
    size_t numHarvestCommands;
    in.read((char*)&numHarvestCommands, sizeof(numHarvestCommands));
    // read each harvest command.
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
    for(size_t i=0; i < numFarmCommands; ++i)
    {
        FarmCommand fc(0,0,false,0);
        in.read((char*)&fc.targetX, sizeof(fc.targetX));
        in.read((char*)&fc.targetY, sizeof(fc.targetY));
        in.read((char*)&fc.readyToFarm, sizeof(fc.readyToFarm));
        in.read((char*)&fc.farmedTime, sizeof(fc.farmedTime));
        farmCommands_.push_back(fc);
    }

    // read player experience and then current level.
    int exp, level;
    in.read((char*)&exp, sizeof(exp));
    in.read((char*)&level, sizeof(level));
    player_.SetLevel(level);
    player_.SetExperience(exp);
    // read timestamp
    in.read((char*)&timeStamp_, sizeof(timeStamp_));
    in.close();
    return true;
}

void SaveData::Cleanup()
{
    moveCommand_.locationX = 0;
    moveCommand_.locationY = 0;
    harvestCommands_.clear();
    farmCommands_.clear();
    inventory_.ClearItems();
}