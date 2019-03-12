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

#include "engine/GameEngine.hpp"
#include "Inventory.hpp"

namespace game
{
    Inventory::Inventory(const ItemTable& itemTable) : itemTable_(itemTable)
    {

    }
    
    Inventory::~Inventory()
    {
        for(Item* item : items_)
        {
            delete item;
        }
    }
    
    bool Inventory::AddItem(const std::string& itemName, int count, int durability)
    {
        // first search for the ItemEntry in the database.
        const auto& itemEntries = itemTable_.GetItemEntries();
        const auto found = itemEntries.find(itemName);
        if(found == itemEntries.end())
        {   // if not found, print error and return false
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Failed to add item `%s' because it is not in database", 
                __FUNCTION__, itemName.c_str());
            return false;
        }
        // make sure count is > 0
        if(count <= 0)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: Attempt to add 0 or less `%s'", __FUNCTION__, itemName.c_str());
            return false;
        }
        // make sure durability was requested only for equipment
        if(durability != 0 && found->second->type != ItemType::Equipment)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Item `%s' cannot have a durability %d because it's not gear", 
                __FUNCTION__, itemName.c_str(), durability);
            return false;
        }
        // item was found but next we must check uniqueness and if true make sure
        //  there is not already the unique item in list
        if(found->second->unique)
        {
            const Item* existingItem = GetFirstItemByName(itemName);
            if(existingItem != nullptr)
            {   // this indicates the item already exists so report the error and return false.
                // (Inventory must be guaranteed to not hold on to 0 or less count items)
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                    "%s: Unable to add unique item `%s'", __FUNCTION__, itemName.c_str());
                return false;
            }
            // we can add the unique item now but only if count=1
            if(count != 1)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                    "%s: Item `%s' so only one can be added!", __FUNCTION__, itemName.c_str());
                return false;
            }
        }
        // all conditions are met to either modify the count field of an existing item
        // or in the case of equipment, always create a new entry
        Item* possiblyExistingItem = GetFirstItemByName(itemName);
        if(possiblyExistingItem == nullptr || found->second->type == ItemType::Equipment)
        {
            Item* newItem = new Item(*(found->second));
            newItem->count = count;
            newItem->durability = durability;
            items_.push_back(newItem);
        }
        else
        {
            possiblyExistingItem->count += count;
        }
        return true;
    }
    
    bool Inventory::RemoveItem(const std::string& itemName, int count, int durability)
    {
        // attempt to decrease the item count of an existing entry, and if the count becomes
        //  0, remove the entry from the list. If durability is non-zero, the item specified
        //  must be an equipment type and the first equipment item that matches the name and
        //  durability will be removed. (When equipment durability reaches 0, the game should
        //  convert it into a different unequippable item or destroy it completely)
        Item* possiblyExistingItem = GetFirstItemByName(itemName, durability);
        // if there is no entry at all, there is nothing to destroy so report this and return false
        if(possiblyExistingItem == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: There is no item entry for `%s'", __FUNCTION__, itemName.c_str());
            return false;
        }
        else 
        {   // make sure there are enough to remove according to count. If not, remove nothing at all
            if(possiblyExistingItem->count >= count)
            {
                possiblyExistingItem->count -= count;
            }
            else
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                    "%s: Failed to remove %d of item `%s'. Only %d in inventory", __FUNCTION__, 
                    count, itemName.c_str(), possiblyExistingItem->count);
                return false;
            }
            // make sure item count did not reach < 0. THIS CODE SHOULD BE UNREACHABLE except maybe
            //  in the case of integer overflow
            if(possiblyExistingItem->count < 0)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                    "%s: Something has gone horribly wrong!!!", __FUNCTION__);
                // try to remove it anyway but return false because the result is not valid
                RemoveItemFromListByPointer(possiblyExistingItem);
                return false;
            }
            else if(possiblyExistingItem->count == 0)
            {   // 0 means we remove the pointer from the list
                RemoveItemFromListByPointer(possiblyExistingItem);
            }
            // the count is decreased and/or the count=0 entry is removed
            return true;
        }
    }
    
    int Inventory::GetItemCount(const std::string& itemName)
    {
        // count the number of entries with the specified name regardless of durability
        int count = 0;
        for(Item* item : items_)
        {
            if(item->GetItemEntry().name == itemName)
                count += item->count;
        }
        return count;
    }

    Item* Inventory::GetFirstItemByName(const std::string& name, int durability)
    {
        // if durability is 0 (i.e. not specified) then we simply return the first
        // entry with the matching name or nullptr if not found. If durability is > 0
        // then the item has to be of type equipment and match the requested durability
        // else nullptr is returned.
        for(Item* item : items_)
        {
            if(durability == 0)
            {
                if(item->GetItemEntry().name == name)
                    return item;
            }
            else // durability is non-zero
            {
                if(item->GetItemEntry().type == ItemType::Equipment)
                {
                    if(item->durability == durability && item->GetItemEntry().name == name)
                        return item;
                }
                else // doesn't matter if the item name was found, requesting specific durability
                {    // for a non-equipment item is just absurd and meaningless.
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                        "%s: Searched by durability %d for unequippable `%s'. Why?", __FUNCTION__,
                        durability, name.c_str());
                    return nullptr;
                }
            }
        }
        // we didn't find anything
        return nullptr;
    }

    void Inventory::RemoveItemFromListByPointer(const Item* item)
    {
        std::vector<Item*>::iterator toFind = items_.begin();
        for(; toFind != items_.end(); ++toFind)
        {
            if((*toFind) == item)
                break;
        }
        if(toFind != items_.end())
            items_.erase(toFind);
    }

    bool Inventory::ConvertItemToFoodstuff(const std::string& name, int count)
    {
        // make sure the item is valid.
        const auto found = itemTable_.GetItemEntries().find(name);
        if(found == itemTable_.GetItemEntries().end())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Cannot convert non-existent item `%s' to foodstuff.", __FUNCTION__, name.c_str());
            return false;
        }
        // the db entry was found so check the foodstuff value. only ones with 1 or more worth can
        //  be converted.
        if(found->second->foodstuff <= 0)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: Item `%s' is not edible and cannot be stored as food!",
                __FUNCTION__, name.c_str());
            return false;
        }
        // make sure there are enough of the item to do the conversion
        Item* item = GetFirstItemByName(name);
        if(item == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: Item `%s' cannot be converted to foodstuff because it is missing from inventory!",
                __FUNCTION__, name.c_str());
            return false;
        }
        if(item->count < count)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: Failed to convert %d `%s' to foodstuff. Only %d in inventory.",
                __FUNCTION__, count, name.c_str(), item->count);
            return false;
        }
        // finally all error conditions are checked and conversion can be done.
        int foodstuffToAdd = found->second->foodstuff * count;
        // if foodstuff is missing from database for some reason then AddItem will return an error.
        RemoveItem(name, count); // use method to remove items to avoid 0 count entries
        return AddItem("foodstuff", foodstuffToAdd);
    }
}