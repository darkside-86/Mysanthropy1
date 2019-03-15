// BuildingEntry.hpp
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
#include <vector>

#include "Inventory.hpp"

namespace game 
{
    // An entry into the building database
    class BuildingEntry
    {
    public:
    // Types specific to Building /////////////////////////////////////////////

        // represents an item requirement to build it
        class Requirement
        { public:
            // name of item in item db
            std::string name; 
            // number of item needed
            int count;
        };

        // represents collision data
        class Collision
        { public:
            // a rectangle relative to position
            int left, top, right, bottom;
        };

        // loot table entry for drops on chance
        class Drop
        { public:
            std::string item;
            int count;
            float chance;
        };

        // represents optional harvestable field
        class HarvestingInfo
        { public:
            // how many times building can be interacted with before changing to a different building
            int maxClicks;
            // how long each harvest click action takes
            int time;
            // loot table for each harvest click
            std::vector<Drop> drops;
            // once all harvest clicks are used, the building turns into this building ID
            std::string completed;
        };

        // represents farmable building entities
        class FarmingInfo
        { public:
            // how many times it can be farmed. -1 for infinite times like a coconut tree
            int maxFarms;
            // how long it takes to pluck the item from the object
            int time;
            // how long it takes for the item to respawn in seconds
            int frequency;
            // what texture to use when item isn't ready for pickup
            std::string pending;
            // what drops when farm action is done
            std::vector<Drop> drops;
            // what the object turns into once farming is maxed out
            std::string completed;
        };

        // info for buildings that can be used to make items over a long time without direct user interaction
        class CraftingInfo
        { public:
            // something to identify the entry. Not exactly an item db entry because crafting drops multiple items
            std::string title;
            // required item(s) to craft this item
            std::vector<Requirement> sources;
            // time in seconds the craft takes the building to complete
            int time;
            // what texture to draw on top of building while craft is in progress
            std::string pending;
            // what texture to draw on top of building to indicate item is done and ready for pickup
            std::string completed;
            // the resulting items that were grown/made by the building when done
            std::vector<Drop> results;
        };

        // when "Remove building" option selected from UI
        class RemovingInfo
        { public:
            // how long removal takes
            int time;
            // what drops upon forced removal
            std::vector<Drop> drops;
        };

    // Ctor and dtor //////////////////////////////////////////////////////////

        BuildingEntry() {}
        BuildingEntry(const BuildingEntry& entry) 
        {
            name = entry.name;
            title = entry.title;
            tooltip = entry.tooltip;
            texture = entry.texture;
            width = entry.width;
            height = entry.height;
            required = entry.required;
            collision = entry.collision;
            time = entry.time;
            level = entry.level;
            if(entry.harvesting)
            {
                delete harvesting;
                harvesting = new HarvestingInfo(*(entry.harvesting));
            }
            if(entry.farming)
            {
                delete farming;
                farming = new FarmingInfo(*(entry.farming));
            }
            crafting = entry.crafting;
            if(entry.removing)
            {
                delete removing;
                removing = new RemovingInfo(*(entry.removing));
            }
        }
        virtual ~BuildingEntry() 
        {
            if(harvesting) delete harvesting;
            if(farming) delete farming;
            if(removing) delete removing;
        }

    // Useful methods 

        // checks material requirement against inventory counts
        bool CanConstruct(Inventory& inv) const
        {
            for(const auto& each : required)
            {
                if(inv.GetItemCount(each.name) < each.count)
                {
                    return false;
                }
            }
            return true;
        }

    // Database entry fields //////////////////////////////////////////////////

        // primary key to database
        std::string name;
        // title to display on UI
        std::string title;
        // tooltip to display on UI
        std::string tooltip;
        // path to the texture representing the building
        std::string texture;
        // the size in logical pixels
        int width, height;
        // item requirements to place the building
        std::vector<Requirement> required;
        // Collision data
        Collision collision;
        // time in seconds required to finish placing building on ground
        int time;
        // level requirement to build
        int level;
        // optional  harvest info
        HarvestingInfo* harvesting = nullptr;
        // optional farming info
        FarmingInfo* farming = nullptr;
        // optional info for long-term craftable items
        std::vector<CraftingInfo> crafting;
        // optional removal info. missing means building can be removed instantly and drops nothing
        RemovingInfo* removing = nullptr;
    };
}