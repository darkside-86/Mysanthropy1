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

#include <vector>

#include "Item.hpp"
#include "ItemTable.hpp"

namespace game
{
    // Represents the player inventory. Although there are no restrictions on amount of items
    //  in the inventory, equipment never interanally "stacks"--it always has a separate list entry because
    //  it is assumed that the durability will change frequently enough to warrant separate entries
    class Inventory
    {
    public:
        // ctor - requires item database parameter
        Inventory(const ItemTable& itemTable);
        // dtor - deallocates the Item pointers in the list
        virtual ~Inventory();
        // Adds a new item from the database into the inventory list. Returns true if
        //  item gets added (e.g. could fail if item is unique and already in inventory)
        //  if durability is not 0, the requested item has to be an equipment or it
        //  will not be added.
        bool AddItem(const std::string& itemName, int count, int durability=0);
        // Removes an item from the inventory. Returns true if the specified number of items
        //  are successfully removed, else false, such as if count=2 and there is only 1 of the item
        //  in which case nothing gets removed at all.
        bool RemoveItem(const std::string& itemName, int count, int durability=0);
        // Gets the number of a given item in the inventory. Equipment with varying durability
        //  is combined in the count if they have the same name
        int GetItemCount(const std::string& itemName);
        // TODO: Item* GetEquippableItemByDurability(name,durability)
        // Get const reference to the item list. Needed for building inventory UI info
        const std::vector<Item*>& GetItems() const { return items_; }
        // (Tries to) convert a number of items into foodstuff currency. Equipment will NEVER
        //  be edible so there is no durability parameter to check.
        bool ConvertItemToFoodstuff(const std::string& name, int count);
    private:
        // Gets the first Item* whose name matches parameter.
        Item* GetFirstItemByName(const std::string& name, int durability=0);
        // Remove an item from the list by pointer
        void RemoveItemFromListByPointer(const Item* item);
        // inventory has to be stored as a list because unstackable items--such as equipment
        //  with varying durability requires its own entry. Owns the pointers.
        std::vector<Item*> items_;
        // link to the database containing all types of possible items
        const ItemTable& itemTable_;
    };
}