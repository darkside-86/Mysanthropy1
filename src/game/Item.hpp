// Item.hpp
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

#include "ItemEntry.hpp"
#include "ogl/Texture.hpp"

namespace game
{
    // An entry in the inventory
    class Item
    {
    public:
        // ctor
        Item(const ItemEntry& ie);
        // used by inventory to determine how many of an identical item is present
        int count = 0;
        // for equipment, durability out of itemEntry_.info->durability. Used to separate otherwise
        //  identical equippable items.
        int durability = 0; 
        // Returns a reference to the item entry that has the database info about the item
        const ItemEntry& GetItemEntry() const { return itemEntry_; }
        // Convenience accessors
        bool IsHidden() const { return itemEntry_.hidden; }
        bool IsUnique() const { return itemEntry_.unique; }
        int GetMaxDurability() const 
            { return itemEntry_.type==ItemType::Equipment? ((EquipmentItemInfo*)itemEntry_.info)->durability: 0; }
    private:
        const ItemEntry& itemEntry_;
    };
}