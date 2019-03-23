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
#include "StatusEffectTable.hpp"

namespace combat
{

    CombatUnit::CombatUnit(bool player, int level, const CombatClassEntry& cce, 
            const std::string& name)
        : characterSheet_(level, player, cce), name_(name)
    {
        const AbilityTable& at = AbilityTable::Get();
        currentHealth_ = characterSheet_.GetMaxHealth();
        for(const auto& abName : characterSheet_.GetCombatClassEntry().abilityList)
        {
            const Ability* ab = at.GetAbility(abName);
            if(ab != nullptr)
            {
                cooldowns_.push_back({abName, ab->cooldown});
                printf("[PRINTF] Adding %s to the cooldown table\n", abName.c_str());
            }
            else 
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                    "%s: No such ability `%s'", __FUNCTION__, abName.c_str());
            }
        }
        
    }

    CombatUnit::~CombatUnit()
    {

    }

    bool CombatUnit::UseTargetAbility(CombatUnit& other, bool targetIsFriendly,
        const std::string& abilityName, std::string& combatLogEntry)
    {
        const Ability* found = AbilityTable::Get().GetAbility(abilityName);
        if(found == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Ability `%s' not found", __FUNCTION__, abilityName.c_str());
            return false;
        }
        if(found->rangeType != Ability::RangeType::Target)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Use the correct method for the range type!", __FUNCTION__);
            return false;
        }

        const Ability &ab = *found;
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
            printf("Comparing CD.name=%s to found->name=%s\n", cd.name.c_str(), found->name.c_str());
            return found->name == cd.name;
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
        auto outputs = ab.formula.CalculateResults(*this);
        // apply all the outputs to either self or target depending on nature of output
        for(const auto& output : outputs)
        {
            if(output.statusEffect != "")
            {
                other.ApplyStatusEffect(output.statusEffect, output.duration, *this);
                combatLogEntry = GetName() + " applied " + output.statusEffect + " to " + other.GetName();
            }
            else if(output.damage)
            {
                // can only dodge physical damage of any kind
                if(output.outputType == OutputType::Physical)
                {
                    float chanceToDodge = other.GetCharacterSheet().GetDodgeChance();
                    float armorM = other.GetCharacterSheet().GetArmorPercent() / 100.f;
                    bool dodged = engine::GameEngine::Get().PercentChance(chanceToDodge);

                    if(dodged)
                    {
                        combatLogEntry = GetName() + " dodged " + other.GetName() + "'s "
                                + ab.name;
                    }
                    else
                    {
                        int amount = output.amount - (int)((float)output.amount * armorM);
                        other.SetCurrentHealth(other.GetCurrentHealth() - amount);
                        combatLogEntry = GetName() + "'s " + ab.name + " hit " + other.GetName() 
                                + " for " + std::to_string(amount) + " physical";
                    }
                }
                else 
                {
                    int resistAmount = other.GetCharacterSheet().GetResistance(output.outputType);
                    other.SetCurrentHealth(other.GetCurrentHealth() - (output.amount - resistAmount));
                    combatLogEntry = GetName() + "'s " + ab.name + " hit " + other.GetName()
                            + " for " + std::to_string(output.amount - resistAmount) + " "
                            + OutputTypeToString(output.outputType);
                    }
            }
            else 
            {
                // TODO: factor in resistances when healing
                other.SetCurrentHealth(other.GetCurrentHealth() + output.amount);
                combatLogEntry = GetName() + "'s " + ab.name + " healed " + other.GetName()
                        + " for " + std::to_string(output.amount) + " " 
                        + OutputTypeToString(output.outputType);
            }
        }
        // set the cooldown of the ability to 0
        abTimer->counter = 0.0f;
        // set the global cooldown timer
        if(ab.onGCD)
            globalCooldownCounter_ = 0.0f;
        // determine overkill or overheal amount if any
        int overKillOrHeal;
        if(other.currentHealth_ > other.GetMaxHealth())
        {
            overKillOrHeal = -(other.currentHealth_ - other.GetMaxHealth());
            other.currentHealth_ = other.GetMaxHealth();
            combatLogEntry += " (" + std::to_string(overKillOrHeal) + " overheal)";
        }
        else if(other.currentHealth_ < 0)
        {
            overKillOrHeal = -other.currentHealth_;
            other.currentHealth_ = 0;
            combatLogEntry += " (" + std::to_string(overKillOrHeal) + " overkill)";
        }
        for(const auto& disp : ab.dispels)
        {
            if(ab.targetType == Ability::TargetType::Enemy)
                DispelStatusEffect("*", "*", disp, false);
            else
                DispelStatusEffect("*", "*", disp, false);

        }

        return true;
    }

    bool CombatUnit::AbilityInRange(CombatUnit& other, const std::string& abilityName)
    {
        float distance = glm::distance(location_, other.location_);
        const auto* found = AbilityTable::Get().GetAbility(abilityName);
        if(found != nullptr)
        {
            const Ability &ab = *found;
            return !(distance < ab.minRange || distance > ab.maxRange);
        }
        else
        {
            return false;
        }
    }

    bool CombatUnit::AbilityIsReady(const std::string& abilityName)
    {
        const auto* found = AbilityTable::Get().GetAbility(abilityName);
        if(found == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Unable to find `%s' in ability table!", __FUNCTION__, abilityName.c_str());
            return false; // "NULL" ability is never ready
        }
        std::string abName = found->name;
        for(const auto& eachCooldown : cooldowns_)
        {
            if(abName == eachCooldown.name)
            {
                return eachCooldown.counter >= found->cooldown;
            }
        }
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
            "%s: Cooldown table entry `%s' is missing!", __FUNCTION__, abilityName.c_str());
        return false;
    }

    float CombatUnit::AbilityCooldownRemaining(const std::string& abilityName)
    {
        const auto* found = AbilityTable::Get().GetAbility(abilityName);
        if(found == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR, 
                "%s: Unable to query cooldown for `%s'", __FUNCTION__, abilityName.c_str());
            return 0.0f;
        }
        const auto& cdEntry = std::find_if(cooldowns_.begin(), cooldowns_.end(), [found](const Cooldown& cd){
            return cd.name == found->name;
        });
        if(cdEntry == cooldowns_.end())
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Cooldown entry is missing for `%s'", __FUNCTION__, abilityName.c_str());
        }
        float remaining = found->cooldown - cdEntry->counter;
        if(remaining < 0.0f)
            remaining = 0.0f;
        return remaining;
    }

    bool CombatUnit::GlobalCooldownIsOff()
    {
        return globalCooldownCounter_ >= GCD;
    }

    std::vector<std::string> CombatUnit::Update(float dtime)
    {
        std::vector<std::string> combatLogs;

        // GCD check
        globalCooldownCounter_ += dtime;
        if(globalCooldownCounter_ > GCD)
            globalCooldownCounter_ = GCD;
        // individual ability's cooldowns
        for(auto& eachCd : cooldowns_)
        {
            const Ability* ab = AbilityTable::Get().GetAbility(eachCd.name);
            float cd = ab == nullptr ? 0.0f : ab->cooldown;
            if(eachCd.counter < cd)
                eachCd.counter += dtime;
        }
        // recovery if out of combat (a clear aggro table)
        if(!inCombat_)
        {
            if(currentHealth_ < GetMaxHealth())
            {
                healthRecoveryTimer_ += dtime;
                if(healthRecoveryTimer_ >= RECOVERY_RATE)
                {
                    healthRecoveryTimer_ -= RECOVERY_RATE;
                    currentHealth_ += RECOVERY_AMOUNT;
                    if(currentHealth_ > GetMaxHealth())
                        currentHealth_ = GetMaxHealth();
                }
            }
        }
        // tick status effects and remove inactive ones one per cycle
        std::vector<StatusEffect*>::iterator toRemove = characterSheet_.GetStatusEffects().end();
        for(auto each = characterSheet_.GetStatusEffects().begin(); 
                each != characterSheet_.GetStatusEffects().end(); ++each)
        {
            (*each)->Update(dtime);
            if((*each)->TimeForTick())
            {
                for(const auto& o : (*each)->GetOutputTicks())
                {
                    // handle damage
                    if(o.type == OutputType::Physical)
                    {
                        // physical damage can be mitigated by armor
                        float armorM = GetCharacterSheet().GetArmorPercent() / 100.f;
                        int amount = o.amount - (int)((float)o.amount * armorM);
                        SetCurrentHealth(GetCurrentHealth() - amount);
                        std::string log = (*each)->GetEntry().name + " ticked for " + std::to_string(amount)
                                + (amount < 0 ? " healing " : " damage ") + "(physical)";
                        combatLogs.push_back(log);
                    }
                    else 
                    {
                        // non-physical damage can only be mitigated by resistance points
                        int resist = GetCharacterSheet().GetResistance(o.type);
                        int amount = o.amount - resist;
                        SetCurrentHealth(GetCurrentHealth() - amount);
                        std::string log = (*each)->GetEntry().name + " ticked for " + std::to_string(amount)
                                + (amount < 0 ? " healing (" : " damage (") + OutputTypeToString(o.type)
                                + ")";
                        combatLogs.push_back(log);
                    }
                }
            }
            if(!(*each)->IsActive())
            {
                toRemove = each;
            }
            if(toRemove != characterSheet_.GetStatusEffects().end())
            {
                delete (*toRemove);
                characterSheet_.GetStatusEffects().erase(toRemove);
            }
        }
        // get rid of everything if unit is "dead"
        if(currentHealth_ <= 0)
        {
            RemoveAllStatusEffects();
        }
        
        return combatLogs;
    }

    void CombatUnit::ApplyStatusEffect(const std::string& name, float duration, CombatUnit& src)
    {
        const StatusEffectEntry* entry = StatusEffectTable::Get().GetEntry(name);
        if(entry == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::ERROR,
                "%s: Unable to apply missing status effect `%s'", __FUNCTION__, name.c_str());
            return;
        }
        StatusEffect *ef = new StatusEffect(*entry, (int)duration, src);
        characterSheet_.GetStatusEffects().push_back(ef);
    }

    void CombatUnit::DispelStatusEffect(const std::string& name, const std::string& group, 
            OutputType type, bool harm)
    {
        std::vector<StatusEffect*>::iterator found = characterSheet_.GetStatusEffects().end();
        for(auto it = characterSheet_.GetStatusEffects().begin(); 
                it != characterSheet_.GetStatusEffects().end(); ++it)
        {
            // only dispel active ones
            if((*it)->IsActive())
            {
                if(((*it)->GetEntry().group == group || group == "*")
                        && (name == (*it)->GetEntry().name || name == "*")
                        && (*it)->GetEntry().outputType == type
                        && (*it)->GetEntry().dispel 
                        && (*it)->GetEntry().harmful == harm)
                {
                    // this is a one
                    found = it;
                }
            }
        }
        if(found != characterSheet_.GetStatusEffects().end())
        {
            delete *found;
            characterSheet_.GetStatusEffects().erase(found);
        }
    }

    void CombatUnit::RemoveAllStatusEffects()
    {
        for(auto& each : characterSheet_.GetStatusEffects())
        {
            delete each;
        }
        characterSheet_.GetStatusEffects().clear();
    }

    const std::vector<std::string>& CombatUnit::GetAbilities() const
    {
        return characterSheet_.GetCombatClassEntry().abilityList;
    }

}