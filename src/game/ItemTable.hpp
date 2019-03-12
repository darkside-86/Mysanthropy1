// ItemTable.hpp
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

#include "ItemEntry.hpp"

namespace game
{
    // Database of all possible items
    class ItemTable
    {
    public:
        // ctor
        ItemTable();
        // dtor
        virtual ~ItemTable();
        // get a const reference to the database table
        const std::unordered_map<std::string,const ItemEntry*>& GetItemEntries() const 
            { return itemEntries_; }
    private:
        // Lua function for reading each entry in the database file
        static int lua_ItemEntry(lua_State* L);
        // database of items read from Lua data file
        std::unordered_map<std::string,const ItemEntry*> itemEntries_;
    };
}