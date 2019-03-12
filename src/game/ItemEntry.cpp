// ItemEntry.cpp
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

#include "ItemEntry.hpp"

namespace game
{
    ItemEntry::ItemEntry(const std::string& nm, const std::string& ttl, const std::string& ttip,
                         const std::string& ico, bool h, bool un, int fs, ItemType t) 
        : name(nm), title(ttl), tooltip(ttip), icon(ico), hidden(h), unique(un), foodstuff(fs), type(t)
    {
        switch(t)
        {
            case ItemType::Consumable: info = new ConsumableItemInfo(); break;
            case ItemType::Equipment: info = new EquipmentItemInfo(); break;
            case ItemType::Food: info = new FoodItemInfo(); break;
        }
    }
    
    ItemEntry::~ItemEntry() 
    {
        switch(type)
        {
            case ItemType::Consumable: delete ((ConsumableItemInfo*)info); break;
            case ItemType::Equipment: delete ((EquipmentItemInfo*)info); break;
            case ItemType::Food: delete ((FoodItemInfo*)info); break;
        }
    }

}