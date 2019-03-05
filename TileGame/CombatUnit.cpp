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

CombatUnit::CombatUnit(bool attackable, const std::unordered_map<std::string, CombatAbility>& abilities)
    : attackable_(attackable), abilities_(abilities)
{

}

CombatUnit::~CombatUnit()
{

}

int CombatUnit::UseAbility(CombatUnit& other, const std::string& abilityName)
{
    auto found = abilities_.find(abilityName);
    if(found != abilities_.end())
    {
        const CombatAbility &ab = found->second;
        if( !((ab.offensive && other.attackable_) || (!ab.offensive && !other.attackable_) ) )
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                "Can't attack this target.");
            return 0;
        }
        other.currentHealth_ -= ab.calculateBaseDamage();
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
        return overkillOrHeal;
    }
    else
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
            "%s: Unit does not have ability `%s'", __FUNCTION__, abilityName.c_str());
    }
    return 0;
}