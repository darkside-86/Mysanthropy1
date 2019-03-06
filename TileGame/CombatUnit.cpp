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

CombatUnit::CombatUnit(bool attackable, const CombatAbilityList& abilities, const std::string& name)
    : attackable_(attackable), abilities_(abilities), name_(name)
{

}

CombatUnit::~CombatUnit()
{

}

int CombatUnit::UseAbility(CombatUnit& other, const std::string& abilityName, std::string& combatLogEntry)
{
    auto found = abilities_.find(abilityName);
    if(found != abilities_.end())
    {
        CombatAbility &ab = found->second;
        // check target and ability friendliness
        if( !((ab.offensive && other.attackable_) || (!ab.offensive && !other.attackable_) ) )
        {
            combatLogEntry = std::string("Cannot use ") + abilityName + " on " + other.name_;
            return 0;
        }
        // check global CD
        if( ab.onGCD && globalCooldownCounter_ < GCD )
        {
            combatLogEntry = std::string("Cannot use ") + abilityName + " yet (waiting on global cooldown)";
            return 0;
        }
        // check ability's cooldown in the CD table
        if(ab.timer < ab.cooldown)
        {
            combatLogEntry = abilityName + " is still on cooldown for " + std::to_string(ab.cooldown - ab.timer)
                + " more seconds.";
            return 0;
        }
        // check range
        float distance = glm::distance(location_, other.location_);
        if(distance < ab.minRange)
        {
            combatLogEntry = "Target too close";
            return 0;
        }
        if(distance > ab.maxRange)
        {
            combatLogEntry = "Target too far away";
            return 0;
        }
        // TODO: account for target's armor and resistances and chances to miss/dodge/parry
        int damageOrHealing = ab.calculateBaseDamage(*this);
        other.currentHealth_ -= damageOrHealing;
        if(other.attackable_)
        {
            combatLogEntry = name_ + "'s " + abilityName + " hit " + other.name_ + " for "
                + std::to_string(damageOrHealing) + " damage";
        }
        else 
        {
            combatLogEntry = name_ + "'s " + abilityName + " healed " + other.name_ + " for " 
                + std::to_string(damageOrHealing) + " healing";
        }
        // set the cooldown of the ability to 0
        ab.timer = 0.0f;
        // set the global cooldown timer
        globalCooldownCounter_ = 0.0f;
        // determine overkill or overheal amount if any
        int overkillOrHeal =0;
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

        // upon dying, the target should remove itself from the aggro table
        // ...
        auto it=aggroTable_.begin();
        for(; it != aggroTable_.end(); ++it)
        {
            if(&other == *it)
                break;
        }
        if(it != aggroTable_.end())
            aggroTable_.erase(it);

        return overkillOrHeal;
    }
    else
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
            "%s: Unit does not have ability `%s'", __FUNCTION__, abilityName.c_str());
    }
    return 0;
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
    if(aggroTable_.size() == 0)
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

void CombatUnit::StopCombatWith(CombatUnit& target)
{
    // look for self in target's aggro table and remove if there
    auto it = target.aggroTable_.begin();
    for(; it != target.aggroTable_.end(); ++it)
    {
        if(this == (*it))
            break;
    }
    if( it != target.aggroTable_.end())
        target.aggroTable_.erase(it);
}