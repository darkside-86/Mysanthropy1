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
    char* name;
    char* texturePath;
    int width, height;
    BOX collision;
    int maxClicks;
    struct ITEM_DROP
    {
        float percentChance;
        int amount;
        char* name;
    };
    int numDrops;
    ITEM_DROP* drops;
    ITEM_DROP onDestroy;
};

// static possibly interactive objects in the map
class Entity : public Sprite 
{
public:
    Entity(ogl::Texture* anim0, int w=0, int h=0);
    virtual ~Entity();
private:
    std::string name;
    int maxClicks;
    int remainingClicks;
};