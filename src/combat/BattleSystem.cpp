// BattleSystem.cpp
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

#include "BattleSystem.hpp"

namespace combat
{

    BattleSystem::BattleSystem()
    {

    }

    BattleSystem::~BattleSystem()
    {

    }

    void BattleSystem::AddMob(game::MobSprite* mobSprite)
    {
        // if mob not already in list add it
        auto found = std::find_if(mobSprites_.begin(), mobSprites_.end(), [mobSprite](const game::MobSprite* spr){
            return (spr == mobSprite);
        });
        if(found == mobSprites_.end())
        {
            mobSprite->GetCombatUnit().SetInCombat(true);
            mobSprites_.push_back(mobSprite);
        }
        playerSprite_->GetPlayerCombatUnit().SetInCombat(true);
    }

    void BattleSystem::RemoveMob(game::MobSprite* mobSprite)
    {
        // look for mob and remove if found
        auto found = std::find_if(mobSprites_.begin(), mobSprites_.end(), [mobSprite](const game::MobSprite* spr){
            return (spr == mobSprite);
        });
        if(found != mobSprites_.end())
        {
            (*found)->GetCombatUnit().SetInCombat(false);
            mobSprites_.erase(found);
        }
        if(mobSprites_.size() == 0)
        {
            playerSprite_->GetPlayerCombatUnit().SetInCombat(false);
        }
    }

    void BattleSystem::AddPlayer(game::PlayerSprite* playerSprite)
    {
        // only one player for now
        playerSprite_ = playerSprite;
    }

    std::string BattleSystem::UsePlayerAbility(const std::string& abilityName, game::Target& target)
    {
        std::string combatLogEntry = ""; // for clarity initialize to blank to indicate nothing gets printed
                                         // in some cases (such as hitting button while on cooldown)
        // make sure there is a target.
        if(target.GetTargetSprite() == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "No target selected!");
            combatLogEntry = "No target selected!";
            return combatLogEntry;
        }

        // check to see if target is a mob and then use the attack. if successfully used
        // the mob is now "aggro"'d and should be added to battle table
        if(target.GetTargetSpriteType() == game::Target::SPRITE_TYPE::MOBSPR)
        {
            game::MobSprite* targetedMob = (game::MobSprite*)target.GetTargetSprite();
            bool hit =  playerSprite_->GetPlayerCombatUnit().UseAbility(targetedMob->GetCombatUnit(), false,
                abilityName, combatLogEntry);
            if(hit)
                AddMob(targetedMob);
        }
        else
        {
            // probably targeting an Entity or something.
            combatLogEntry = "Invalid target";
        }
        return combatLogEntry;
    }

    std::vector<std::string> BattleSystem::CalculateMoves()
    {
        std::vector<std::string> combatLogEntries;
        // iterate through mobs and either attempt to move close enough to player for chosen attack
        //  or use the attack.
        game::MobSprite* markedForDeletion = nullptr;
        for(auto eachMob : mobSprites_)
        {
            // check to make sure mob is alive.
            if(eachMob->GetCombatUnit().GetCurrentHealth() == 0)
            {
                markedForDeletion = eachMob;
            }
            else 
            {
                // pick a random attack to try to use
                const auto& ab = PickRandomAbility(eachMob->GetCombatUnit());
                // if out of range set velocity to move toward player.
                if(!eachMob->GetCombatUnit().AbilityInRange(playerSprite_->GetPlayerCombatUnit(), ab.name))
                {
                    eachMob->velocity = playerSprite_->position - eachMob->position;
                }
                else // check to see if random ability and GCD are off CD and use them if so
                {
                    eachMob->velocity = {0.0f, 0.0f}; // no use moving if in range
                    // in range so check to see if global cooldown is off
                    if(eachMob->GetCombatUnit().GlobalCooldownIsOff())
                    {
                        // GCD is ready so check ability cooldown
                        if(eachMob->GetCombatUnit().AbilityIsReady(ab.name))
                        {
                            // all conditions are met so use offensive attack on player
                            std::string combatLogEntry;
                            eachMob->GetCombatUnit().UseAbility(playerSprite_->GetPlayerCombatUnit(), false, 
                                ab.name, combatLogEntry);
                            combatLogEntries.push_back(combatLogEntry);
                        }
                    }
                }
            }
        }
        // remove dead mobs one cycle at a time (dead mobs can't attack no matter how many there are)
        if(markedForDeletion) 
        {
            markedForDeletion->SetKilledByPlayer(true);
            RemoveMob(markedForDeletion);
        }
        // check to see if player died, and if so, clear out mob list (caller will manage player death)
        if(playerSprite_->GetPlayerCombatUnit().GetCurrentHealth() == 0)
        {
            for(auto each: mobSprites_)
            {
                each->GetCombatUnit().SetInCombat(false);
            }
            mobSprites_.clear();
        }
        // if no mobs are in list, set out of combat to true for player
        if(mobSprites_.size() == 0)
            playerSprite_->GetPlayerCombatUnit().SetInCombat(false);
        // TODO: one pass of removing a mob from combat if distance from origin is greater than leash
        return combatLogEntries;
    }

    const Ability BattleSystem::PickRandomAbility(CombatUnit& unit)
    {
        const auto& abilityTable = unit.GetAbilities();
        const std::vector<std::pair<std::string, Ability> > listView(abilityTable.begin(), abilityTable.end());
        auto &rng = engine::GameEngine::Get().GetRNG();
        return listView[rng() % listView.size()].second;
    }

}