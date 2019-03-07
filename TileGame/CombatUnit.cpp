// CombatUnit.cpp
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
#include "CombatUnit.hpp"

CombatUnit::CombatUnit(Configuration& config, bool player, int level, const CombatAbilityList& abilities, 
                        const std::string& name)
    : abilities_(abilities), name_(name), statSheet_(level, player, config)
{
    maxHealth_ = statSheet_.GetMaxHealth();
    currentHealth_ = maxHealth_;
}

CombatUnit::~CombatUnit()
{

}

bool CombatUnit::UseAbility(CombatUnit& other, bool targetIsFriendly,
    const std::string& abilityName, std::string& combatLogEntry)
{
    auto found = abilities_.find(abilityName);
    if(found != abilities_.end())
    {
        CombatAbility &ab = found->second;
        // check target and ability friendliness
        if((ab.offensive && targetIsFriendly) || (!ab.offensive && !targetIsFriendly)  )
        {
            combatLogEntry = std::string("Cannot use ") + abilityName + " on " + other.name_;
            return false;
        }
        // check global CD
        if( ab.onGCD && globalCooldownCounter_ < GCD )
        {
            combatLogEntry = std::string("Cannot use ") + abilityName + " yet (waiting on global cooldown)";
            return false;
        }
        // check ability's cooldown in the CD table
        if(ab.timer < ab.cooldown)
        {
            combatLogEntry = abilityName + " is still on cooldown for " + std::to_string(ab.cooldown - ab.timer)
                + " more seconds.";
            return false;
        }
        // check range
        float distance = glm::distance(location_, other.location_);
        if(distance < ab.minRange)
        {
            combatLogEntry = "Target too close";
            return false;
        }
        if(distance > ab.maxRange)
        {
            combatLogEntry = "Target too far away";
            return false;
        }
        // TODO: account for target's armor and resistances and chances to miss/dodge/parry
        Damage damageOrHealing = ab.calculateBaseDamage(*this);
        other.currentHealth_ -= damageOrHealing.amount;
        if(ab.offensive)
        {
            combatLogEntry = name_ + "'s " + abilityName + " hit " + other.name_ + " for "
                + std::to_string(damageOrHealing.amount) + " damage";
        }
        else 
        {
            combatLogEntry = name_ + "'s " + abilityName + " healed " + other.name_ + " for " 
                + std::to_string(damageOrHealing.amount) + " healing";
        }
        // set the cooldown of the ability to 0
        ab.timer = 0.0f;
        // set the global cooldown timer
        globalCooldownCounter_ = 0.0f;
        // determine overkill or overheal amount if any
        int overkillOrHeal;
        if(other.currentHealth_ > other.maxHealth_)
        {
            overkillOrHeal = -(other.currentHealth_ - other.maxHealth_);
            other.currentHealth_ = other.maxHealth_;
        }
        else if(other.currentHealth_ < 0)
        {
            overkillOrHeal = -other.currentHealth_;
            other.currentHealth_ = 0;
        }
        // todo: add overheal or overkill to log
        return true;
    }
    else
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
            "%s: Unit does not have ability `%s'", __FUNCTION__, abilityName.c_str());
    }
    return false;
}

bool CombatUnit::AbilityInRange(CombatUnit& other, const std::string& abilityName)
{
    float distance = glm::distance(location_, other.location_);
    auto found = abilities_.find(abilityName);
    if(found != abilities_.end())
    {
        CombatAbility &ab = found->second;
        return !(distance < ab.minRange || distance > ab.maxRange);
    }
    else
    {
        return false;
    }
}

bool CombatUnit::AbilityIsReady(const std::string& abilityName)
{
    auto found = abilities_.find(abilityName);
    if(found == abilities_.end())
        return false; // "NULL" ability is never ready
    return found->second.timer >= found->second.cooldown;
}

bool CombatUnit::GlobalCooldownIsOff()
{
    return globalCooldownCounter_ >= GCD;
}

void CombatUnit::Update(float dtime)
{
    // GCD check
    globalCooldownCounter_ += dtime;
    if(globalCooldownCounter_ > GCD)
        globalCooldownCounter_ = GCD;
    // individual ability's cooldowns
    for(auto each = abilities_.begin(); each != abilities_.end(); ++each)
    {
        each->second.timer += dtime;
        if(each->second.timer > each->second.cooldown)
            each->second.timer = each->second.cooldown;
    }
    // recovery if out of combat (clear aggro table)
    if(!inCombat_)
    {
        if(currentHealth_ < maxHealth_)
        {
            healthRecoveryTimer_ += dtime;
            if(healthRecoveryTimer_ >= RECOVERY_RATE)
            {
                healthRecoveryTimer_ -= RECOVERY_RATE;
                currentHealth_ += RECOVERY_AMOUNT;
                if(currentHealth_ > maxHealth_)
                    currentHealth_ = maxHealth_;
            }
        }
    }
}
