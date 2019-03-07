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

BattleSystem::BattleSystem()
{

}

BattleSystem::~BattleSystem()
{

}

void BattleSystem::AddMob(MobSprite* mobSprite)
{
    // if mob not already in list add it
    auto found = std::find_if(mobSprites_.begin(), mobSprites_.end(), [mobSprite](const MobSprite* spr){
        return (spr == mobSprite);
    });
    if(found == mobSprites_.end())
    {
        mobSprite->GetCombatUnit().SetInCombat(true);
        mobSprites_.push_back(mobSprite);
    }
    playerSprite_->GetPlayerCombatUnit().SetInCombat(true);
}

void BattleSystem::RemoveMob(MobSprite* mobSprite)
{
    auto found = std::find_if(mobSprites_.begin(), mobSprites_.end(), [mobSprite](const MobSprite* spr){
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

void BattleSystem::AddPlayer(PlayerSprite* playerSprite)
{
    // only one player for now
    playerSprite_ = playerSprite;
}

std::string BattleSystem::UsePlayerAbility(const std::string& abilityName, Target& target)
{
    // make sure there is a target.
    std::string combatLogEntry = "???"; // return value should never be "???"
    if(target.GetTargetSprite() == nullptr)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
            "No target selected!");
        combatLogEntry = "No target selected!";
        return combatLogEntry;
    }

    // check to see if this is a new target and then use the attack. if successfully used
    // the mob is now "aggro"'d and should be added to battle table
    if(target.GetTargetSpriteType() == Target::SPRITE_TYPE::MOBSPR)
    {
        MobSprite* targetedMob = (MobSprite*)target.GetTargetSprite();
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
    //  or use the attack. Mobs will always choose "attack" for now
    MobSprite* markedForDeletion = nullptr;
    for(auto eachMob : mobSprites_)
    {
        // check to make sure mob is alive.
        if(eachMob->GetCombatUnit().GetCurrentHealth() == 0)
        {
            markedForDeletion = eachMob;
        }
        // check range of attack
        else if(!eachMob->GetCombatUnit().AbilityInRange((CombatUnit)playerSprite_->GetPlayerCombatUnit(), "attack"))
        {
            // if out of range set velocity to move toward player.
            eachMob->SetVelocity(playerSprite_->GetPosition() - eachMob->GetPosition());
        }
        else 
        {
            eachMob->SetVelocity({0.0f, 0.0f, 0.0f}); // no use moving if in range
            // in range so check to see if global cooldown is off
            if(eachMob->GetCombatUnit().GlobalCooldownIsOff())
            {
                // GCD is ready so check ability cooldown
                if(eachMob->GetCombatUnit().AbilityIsReady("attack"))
                {
                    // all conditions are met so use offensive attack on player
                    std::string combatLogEntry;
                    eachMob->GetCombatUnit().UseAbility(playerSprite_->GetPlayerCombatUnit(), false, 
                        "attack", combatLogEntry);
                    combatLogEntries.push_back(combatLogEntry);
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
    // if no mobs are in list, set out of combat to true for player
    if(mobSprites_.size() == 0)
        playerSprite_->GetPlayerCombatUnit().SetInCombat(false);
    // TODO: one pass of removing a mob from combat if distance from origin is greater than leash
    return combatLogEntries;
}