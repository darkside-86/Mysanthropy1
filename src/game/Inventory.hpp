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

#include "Item.hpp"

namespace game
{
    class ItemEntry
    { public:
        Item *item=nullptr;
        int count=0;
        int foodstuffValue=0;
    };

    class Inventory
    {
    public:
        Inventory();
        virtual ~Inventory();
        void AddItemEntry(const std::string& name, ogl::Texture* texture, bool hidden=false, int foodstuffValue=0);
        void AddItemByName(const std::string &name, int count);
        ItemEntry GetItemEntryByName(const std::string &name) const;
        void ForEachItemEntry(std::function<void(const std::string&,const ItemEntry&)> expr) const;
        inline size_t GetNumEntries() const { return items_.size(); }
        void ClearItems();
        void SetItemAmount(const std::string& name, int amount);
        int GetItemAmount(const std::string& name);
        bool ConvertItemToFoodstuff(const std::string& name, int amount);
    private:
        // owns ItemEntry.item pointer
        std::unordered_map<std::string,ItemEntry> items_;
    };
}