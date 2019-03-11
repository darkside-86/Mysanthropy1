// Battle.cpp
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

#include "Battle.hpp"

namespace combat
{

    Battle::Battle()
    {

    }

    Battle::~Battle()
    {

    }

    void Battle::AddMob(game::MobSprite* mobSprite)
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

    void Battle::RemoveMob(game::MobSprite* mobSprite)
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

    void Battle::AddPlayer(game::PlayerSprite* playerSprite)
    {
        // only one player for now
        playerSprite_ = playerSprite;
    }

    std::string Battle::UsePlayerAbility(const std::string& abilityName, game::Target& target)
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
            {
                AddMob(targetedMob);
                // play the animation if mob was hit
                const Ability ab = playerSprite_->GetPlayerCombatUnit().GetAbilityByName(abilityName);
                if(ab.animation != "") // make sure there is an animation to actually play
                {
                    animationSystem_.AddAndStartNewAnimation(ab.name, playerSprite_->GetPlayerCombatUnit(), 
                        targetedMob->GetCombatUnit(), playerSprite_->GetWidth());
                }
            }
        }
        else
        {
            // probably targeting an Entity or something.
            combatLogEntry = "Invalid target";
        }
        return combatLogEntry;
    }

    std::vector<std::string> Battle::CalculateMoves()
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
                            // play the associated animation.
                            animationSystem_.AddAndStartNewAnimation(ab.animation, eachMob->GetCombatUnit(),
                                playerSprite_->GetPlayerCombatUnit(), eachMob->GetWidth());
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
            // this mob might be somebody's spell animation target. 
            animationSystem_.InvalidateAnimationTarget(markedForDeletion->GetCombatUnit());
            // this mob might be the source unit for a target.
            animationSystem_.InvalidateAnimationSource(markedForDeletion->GetCombatUnit());
        }
        // check to see if player died, and if so, clear out mob list (caller will manage player death)
        if(playerSprite_->GetPlayerCombatUnit().GetCurrentHealth() == 0)
        {
            for(auto each: mobSprites_)
            {
                each->GetCombatUnit().SetInCombat(false);
            }
            mobSprites_.clear();
            // the player is probably the source of a combat animation
            animationSystem_.InvalidateAnimationSource(playerSprite_->GetPlayerCombatUnit());
            // don't want projectiles following player to the spawn point
            animationSystem_.InvalidateAnimationTarget(playerSprite_->GetPlayerCombatUnit());
        }
        // if no mobs are in list, set out of combat to true for player
        if(mobSprites_.size() == 0)
            playerSprite_->GetPlayerCombatUnit().SetInCombat(false);
        // TODO: one pass of removing a mob from combat if distance from origin is greater than leash
        return combatLogEntries;
    }

    const Ability Battle::PickRandomAbility(CombatUnit& unit)
    {
        const auto& abilityTable = unit.GetAbilities();
        const std::vector<std::pair<std::string, Ability> > listView(abilityTable.begin(), abilityTable.end());
        auto &rng = engine::GameEngine::Get().GetRNG();
        return listView[rng() % listView.size()].second;
    }

}