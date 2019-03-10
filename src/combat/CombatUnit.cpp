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

namespace combat
{

    CombatUnit::CombatUnit(bool player, int level, const AbilityTable& abilities, 
                            const std::string& name)
        : abilities_(abilities), name_(name), attributeSheet_(level, player)
    {
        maxHealth_ = attributeSheet_.GetMaxHealth();
        currentHealth_ = maxHealth_;
        // spells are ready upon combat unit creation.
        for(const auto& eachAbility : abilities_)
        {
            cooldowns_.push_back({eachAbility.first, eachAbility.second.cooldown});
        }
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
            Ability &ab = found->second;
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
            auto abTimer = std::find_if(cooldowns_.begin(), cooldowns_.end(), [found](const Cooldown& cd){
                return found->first == cd.name;
            });
            if(abTimer != cooldowns_.end())
            {
                if(abTimer->counter < ab.cooldown)
                {
                    //combatLogEntry = abilityName + " is still on cooldown for " + 
                    //    std::to_string(ab.cooldown - abTimer->counter) + " more seconds.";
                    return false;
                }
            }
            else
            {
                // because of how cooldown table is created, THIS should never happen.
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                    "Invalid ability `%s' for %s", abilityName.c_str(), name_.c_str());
                combatLogEntry = "<ERROR>";
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
            auto outputs = ab.formula.CalculateResults(*this);
            // apply all the outputs to either self or target depending on nature of output
            for(const auto& output : outputs)
            {
                switch(output.type)
                {
                case Output::Type::Direct:
                    if(output.target == Output::Target::Enemy)
                    {
                        other.currentHealth_ -= output.direct.amount;
                        combatLogEntry = this->name_ + "'s " + found->first + " hit " + other.name_ +
                            " for " + std::to_string(output.direct.amount) + " " + 
                            SchoolToString(output.school);
                    }
                    else if(output.target == Output::Target::Friendly)
                    {
                        other.currentHealth_ += output.direct.amount;
                        combatLogEntry = this->name_ + "'s" + found->first + " healed " + other.name_ +
                            " for " + std::to_string(output.direct.amount) + " " +
                            SchoolToString(output.school);
                    }
                    // TODO: something self
                    break;
                case Output::Type::OverTime:
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                        "%s: Can't handle DOT/HOTs yet!", __FUNCTION__);
                    break;
                case Output::Type::StatusEffect:
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                        "%s: Can't handle buffs/debuffs yet!", __FUNCTION__);
                    break;
                }
            }
            // set the cooldown of the ability to 0
            abTimer->counter = 0.0f;
            // set the global cooldown timer
            if(ab.onGCD)
                globalCooldownCounter_ = 0.0f;
            // determine overkill or overheal amount if any
            int overKillOrHeal;
            if(other.currentHealth_ > other.maxHealth_)
            {
                overKillOrHeal = -(other.currentHealth_ - other.maxHealth_);
                other.currentHealth_ = other.maxHealth_;
                combatLogEntry += " (" + std::to_string(overKillOrHeal) + " overheal)";
            }
            else if(other.currentHealth_ < 0)
            {
                overKillOrHeal = -other.currentHealth_;
                other.currentHealth_ = 0;
                combatLogEntry += " (" + std::to_string(overKillOrHeal) + " overkill)";
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
            Ability &ab = found->second;
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
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Unable to find `%s' in ability table!", __FUNCTION__, abilityName.c_str());
            return false; // "NULL" ability is never ready
        }
        std::string abName = found->first;
        for(const auto& eachCooldown : cooldowns_)
        {
            if(abName == eachCooldown.name)
            {
                return eachCooldown.counter >= found->second.cooldown;
            }
        }
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
            "%s: Cooldown table entry `%s' is missing!", __FUNCTION__, abilityName.c_str());
        return false;
    }

    float CombatUnit::AbilityCooldownRemaining(const std::string& abilityName)
    {
        const auto& found = abilities_.find(abilityName);
        if(found == abilities_.end())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: Unable to query cooldown for `%s'", __FUNCTION__, abilityName.c_str());
            return 0.0f;
        }
        const auto& cdEntry = std::find_if(cooldowns_.begin(), cooldowns_.end(), [found](const Cooldown& cd){
            return cd.name == found->first;
        });
        if(cdEntry == cooldowns_.end())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Cooldown entry is missing for `%s'", __FUNCTION__, abilityName.c_str());
        }
        float remaining = found->second.cooldown - cdEntry->counter;
        if(remaining < 0.0f)
            remaining = 0.0f;
        return remaining;
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
        for(auto& eachCd : cooldowns_)
        {
            eachCd.counter += dtime;
            // todo: check for near max float value? realistically CD counters shouldn't reach that
            // high of a number in one play session.
        }
        // recovery if out of combat (a clear aggro table)
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

    Ability CombatUnit::GetAbilityByName(const std::string& name)
    {
        // this works because of the empty parameter constructor of Ability and the fact
        //  the table isn't stored internally as const
        return abilities_[name];
    }

}