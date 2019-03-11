// Entity.hpp
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
#include <vector>

#include "game/PlayerCommand.hpp"
#include "game/Sprite.hpp"
#include "ogl/Texture.hpp"

namespace game 
{
    // A description of an entity used to construct Entity objects
    class EntityType 
    { public:
        // The primary database key. (The entries are defined in a map's script file)
        std::string name;
        // The anim0_ texture to use for the Entity. Entity objects are NOT animated so there is only this texture
        std::string texturePath;
        // The dimensions of the object
        int width = 0, height = 0;
        // The collision information. Relative to position. If the first value is -1 that indicates there is no
        //  valid collision box
        BOX collision = {0,0,0,0};
        // The maximum number of harvest interactions that can be applied to the entity before it despawns.
        int maxClicks = 0;
        // The amount of time in seconds a harvest interaction with this entity takes to complete.
        float clickTime = 0.f;
        // Defines item database entries that can modify inventory contents when the object is clicked or destroyed
        class ItemDrop
        { public:
            // a number between 0 and 100 indicated the chance the item has to be added to inventory
            float percentChance = 0.f;
            // the number of the item to add to inventory if the chance roll is success
            int amount = 0;
            // primary key for item database
            std::string name;
        };
        // The possible items that drop when the entity is harvested
        std::vector<ItemDrop> drops;
        // The possible items that drop when the entity is destroyed
        std::vector<ItemDrop> onDestroy;
        // Indicates item drop information for when the entity is "farmed." Farming means that a click
        // is not applied to the target, but instead, the entity becomes "unfarmable" for a specified
        // number of seconds, calculated by system time between saves.
        class FarmInfo
        { public:
            // What item can possibly drop when farmed.
            ItemDrop drop;
            // The number of seconds before the entity becomes farmable again
            unsigned int respawnTime = 1;
            // The texture to represent the entity when it is not farmable
            std::string pendingTexture;
        };
        // Whether or not this entity is farmable
        bool farmable = false;
        // See FarmInfo definition. This field is meaningless if farmable is false
        FarmInfo farmInfo;
    };

    // Represents an item of an amount to be added to inventory.
    class ItemDropInfo { public: std::string name; int num; };

    // static possibly interactive objects in the map
    class Entity : public Sprite 
    {
    public:
        // ctor
        Entity(const EntityType& etype);
        // dtor
        virtual ~Entity();
        // Get the name of the entity
        inline std::string GetName() const { return name_; }
        // Get the maximum number of harvests allowed before entity is destroyed
        inline int GetMaxClicks() const { return maxClicks_; }
        // Get the remaining number of harvests allowed before entity is destroyed
        inline int GetRemainingClicks() const { return remainingClicks_; }
        // Sets the remaining number of harvests
        inline void SetRemainingClicks(int r) { remainingClicks_ = r; }
        // Decrease the remaining number of harvests by one
        inline void DecRemainingClicks() { remainingClicks_--; }
        // Gets the amount of time a harvest interaction requires in seconds
        inline float GetClickTime() const { return clickTime_; }
        // Returns a loot table of item information representing what should possibly drop when 
        //  the entity is harvested. Does not affect number of harvests remaining.
        std::vector<ItemDropInfo> OnInteract();
        // Returns a loot table of item information representing what should possibly drop when
        //  the entity is destroyed. Does not destroy the entity.
        std::vector<ItemDropInfo> OnDestroy();
        // Returns whether or not the entity can be farmed.
        inline bool IsFarmable() const { return farmable_; }
        // Returns whether or not an entity is currently farmable
        inline bool IsReadyForPickup() const { return farmEvent_.readyForPickup; }
        // Returns the amount of time remaining in seconds before the entity becomes farmable or 0.0f
        //  if it is already farmable or if it is not a farmable entity
        time_t FarmTimeRemaining() const;
        // Sets the farmable flag to false and returns a generated loot table calculated with RNG on what
        //  dropped. 
        ItemDropInfo Farm();
        // Processes a farm command loaded from a save file to determine whether or not entity can be farmed
        //  at the moment. Has no effect if entity is not farmable.
        void SetFarmData(const FarmCommand& fc);
        // Calls Sprite::Update to update position in case entity is moving. Then checks system time against
        //  last farm time to determine whether or not the entity should become farmable again IFF the entity
        //  is a farmable one.
        void Update(float dtime) override;
    private:
        // primary key into entity database defined by map's script file
        std::string name_;
        // maximum number of harvest clicks before destroyed. If -1, then the object cannot be harvested but can still
        //  possibly be farmed.
        int maxClicks_;
        // remaining amount of harvest clicks before object will be destroyed.
        int remainingClicks_;
        // the amount of time in seconds required to farm or harvest
        float clickTime_;
        // Defines loot table entries stored internally, generated by a map's script file
        class ItemDrop { public:
            // number between 0 and 100 percent chance to drop
            float percentChance;
            // number of items to drop
            int amount;
            // name of item as defined in item database
            std::string name;
        };
        // loot table for harvesting actions
        std::vector<ItemDrop> onInteract_;
        // loot table for destroying entity (if harvestable)
        std::vector<ItemDrop> onDestroy_;
        // used for storing the farm state of farmable entities
        class FarmEvent { public:
            // what drops when farmed
            ItemDrop itemDrop;
            // time it takes to regenerate between farms
            unsigned int respawnTimer;
            // whether or not it's ready to farm again
            bool readyForPickup = true;
            // the last system time it was farmed
            time_t farmTimeStamp = 0;
            // what texture to use for the sprite when not farmable
            ogl::Texture* pendingTexture;
        };
        // whether or not entity can be farmed
        bool farmable_;
        // the state of farming
        FarmEvent farmEvent_;
        // holds the usual texture for entity when it is not farmable
        ogl::Texture* swapTexture_ = nullptr;
    };

}