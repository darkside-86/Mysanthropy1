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
    class EntityType 
    { public:
        std::string name;
        std::string texturePath;
        int width = 0, height = 0;
        BOX collision = {0,0,0,0};
        int maxClicks = 0;
        float clickTime = 0.f; // in seconds!
        class ItemDrop
        { public:
            float percentChance = 0.f;
            int amount = 0;
            std::string name;
        };
        std::vector<ItemDrop> drops;
        std::vector<ItemDrop> onDestroy;
        class FarmInfo
        { public:
            ItemDrop drop;
            unsigned int respawnTime = 1;
            std::string pendingTexture;
        };
        bool farmable = false;
        FarmInfo farmInfo;
    };

    class ItemDropInfo { public: std::string name; int num; };

    // static possibly interactive objects in the map
    class Entity : public Sprite 
    {
    public:
        Entity(const EntityType& etype);
        virtual ~Entity();

        inline std::string GetName() const { return name_; }
        inline int GetMaxClicks() const { return maxClicks_; }
        inline int GetRemainingClicks() const { return remainingClicks_; }
        inline void SetRemainingClicks(int r) { remainingClicks_ = r; }
        inline void DecRemainingClicks() { remainingClicks_--; }
        inline float GetClickTime() const { return clickTime_; }
        std::vector<ItemDropInfo> OnInteract();
        std::vector<ItemDropInfo> OnDestroy();
        inline bool IsFarmable() const { return farmable_; }
        inline bool IsReadyForPickup() const { return farmEvent_.readyForPickup; }
        time_t FarmTimeRemaining() const;
        ItemDropInfo Farm();
        void SetFarmData(const FarmCommand& fc);

        void Update(float dtime) override;
    private:
        std::string name_;
        int maxClicks_;
        int remainingClicks_;
        float clickTime_;
        class ItemDrop { public:
            float percentChance;
            int amount;
            std::string name;
        };
        std::vector<ItemDrop> onInteract_;
        std::vector<ItemDrop> onDestroy_;
        class FarmEvent { public:
            ItemDrop itemDrop;
            unsigned int respawnTimer;
            bool readyForPickup = true;
            time_t farmTimeStamp = 0;
            ogl::Texture* pendingTexture;
        };
        bool farmable_;
        FarmEvent farmEvent_;
        ogl::Texture* swapTexture_ = nullptr; // store other anim0_ texture
    };

}