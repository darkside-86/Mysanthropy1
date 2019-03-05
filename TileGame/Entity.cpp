// Entity.cpp
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

#include "engine/GameEngine.hpp"
#include "Entity.hpp"

Entity::Entity(const ENTITY_TYPE& etype)
    : Sprite(engine::GameEngine::Get().GetTextureManager().GetTexture(etype.texturePath), 
      etype.width, etype.height), name_(etype.name), maxClicks_(etype.maxClicks), 
      remainingClicks_(etype.maxClicks), clickTime_(etype.clickTime), farmable_(etype.farmable)
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
    if(farmable_)
    {
        farmInfo_.itemDrop.amount = etype.farmInfo.drop.amount;
        farmInfo_.itemDrop.name = etype.farmInfo.drop.name;
        farmInfo_.itemDrop.percentChance = etype.farmInfo.drop.percentChance;
        farmInfo_.respawnTimer = etype.farmInfo.respawnTime;   
        farmInfo_.farmTimeStamp = 0;
        farmInfo_.pendingTexture = engine::GameEngine::Get().GetTextureManager().GetTexture(
            etype.farmInfo.pendingTexture);
        farmInfo_.readyForPickup = true;
        farmInfo_.respawnTimer = etype.farmInfo.respawnTime; 
        swapTexture_ = anim0_;
    }
}

Entity::~Entity()
{

}

void Entity::Update(float dtime)
{
    Sprite::Update(dtime);
    if(farmable_)
    {
        if(!farmInfo_.readyForPickup)
        {
            time_t remaining = FarmTimeRemaining();
            if(remaining <= 0)
            {
                farmInfo_.readyForPickup = true;
                anim0_ = swapTexture_;
            }
        }
    }
}

std::vector<ItemDropInfo> Entity::OnInteract()
{
    std::vector<ItemDropInfo> infos;
    auto& rng = engine::GameEngine::Get().GetRNG();
    for(auto each : onInteract_)
    {
        float chance = 100.0f * (float)rng() / (float)rng.max();
        // engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
        //    "%s: chance = %f", __FUNCTION__, chance);
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
    auto& rng = engine::GameEngine::Get().GetRNG();
    for(auto each : onDestroy_)
    {
        float chance = 100.f * (float)rng() / (float)rng.max();
        // engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO,
        //    "%s: chance = %f", __FUNCTION__, chance);
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

time_t Entity::FarmTimeRemaining() const
{
    time_t currentTime = time(nullptr);
    time_t elapsed = currentTime - farmInfo_.farmTimeStamp;
    time_t remaining = farmInfo_.respawnTimer - elapsed;
    return remaining;
}

void Entity::SetFarmData(const FarmCommand& fc)
{
    farmInfo_.readyForPickup = fc.readyToFarm;
    if(farmInfo_.readyForPickup)
    {
        anim0_ = swapTexture_;
    }
    else
    {
        anim0_ = farmInfo_.pendingTexture;
    }
    farmInfo_.farmTimeStamp = fc.farmedTime;
}

ItemDropInfo Entity::Farm()
{
    ItemDropInfo info = { "null", 0 };
    if(!farmable_)
        return info;
    farmInfo_.readyForPickup = false;
    anim0_ = farmInfo_.pendingTexture;
    info.name = farmInfo_.itemDrop.name;
    info.num = farmInfo_.itemDrop.amount;
    farmInfo_.farmTimeStamp = time(nullptr);
    return info;
}