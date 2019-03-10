// CombatUnit.hpp
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
#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

#include "Ability.hpp"
#include "AttributeSheet.hpp"

namespace combat
{
    class Cooldown
    { public:
        std::string name;
        float counter;
    };

    class CombatUnit
    {
    public:
        // ctor
        CombatUnit(bool player, int level, const AbilityTable& abilities, 
                const std::string& name);
        // dtor
        virtual ~CombatUnit();
        // get the name of the unit
        inline std::string GetName() { return name_; }
        // get the stat sheet by reference
        inline AttributeSheet& GetAttributeSheet() { return attributeSheet_; }
        // get location
        inline glm::vec2 GetLocation() const { return location_; }
        // set location to determine ability range
        inline void SetLocation(const glm::vec2& loc) { location_ = loc; }
        // get current health
        inline int GetCurrentHealth() const { return currentHealth_; }
        // set current health. used by game to set health of player upon respawn
        inline void SetCurrentHealth(int h) { currentHealth_ = h; }
        // get max health.
        inline int GetMaxHealth() { maxHealth_ = attributeSheet_.GetMaxHealth(); return maxHealth_; }
        // return value = successful cast
        bool UseAbility(CombatUnit& other, bool targetIsFriendly, 
                        const std::string& abilityName, std::string& combatLogEntry);
        // determine if an ability is in range
        bool AbilityInRange(CombatUnit& other, const std::string& abilityName);
        // return true if ability is ready
        bool AbilityIsReady(const std::string& abilityName);
        // return cooldown remaining on an ability
        float AbilityCooldownRemaining(const std::string& abilityName);
        // return true if GCD is off
        bool GlobalCooldownIsOff();
        // updates the cooldown timers and generates 1 health per second when out of combat
        void Update(float dtime);
        // get/set in combat
        inline bool IsInCombat() const { return inCombat_; }
        inline void SetInCombat(bool b) { inCombat_ = b; }
        // get abilities
        inline const AbilityTable& GetAbilities() const { return abilities_; }
    private:
        //  name of unit for combat log
        std::string name_;
        // stat sheet
        AttributeSheet attributeSheet_;
        // determines whether or not an ability is in range
        glm::vec2 location_ = {0.f, 0.f};
        // the current health of the unit
        int currentHealth_ = 0;
        // the max health of the unit.
        int maxHealth_ = 0;
        // recovery amount : 1 HP
        static constexpr int RECOVERY_AMOUNT = 1;
        // recovery rate : 1 second
        static constexpr float RECOVERY_RATE = 1.0f;
        // health recovery timer
        float healthRecoveryTimer_ = 0.f;
        // determines if unit is in combat
        bool inCombat_ = false;
        // list of abilities that can be performed
        AbilityTable abilities_;
        // GCD
        static constexpr float GCD = 1.0f;
        // GCD counter
        float globalCooldownCounter_ = 0.0f;
        // ability cooldown tracker
        std::vector<Cooldown> cooldowns_;
    };

}