// Entity.h
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2018
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

#include "ogl/Texture.h"
#include "Sprite.h"

struct ENTITY_TYPE 
{
    char* name = nullptr;
    char* texturePath = nullptr;
    int width = 0, height = 0;
    BOX collision = {0,0,0,0};
    int maxClicks = 0;
    float clickTime = 0.f; // in seconds!
    struct ITEM_DROP
    {
        float percentChance = 0.f;
        int amount = 0;
        char* name = nullptr;
    };
    int numDrops = 0;
    ITEM_DROP* drops = nullptr;
    int numOnDestroy = 0;
    ITEM_DROP* onDestroy;
};

class ItemDropInfo { public: std::string name; int num; };

// static possibly interactive objects in the map
class Entity : public Sprite 
{
public:
    Entity(const ENTITY_TYPE& etype);
    virtual ~Entity();

    inline std::string GetName() const { return name_; }
    inline int GetMaxClicks() const { return maxClicks_; }
    inline int GetRemainingClicks() const { return remainingClicks_; }
    inline void SetRemainingClicks(int r) { remainingClicks_ = r; }
    inline void DecRemainingClicks() { remainingClicks_--; }
    inline float GetClickTime() const { return clickTime_; }
    std::vector<ItemDropInfo> OnInteract();
    std::vector<ItemDropInfo> OnDestroy();
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
};