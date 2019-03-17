// Building.cpp
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.hpp"
#include "Building.hpp"

namespace game
{
    Building::Building(const BuildingEntry& entry, int x, int y, const HarvestData* harvestData,
                       const FarmData* farmData, const CraftData* craftData) 
        : Sprite(engine::GameEngine::Get().GetTextureManager().GetTexture(entry.texture), entry.width, 
        entry.height), entry_(entry)
    {
        position.x = (float)x;
        position.y = (float)y;
        collisionBox_.left = (float)entry.collision.left;
        collisionBox_.top = (float)entry.collision.top;
        collisionBox_.right = (float)entry.collision.right;
        collisionBox_.bottom = (float)entry.collision.bottom;
        // validate harvest parameters
        if((harvestData == nullptr && entry_.harvesting != nullptr) 
            || (harvestData != nullptr && entry.harvesting == nullptr))
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Building created with inconsistent harvest data!", __FUNCTION__);
        }
        // validate farm parameters
        if((farmData == nullptr && entry_.farming != nullptr)
            ||( farmData != nullptr && entry.farming == nullptr))
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Building created with inconsistent farm data!", __FUNCTION__);
        }
        // validate crafting parameters
        if((craftData == nullptr && entry_.crafting.size() != 0)
            ||(craftData != nullptr && entry_.crafting.size() == 0))
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Building created with inconsistent craft data!", __FUNCTION__);
        }
        // NOTE: if the object is created during game play, a timestamp of the current system time
        //       should fill fields requiring time_t as farmable buildings are never ready to farm
        //       upon creation.

        // fill in harvesting data as needed
        if(entry_.harvesting)
        {
            // For brand new objects, harvestData->remainingClicks should already be set to
            //  entry_.harvesting->maxClicks before sent to ctor
            remainingClicks_ = harvestData->remainingClicks;
        }

        // fill in farming data
        if(entry_.farming)
        {
            remainingFarms_ = farmData->remainingFarms;
            lastFarmTime_ = farmData->lastFarmTime;
            pendingFarm_ = engine::GameEngine::Get().GetTextureManager().GetTexture(entry_.farming->pending);
            // has the required amount of time passed since the last farm time? if so set the flag
            //  to indicate ready for pickup
            time_t currentTime = time(nullptr);
            if(currentTime - lastFarmTime_ >= entry_.farming->frequency)
            {
                readyForFarm_ = true;
                readyFarmTexture_ = anim0_;
                anim0_ = pendingFarm_;
            }
        }

        // fill in craft data
        if(entry_.crafting.size() != 0)
        {
            // NOTE: if the next field is "" this value can be anything
            craftingBegan_ = craftData->craftingBegan;
            craftingItem_ = craftData->itemCrafting;
            // if something is/was crafting, find it in the crafting list to determine
            //  the time needed to craft it compared to when it was started and what time it is now.
            //  Also validate that the item being crafted can be crafted inside this building.
            if(craftingItem_ != "")
            {
                const BuildingEntry::CraftingInfo* craftInfo = nullptr;
                int timeToCraft = 0;
                for(const auto& each : entry_.crafting)
                {
                    if(each.title == craftingItem_)
                    {
                        craftInfo = &each;
                        timeToCraft = each.time;
                        break;
                    }
                }
                // if the item title was not in the list of crafting options for this building, then
                //  something has gone horribly wrong
                if(craftInfo == nullptr)
                {
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                        "%s: Data integrity check failed. Cannot craft `%s' in building `%s'", __FUNCTION__,
                        craftingItem_.c_str(), entry_.name.c_str());
                }
                // grab the textures associated with pending or ready state for this item.
                //  NOTE: these textures should changed when a different item is crafted
                pendingCrafting_ = engine::GameEngine::Get().GetTextureManager().GetTexture(craftInfo->pending);
                completedCrafting_ = engine::GameEngine::Get().GetTextureManager().GetTexture(craftInfo->completed);
                currentCraftTexture_ = pendingCrafting_; // assume it's not ready unless next statement otherwise
                // validation passed so check current time against craft start time to see if it's ready for pickup
                time_t currentTime = time(nullptr);
                if(currentTime - craftingBegan_ >= craftInfo->time)
                {
                    readyForPickup_ = true;
                    currentCraftTexture_ = completedCrafting_;
                }
            }
            // load the associated crafting textures and create the crafting quad. If nothing is being crafted
            //  then the quad will not render so null textures will be safe. For engine simplicity, these
            //  quads are the exact size of the building so one must consider that when implementing the
            //  textures.
            float w = (float)entry_.width;
            float h = (float)entry_.height;
            ogl::Vertex craftverts[6] = {
                {{0.f,0.f,0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
                {{0.f,  h,0.f},{255,255,255,255},{0.f,1.f},{0.f,0.f,1.f}},
                {{  w,  h,0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
                {{0.f,0.f,0.f},{255,255,255,255},{0.f,0.f},{0.f,0.f,1.f}},
                {{  w,  h,0.f},{255,255,255,255},{1.f,1.f},{0.f,0.f,1.f}},
                {{  w,0.f,0.f},{255,255,255,255},{1.f,0.f},{0.f,0.f,1.f}}
            };
            craftVbo_ = new ogl::VertexBuffer();
            craftVbo_->SetData(6*sizeof(ogl::Vertex), craftverts, GL_STATIC_DRAW);
            ogl::VertexBufferLayout vbl;
            ogl::Vertex::PushLayout(vbl);
            craftVao_ = new ogl::VertexArray();
            craftVao_->AddBuffer(*craftVbo_, vbl);
        }
    }

    Building::~Building()
    {
        // check to make sure crafting quad was NOT created for non-crafting buildings
        if(entry_.crafting.size() == 0 && craftVbo_ && craftVao_)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Data integrity check failed! Crafting quad was created for a non-crafting building",
                __FUNCTION__);
        }
        delete craftVbo_;
        delete craftVao_;
    }

    void Building::Update(float dtime)
    {
        Sprite::Update(dtime);

        dataCheckTimer_ += dtime;
        if(dataCheckTimer_ < DATA_CHECK_TIME)
            return; // nothing to check yet
        
        dataCheckTimer_ -= DATA_CHECK_TIME;

        time_t currentTime = time(nullptr);

        // check farming
        if(entry_.farming)
        {
            if(currentTime - lastFarmTime_ >= entry_.farming->frequency)
            {
                readyForFarm_ = true;
                anim0_ = readyFarmTexture_;
            }
        }
        // check crafting
        if(entry_.crafting.size() != 0)
        {
            // is something being crafted?
            if(craftingItem_ != "")
            {
                // find out what's being crafted, how long it takes to craft and compare it to
                //  current data to determine if it's ready for pickup and the texture should change
                const BuildingEntry::CraftingInfo* craftingInfo = nullptr;
                int craftRequiredTime;
                for(const auto& each : entry_.crafting)
                {
                    if(each.title == craftingItem_)
                    {
                        craftingInfo = &each;
                        craftRequiredTime = each.time;
                        break;
                    }
                }
                // if nothing was found, then something has gone horribly wrong and the building craft
                //  data is in an invalid state
                if(craftingInfo == nullptr)
                {
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                        "%s: Data integrity failure. Item `%s' is being crafted in building `%s'",
                        __FUNCTION__, craftingItem_.c_str(), entry_.name.c_str());
                }
                // check for crafting completion and if complete, change texture on craft quad
                if(currentTime - craftingBegan_ > craftingInfo->time)
                {
                    readyForPickup_ = true;
                    currentCraftTexture_ = completedCrafting_;
                }
            }
        }
    }

    void Building::Render(const glm::vec2& camera, ogl::Program& program)
    {
        Sprite::Render(camera, program);
        // extra render work is only needed for crafting buildings with something being crafted.
        if(craftingItem_ != "")
        {
            // craftingItem_ should always remain "" for non-crafting buildings
            if(entry_.crafting.size() == 0)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                    "%s: Data integrity failure. craftingItem_ set to `%s' in building `%s'", __FUNCTION__,
                    craftingItem_.c_str(), entry_.name.c_str());
                return;
            }
            // draw the quad
            currentCraftTexture_->Bind(); // this will NEVER be null for a crafting building
            glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(
                camera.x + position.x, camera.y + position.y, 0.f));
            program.SetUniform("u_model", model);
            craftVao_->Bind(); // this will NEVER be null for a crafting building
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    bool Building::Construct(Inventory& inv)
    {
        // first check item requirements against what is in inventory.
        for(const auto& req : entry_.required)
        {
            if(inv.GetItemCount(req.name) < req.count)
                return false;
        }
        // check passed
        for(const auto& req : entry_.required)
        {
            inv.RemoveItem(req.name, req.count);
        }
        return true;
    }

    Building::Interaction Building::GetInteraction()
    {
        // interaction type is prioritized as follows, 
        // if craftable, crafting pickup if ready, crafting select start if not, 
        // if farmable, farm,
        // and if harvestable, harvest. 
        // So for example, if a building were to meet all 3 conditions (unlikely) it would only be
        //  farmable, and then harvestable, when something is being crafted in the building. In such
        //  a strange case, the caller would need to call IsHarvestable, IsFarmable to find out more
        //  information on what Interact will do because Interact does NOT start the crafting process.

        // first check crafting state
        if(entry_.crafting.size() != 0)
        {
            if(readyForPickup_)
                return Interaction::PickupCrafted;
            else if(craftingItem_ == "")
                return Interaction::ReadyToCraft;
            // else something is being crafted so continue to farm and harvest checks
        }
        if(entry_.farming)
        {
            // for the case of remainingFarms_, caller should change the building according
            //  to the entry farm data.
            if(readyForFarm_ && remainingFarms_ > 0)
                return Interaction::Farm;
            // else farmable item is still pending so it MAY be harvestable instead
        }
        if(entry_.harvesting)
        {
            // then this can be harvested. When a harvest is maxed out, other methods would destroy
            //  this building so there is no need to check maxClicks_ here
            return Interaction::Harvest;
        }
        // if we reach this point, there is no meaningful interaction to be done on the building
        return Interaction::None;
    }

    LootTable Building::Interact()
    {
        LootTable lt;
        // check the meaning of interact.
        Interaction interaction = GetInteraction();
        if(interaction == Interaction::PickupCrafted)
        {   // this means we're going to pick up the item(s) that were crafted, change the building states
            //  and return true.
            // First, find the item in the crafting list
            const BuildingEntry::CraftingInfo* craftingInfo = nullptr;
            for(const auto& each : entry_.crafting)
            {
                if(each.title == craftingItem_)
                {
                    craftingInfo = &each;
                    break;
                }
            }
            // with the amount of data checking in other methods this shouldn't be null but ok to check
            if(craftingInfo == nullptr)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                    "%s: `%s' would be ready to pick up except IT DOESN'T EXIST OR IS INVALID!", 
                    __FUNCTION__, craftingItem_.c_str());
                return lt;
            }
            // add the results to inventory
            for(const auto& each: craftingInfo->results)
            {
                if(engine::GameEngine::Get().PercentChance(each.chance))
                {
                    lt.push_back({each.item, each.count});
                }
            }
            // change the crafting data and building state
            craftingItem_ = "";
            readyForPickup_ = false;
            // (we don't need to change the texture because the craft quad will not be rendered unless
            //  another item starts crafting, in which case the texture will be changed there)
            // (there is no need to change the crafting start timestamp because it is not interpreted
            //  unless something is being crafted, so when another item starts crafting it will be set)
            return lt; // everything went ok
        }
        // ignore case for Interaction::ReadyToCraft - a separate method must handle that
        else if(interaction == Interaction::Farm)
        {   // this means the farmable item will be collected and the farm state reset
            if(entry_.farming == nullptr)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                    "%s: I don't know how we got here but farming is null when it shouldn't be...", 
                    __FUNCTION__);
                return lt; // loot table is still empty at this point
            }
            // check remaining farms. -1 max means infinite farms
            if(remainingFarms_ <= 0 && entry_.farming->maxFarms != -1)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                    "%s: Invalid farm state, remaining farm interactions are <= 0", __FUNCTION__);
                return lt;
            }
            remainingFarms_--;
            lastFarmTime_ = time(nullptr);
            readyForFarm_ = false;
            anim0_ = pendingFarm_;
            // add the farm loot
            for(const auto& each : entry_.farming->drops)
            {
                if(engine::GameEngine::Get().PercentChance(each.chance))
                {
                    lt.push_back({each.item, each.count});
                }
            }
            return lt;
        }
        else if(interaction == Interaction::Harvest)
        {
            if(entry_.harvesting == nullptr)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                    "%s: I don't know how we got here but harvesting is null when it shouldn't be!",
                    __FUNCTION__);
                return lt;
            }
            // check remaining harvests (no such thing as infinite harvests)
            if(remainingClicks_ <= 0)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                    "%s: Invalid harvest state, remaining clicks are <= 0", __FUNCTION__);
                return lt;
            }
            remainingClicks_--;
            for(const auto& each : entry_.harvesting->drops)
            {
                if(engine::GameEngine::Get().PercentChance(each.chance))
                {
                    lt.push_back({each.item, each.count});
                }
            }
            return lt;
        }
        // Interact shouldn't be called when caller knows that the interact is pointless, so print
        //  a warning if we get to this point.
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
            "%s: There was no valid interaction to occur!", __FUNCTION__);
        return lt;
    }

    bool Building::CraftBegin(Inventory& inv, const std::string& itemToCraft)
    {
        // first validate that this is a crafting building.
        if(entry_.crafting.size() == 0)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Building `%s' is not for crafting!", __FUNCTION__, entry_.name.c_str());
            return false;
        }
        // only one thing at a time can be crafted in buildings!
        if(craftingItem_ != "")
        {
            if(!readyForPickup_)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                    "%s: Must wait for `%s' to finish crafting first!", 
                    __FUNCTION__, craftingItem_.c_str());
                return false;
            }
            else 
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                    "%s: Must pick up `%s' before crafting another item!", 
                    __FUNCTION__, craftingItem_.c_str());
                return false;
            }
        }
        // building is ready to start crafting so look for item to craft in list
        const BuildingEntry::CraftingInfo* craftingInfo = nullptr;
        for(const auto& each : entry_.crafting)
        {
            if(each.title == itemToCraft)
            {
                craftingInfo = &each;
                break;
            }
        }
        if(craftingInfo == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Cannot craft `%s' in building `%s'", __FUNCTION__,
                itemToCraft.c_str(), entry_.name.c_str());
            return false;
        }
        // everything passes so set data
        craftingBegan_ = time(nullptr);
        craftingItem_ = itemToCraft;
        readyForPickup_ = false;
        // update textures
        pendingCrafting_ = engine::GameEngine::Get().GetTextureManager().GetTexture(craftingInfo->pending);
        completedCrafting_=engine::GameEngine::Get().GetTextureManager().GetTexture(craftingInfo->completed);
        currentCraftTexture_ = pendingCrafting_;
        return true;
    }

    LootTable Building::Remove()
    {
        LootTable lt;
        if(entry_.removing == nullptr)
            return lt;

        for(const auto& each : entry_.removing->drops)
        {
            if(engine::GameEngine::Get().PercentChance(each.chance))
            {
                lt.push_back({each.item, each.count});
            }
        }
        return lt;
    }
}