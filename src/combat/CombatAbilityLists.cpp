// CombatAbilityLists.cpp
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

#include "CombatAbilityLists.hpp"

namespace combat
{

    const CombatAbilityLists& CombatAbilityLists::Get()
    {
        static CombatAbilityLists singleton;
        return singleton;
    }
    
    CombatAbilityLists::CombatAbilityLists()
    {
        // here construct all combat ability lists for all mobs.
        AbilityTable redcrab;
        Expression expr(Output::Type::Direct, Output::Target::Enemy, School::Physical);
        expr.terms = { {NumericRange(0.5f, 0.5f), AttributeInput::MAP }};
        Formula redcrab_attack_formula;
        redcrab_attack_formula.expressions.push_back(expr);
        redcrab["attack"] = Ability(0, 32, true, 1.0f, false, Ability::CastType::Instant, 0.0f, 
            redcrab_attack_formula);
        lists_["redcrab"] = redcrab;
    }

    CombatAbilityLists::~CombatAbilityLists()
    {

    }

}