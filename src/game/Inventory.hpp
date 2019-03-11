// Inventory.hpp
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

#include <functional>
#include <unordered_map>

#include <lua/lua.hpp>

#include "Item.hpp"

namespace game
{
    // Represents an entry of the item database. 
    class ItemEntry
    { public:
        // TODO: This shouldn't be a pointer and redundancy should be eliminated.
        Item *item=nullptr;
        // The number of the item in inventory
        int count=0;
        // The foodstuff value of an item when converted to the foodstuff currency. 
        //  TODO: eliminate this redundancy (should be in Item)
        int foodstuffValue=0;
    };

    // Represents the database of all possible items as well as the player inventory
    class Inventory
    {
    public:
        // ctor
        Inventory();
        // dtor
        virtual ~Inventory();
        // Adds a new item entry to the database. NOT for use in modifying the count of existing items
        void AddItemEntry(const std::string& name, const std::string& texturePath, ogl::Texture* texture,
                          bool hidden=false, int foodstuffValue=0);
        // Adds to the count value of an existing item in the backpack
        void AddItemByName(const std::string &name, int count);
        // Returns item information by name. caller does NOT own the pointer in the ItemEntry
        ItemEntry GetItemEntryByName(const std::string &name) const;
        // Deprecated. TODO: replace this nonsense with a const& return
        void ForEachItemEntry(std::function<void(const std::string&,const ItemEntry&)> expr) const;
        // Returns the number of entries in the item database
        inline size_t GetNumEntries() const { return items_.size(); }
        // Sets the inventory count of all items to 0. Does NOT erase item database entries
        void ClearItems();
        // Sets the count of an item
        void SetItemAmount(const std::string& name, int amount);
        // Gets the count of an item
        int GetItemAmount(const std::string& name);
        // Attempts to convert N items into foodstuff currency. Returns false if fails, such as due
        //  to the item having 0 foodstuff value or the player not having enough items to convert.
        bool ConvertItemToFoodstuff(const std::string& name, int amount);
    private:
        // see res/config/inventory.lua
        static int lua_ItemEntry(lua_State *L);
        // Database of all possible items. Owns ItemEntry::item pointer
        std::unordered_map<std::string,ItemEntry> items_;
        // Lua state for reading Lua defined database. TODO: discard this once the database is read
        //  from file. This serves no purpose otherwise.
        lua_State* script_ = nullptr;
    };
}