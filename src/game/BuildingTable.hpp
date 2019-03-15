// BuildingTable.hpp
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

#include <string>
#include <unordered_map>

#include <lua/lua.hpp>

#include "BuildingEntry.hpp"

namespace game
{
    // Database of all building types
    class BuildingTable
    {
    public:
        BuildingTable();
        virtual ~BuildingTable();
        const BuildingEntry* GetEntry(const std::string& name) const;
        inline const std::unordered_map<std::string, const BuildingEntry>& GetBuildingEntries() const
            { return buildingEntries_; }
    private:
        static int lua_Building(lua_State* L);
        std::unordered_map<std::string, const BuildingEntry> buildingEntries_;
    };
}