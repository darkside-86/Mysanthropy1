// ItemEntry.hpp
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

#include "ogl/Texture.hpp"

namespace game
{
    enum class ItemType
    {
        None, // like a piece of wood or stone
        Consumable, // like a potion or anything edible/drinkable in combat and has a cooldown
        Food, // something that can be eaten out of combat for a buff. Has a foodstuff conversion value
        Equipment // something that can go in the character gear list and has durability
    };

    class BaseItemInfo 
    {
    protected:
        BaseItemInfo() {}
    };

    class ConsumableItemInfo : public BaseItemInfo
    {
        // TODO: ...
    };

    class FoodItemInfo : public BaseItemInfo
    {
        // TODO: ...
    };

    class EquipmentItemInfo : public BaseItemInfo
    {
    public:
        // TODO: list of equipment stats from combat namespace
        // ..
        // total durability of item
        int durability;
    };

    // Database entry for item
    class ItemEntry
    {
    public:
        // ctor - requires a type so destructor knows what if anything to delete concerning info
        ItemEntry(const std::string& nm, const std::string& ttl, const std::string& ttip,
                  const std::string& ico, bool h, bool un, int fs, ItemType t);
        // dtor
        virtual ~ItemEntry();
        // database unique primary key.
        std::string name;
        // title - what to be shown in UI systems
        std::string title;
        // tooltip - something for UI to show when hovered over
        std::string tooltip;
        // path to the icon associated with the item
        std::string icon;
        // Whether or not item should be shown in an inventory UI
        bool hidden = false;
        // Whether or not inventory can carry more than one
        bool unique = false;
        // foodstuff conversion value if any
        int foodstuff = 0;
        // light RTTI for item info pointer
        ItemType type = ItemType::None;
        // Specific data if non-null
        BaseItemInfo* info = nullptr;
    };

}