// Entity.cpp
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

#include "Entity.h"

#include "engine/GameEngine.h"

Entity::Entity(const ENTITY_TYPE& etype)
    : Sprite(engine::GameEngine::Get().GetTextureManager().GetTexture(etype.texturePath), 
      etype.width, etype.height), name_(etype.name), maxClicks_(etype.maxClicks), 
      remainingClicks_(etype.maxClicks), clickTime_(etype.clickTime)
{
    SetCollisionBox(etype.collision.left, etype.collision.top, etype.collision.right, etype.collision.bottom);
    for(int i=0; i < etype.numDrops; ++i)
    {
        onInteract_.push_back({etype.drops[i].percentChance,
            etype.drops[i].amount, etype.drops[i].name});
    }
    for(int i=0; i < etype.numOnDestroy; ++i)
    {
        onDestroy_.push_back({etype.onDestroy[i].percentChance,
            etype.onDestroy[i].amount, etype.onDestroy[i].name});
    }
}

Entity::~Entity()
{

}

std::vector<ItemDropInfo> Entity::OnInteract()
{
    std::vector<ItemDropInfo> infos;
    auto rng = engine::GameEngine::Get().GetRNG();
    for(auto each : onInteract_)
    {
        float chance = (float)rng() / (float)rng.max();
        if(chance <= each.percentChance)
        {
            ItemDropInfo info;
            info.name = each.name;
            info.num = each.amount;
            infos.push_back(info);
        }
    }
    return infos;
}

std::vector<ItemDropInfo> Entity::OnDestroy()
{
    std::vector<ItemDropInfo> infos;
    auto rng = engine::GameEngine::Get().GetRNG();
    for(auto each : onDestroy_)
    {
        float chance = (float)rng() / (float)rng.max();
        if(chance <= each.percentChance)
        {
            ItemDropInfo info;
            info.name = each.name;
            info.num = each.amount;
            infos.push_back(info);
        }
    }
    return infos;
}