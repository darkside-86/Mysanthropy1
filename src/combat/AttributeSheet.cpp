// StatSheet.cpp
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

#include "AttributeSheet.hpp"

#include "game/Configuration.hpp"

namespace combat
{

    AttributeSheet::AttributeSheet(int level, bool player) 
        : level_(level), player_(player)
    {
        // TODO: Read from configuration file "Survivalist" core stats
        auto& config = game::Configuration::Get();
        std::vector<std::string> survCoreStats;
        if(player_) // only player is Survivalist so far
        {
            config.GetVar("SURVIVALIST_CORE_STATS", survCoreStats);
            for(const auto& eachStat : survCoreStats)
            {
                if(eachStat == "Strength")
                    coreAttributes_.push_back(Attribute::Strength);
                else if(eachStat == "Agility")
                    coreAttributes_.push_back(Attribute::Agility);
                else if(eachStat == "Intellect")
                    coreAttributes_.push_back(Attribute::Intellect);
                else if(eachStat == "Dexterity")
                    coreAttributes_.push_back(Attribute::Dexterity);
                else if(eachStat == "Wisdom")
                    coreAttributes_.push_back(Attribute::Wisdom);
                else if(eachStat == "Knowledge")
                    coreAttributes_.push_back(Attribute::Knowledge);
                else if(eachStat == "Vitality")
                    coreAttributes_.push_back(Attribute::Vitality);
            }
        }
        else // make mobs OP by making all their stats CORE
        {
            coreAttributes_.push_back(Attribute::Strength);
            coreAttributes_.push_back(Attribute::Agility);
            coreAttributes_.push_back(Attribute::Vitality);
        }
        SetLevel(level);
    }

    AttributeSheet::~AttributeSheet()
    {

    }

    void AttributeSheet::SetLevel(int level)
    {
        level_ = level;
        auto& config = game::Configuration::Get();

        int CORE_STATS; // set to 5 in config file
        config.GetVar("CORE_STATS", CORE_STATS);
        if(player_)
        {
            int OTHER_STATS; // also set to 5 in config for now
            config.GetVar("OTHER_STATS", OTHER_STATS);
            float CORESTAT_SCALE;
            config.GetVar("CORESTAT_SCALE", CORESTAT_SCALE);
            float OTHERSTAT_SCALE;
            config.GetVar("OTHERSTAT_SCALE", OTHERSTAT_SCALE);

            float coreMultiplier = pow(CORESTAT_SCALE, level - 1);
            float otherMultiplier = pow(OTHERSTAT_SCALE, level - 1);
            // calculate strength

            strength_ = (int)(IsCoreAttribute(Attribute::Strength)? 
                CORE_STATS * coreMultiplier : OTHER_STATS * otherMultiplier);
            // calculate agility
            agility_ = (int)(IsCoreAttribute(Attribute::Agility)? 
                CORE_STATS * coreMultiplier : OTHER_STATS * otherMultiplier);
            // calculate vitality
            vitality_ = (int)(IsCoreAttribute(Attribute::Vitality)? 
                CORE_STATS * coreMultiplier : OTHER_STATS * otherMultiplier);
        }
        else // for mobs, all attributes are core
        {
            float MOB_STAT_SCALE;
            config.GetVar("MOB_STAT_SCALE", MOB_STAT_SCALE);
            float mobMultiplier = pow(MOB_STAT_SCALE, level - 1);
            strength_ = (int)(mobMultiplier * CORE_STATS);
            agility_ = (int)(mobMultiplier * CORE_STATS);
            vitality_ = (int)(mobMultiplier * CORE_STATS);
        }
    }

    int AttributeSheet::GetMeleeAttackPower() const
    {
        return strength_ + (level_ - 1); // for now
    }

    int AttributeSheet::GetRangedAttackPower() const
    {
        return agility_ + (level_ - 1); // for now
    }

    int AttributeSheet::GetMaxHealth() const
    {
        return 5 * vitality_ + (level_ - 1); // for now
    }

    bool AttributeSheet::IsCoreAttribute(const Attribute attr) const
    {
        for(int i=0; i < coreAttributes_.size(); ++i)
        {
            if(coreAttributes_[i] == attr)
                return true;
        }
        return false;
    }
}