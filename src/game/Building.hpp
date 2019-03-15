// Building.hpp
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

#include <ctime>

#include "BuildingEntry.hpp"
#include "Inventory.hpp"
#include "Sprite.hpp"

namespace game
{
    class Building : public Sprite 
    {
    public:
        // enum for interpreting context sensitive Interact() function so caller knows what
        //  entry data to read
        enum class Interaction { None, Harvest, Farm, PickupCrafted, ReadyToCraft };

        // types for Persistence to fill in and send to ctor when building is loaded from file
        class HarvestData
        { public:
            int remainingClicks;
        };

        class FarmData
        { public:
            int remainingFarms;
            time_t lastFarmTime;
        };

        class CraftData 
        { public:
            time_t craftingBegan;
            std::string itemCrafting;
        };

        // ctor. The data parameters MUST BE NON-NULL if the entry has information associated with
        //  them. The data parameters MUST BE NULL if the entry has no information associated with 
        //  them. e.g. A harvest-only building (like a well) must receive HarvestData and ONLY 
        //  HarvestData. 
        Building(const BuildingEntry& entry, int x, int y, const HarvestData* harvestData = nullptr,
                 const FarmData* = nullptr, const CraftData* = nullptr);
        // dtor
        virtual ~Building();

        // override update so that farming and crafting time can be checked every update
        void Update(float dtime) override;
        // override render in case an extra quad for crafting needs to be rendered.
        void Render(const glm::vec2& camera, ogl::Program& program) override;
        
        //-- accessors --------------------------------------------------------

        // Returns true when building can be clicked some number of times to harvest like an entity
        inline bool IsHarvestable() const { return entry_.harvesting != nullptr; }
        // Returns the number of remaining harvest clicks. Should be checked after Interact IF IsHarvestable
        //  returns true.
        inline int GetRemainingHarvests() const { return remainingClicks_; }
        // Returns true when building can be farmed every so often like some entities
        inline bool IsFarmable() const { return entry_.farming != nullptr; }
        // Return true if currently can be farmed with an Interact call. Should be used in conjunction
        //  with other calls to ensure validity of return value.
        inline bool IsReadyToFarm() const { return readyForFarm_; }
        // Get the number of remaining farm interactions before building should be changed.
        inline int GetRemainingFarms() const { return remainingFarms_; }
        // Get the timestamp of the last farm event
        inline time_t GetLastFarmTime() const { return lastFarmTime_; }
        // Returns true when building can be used to craft a special item that takes a long time
        inline bool IsCraftable() const { return entry_.crafting.size() != 0; }
        // Returns true if building removal process requires time and drops anything
        inline bool IsRemovable() const { return entry_.removing != nullptr; }
        // Returns true if an item is being crafted in the building, false if an item is ready to start
        inline bool IsCurrentlyCrafting() const { return craftingItem_ != ""; }
        // Returns the last craft start timestamp
        inline time_t GetLastCraftStartTime() const { return craftingBegan_; }
        // Returns the item being crafted or "" for none
        inline std::string GetItemCrafting() const { return craftingItem_; }
        // Returns true if a craftable item is ready for pickup. To be used in conjunction with other calls
        //  such as GetInteraction and checking building entry type to make sure result is valid.
        inline bool IsReadyForPickup() const { return readyForPickup_; }
        // Returns a reference to the database entry associated with this building
        inline const BuildingEntry& GetEntry() const { return entry_; }

        //-- modifiers of inventory and building ------------------------------

        // consumes inventory items required to build the building
        bool Construct(Inventory& inv);
        // returns the type of interaction that would be performed if clicked.
        Interaction GetInteraction();
        // context sensitive interaction, can be harvest, farming, or picking up a craftable
        //  returns true upon meaningful interaction that adds (on chance) to inventory.
        bool Interact(Inventory& inv);
        // returns true if successful craft starts
        bool CraftBegin(Inventory& inv, const std::string& itemToCraft);
        // Returns true if the tearing down of building yields any items
        bool Remove(Inventory& inv);

    private:
        // utility function for adding Drop loot to inventory based on chance
        void AddLoot(Inventory& inv, const BuildingEntry::Drop& drop);

        // data check timer--don't pull system time and iterate crafting set every single cycle because
        //  farm and craft times are in seconds anyway
        static constexpr float DATA_CHECK_TIME = 1.0f; // in seconds
        float dataCheckTimer_ = 0.0f; // in seconds
        // harvest info
        int remainingClicks_ = 0; // not harvestable by default
        // farm info
        int remainingFarms_ = 0; // not farmable by default
        time_t lastFarmTime_;
        bool readyForFarm_ = false; // if building is loaded from file, ctor will check time and change this
        ogl::Texture* pendingFarm_ = nullptr; // managed by TextureManager
        ogl::Texture* readyFarmTexture_ = nullptr; // a place to hold anim0_
        // craft info
        time_t craftingBegan_; // system time of when crafting started
        std::string craftingItem_; // title of item being crafted according to entry_.crafting[N].title
        bool readyForPickup_ = false; // if building is loaded, ctor will check time and change as needed
        ogl::Texture* pendingCrafting_ = nullptr; // managed by TextureManager
        ogl::Texture* completedCrafting_ = nullptr; // managed by TextureManager
        ogl::Texture* currentCraftTexture_ = nullptr; // one of the above for the craft quad
        // extra quad to render on top of sprite for crafting 
        ogl::VertexArray* craftVao_ = nullptr;
        ogl::VertexBuffer* craftVbo_ = nullptr;
        // link to BuildingEntry
        const BuildingEntry& entry_;
    };
}