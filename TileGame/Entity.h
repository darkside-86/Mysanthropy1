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

#include "ogl/Texture.h"
#include "Sprite.h"

struct ENTITY_TYPE 
{
    char* name = nullptr;
    char* texturePath = nullptr;
    int width = 0, height = 0;
    BOX collision = {0,0,0,0};
    int maxClicks = 0;
    struct ITEM_DROP
    {
        float percentChance = 0.f;
        int amount = 0;
        char* name = "";
    };
    int numDrops = 0;
    ITEM_DROP* drops = nullptr;
    ITEM_DROP onDestroy;
};

// static possibly interactive objects in the map
class Entity : public Sprite 
{
public:
    Entity(const ENTITY_TYPE& etype);
    virtual ~Entity();

    std::string GetName() { return name_; }
private:
    std::string name_;
    int maxClicks_;
    int remainingClicks_;
};