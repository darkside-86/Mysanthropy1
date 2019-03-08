// Inventory.cpp
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
    Inventory::Inventory() 
    {

    }

    Inventory::~Inventory()
    {
        for(auto each : items_)
        {
            delete each.second.item;
        }
    }

    void Inventory::AddItemEntry(const std::string& name, ogl::Texture* texture, bool hidden, int foodstuffValue)
    {
        // make sure item entry is not already in the list
        auto found = items_.find(name);

        if(found == items_.end())
        {
            // not found so create the new item entry.
            ItemEntry ie;
            ie.item = new Item(name, texture, hidden);
            ie.count = 0;
            ie.foodstuffValue = foodstuffValue;
            items_[name] = ie;
        }
        else 
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                "%s: Item `%s' already exists!", __FUNCTION__, name.c_str());
        }
    }

    void Inventory::AddItemByName(const std::string &name, int count)
    {
        // search for item entry
        auto found = items_.find(name);

        if(found != items_.end())
        {
            found->second.count += count;
        }
        else 
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: Item `%s' has no entry.", __FUNCTION__, name.c_str());
        }
    }

    ItemEntry Inventory::GetItemEntryByName(const std::string &name) const
    {
        auto found = items_.find(name);
        if(found != items_.end())
        {
            return found->second;
        }
        else 
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: Item `%s' does not have an entry!", __FUNCTION__, name.c_str());
            ItemEntry blank = { nullptr, 0, 0 };
            return blank;
        }
    }

    void Inventory::ForEachItemEntry(std::function<void(const std::string&,const ItemEntry&)> expr) const
    {
        for(auto it = items_.begin(); it != items_.end(); ++it)
        {
            expr(it->first, it->second);
        }
    }

    void Inventory::ClearItems()
    {
        for(auto& each : items_)
        {
            each.second.count = 0;
        }
    }

    void Inventory::SetItemAmount(const std::string& name, int amount)
    {
        auto found = items_.find(name);
        if(found != items_.end())
        {
            found->second.count = amount;
        }
        else
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                "%s: Entry not found: `%s'", __FUNCTION__, name.c_str());
        }
        
    }

    int Inventory::GetItemAmount(const std::string& name)
    {
        auto found = items_.find(name);
        if(found != items_.end())
        {
            return found->second.count;
        }
        else
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                "%s: Entry not found: `%s'", __FUNCTION__, name.c_str());
            return 0;      
        }
    }

    bool Inventory::ConvertItemToFoodstuff(const std::string& name, int amount)
    {
        if(GetItemAmount(name) < amount)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "%s: Not enough %s to convert to foodstuff. Wanted: %d. Have: %d", __FUNCTION__, name.c_str(),
                amount, GetItemAmount(name));
            return false;
        }

        ItemEntry ie = GetItemEntryByName(name);

        if(ie.foodstuffValue <= 0)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "%s: Item %s is not worth any foodstuff. No trade.", __FUNCTION__, name.c_str());
            return false;
        }

        int foodstuffPerItem = ie.foodstuffValue;
        int totalFoodstuff = ie.foodstuffValue * amount;
        // first decrease the requested number of items to trade
        SetItemAmount(name, GetItemAmount(name) - amount);
        // then add the corresponding foodstuff value
        SetItemAmount("foodstuff", GetItemAmount("foodstuff") + totalFoodstuff);
        return true;
    }
}