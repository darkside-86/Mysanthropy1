// Formula.hpp
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

#include <string>
#include <vector>

#include "Attribute.hpp"
#include "NumericRange.hpp"
#include "OutputType.hpp"

namespace combat
{
    class CombatUnit; // forward declaration

    // formulas are just a series of terms added together
    class ExpressionTerm
    { public:
        // what to multiply the variable with
        NumericRange coefficient;
        // the input variable
        Attribute variable;
    };

    // defines the output of an expression in a formula.
    class Output
    { 
    public:
        enum class TargetType { Self, Friendly, Hostile };

        Output() {}

        bool damage = true;
        OutputType outputType;
        TargetType targetType;
        std::string statusEffect;
        float duration = 0.0f;
        int amount = 0; // the final calculated amount
    };

    // a formula can be any number of expressions -> outputs
    class Expression
    { public:
        Expression() {}
        // defines < or > (see formula language spec)
        bool damage = true;
        // input variables multiplied by ranged coefficients added together
        std::vector<ExpressionTerm> terms;
        // name of a status effect that might have been applied
        std::string statusEffectName;
        // duration of possible status effect.
        int statusEffectDuration=0;
        // Output type
        OutputType outputType = OutputType::Physical;
        // runs calculation to get output or buff info
        Output RunCalculation(CombatUnit&) const;
    };

    // encapsulates an algebraic expression and damage output type for calculating combat unit moves
    class Formula
    {
    public:
        Formula(const std::string& formulaText);
        Formula() {}
        std::vector<Output> CalculateResults(CombatUnit&) const;

        std::vector<Expression> expressions;
    };

    // formula language specification:
    // *** OUTPUT TYPE *** <, > or /
    //      indicates whether flat damage or healing, DOT/HOT, buff/debuff.
    //      always comes after specifier in formula (it is the second char)
    //      '<' for damage, '>' for healing. If healing is specified for an ability with a
    //      hostile target, the healing is applied to the ability user. '/' followed by
    //      NameOfStatusEffect then a number representing seconds to apply status effect for
    // *** COEFFICIENT *** is a range enclosed in () pairs with lower followed by , then upper.
    //      for example (1,5) means a random number between 1 and 5 inclusive.
    //      constant numbers can also be used as coefficients such as 3.14159
    // *** ATTRIBUTE *** indicates the input variable to multiply, such as STR for strneght.
    //      STR - Strength of the given combat unit performing the action.
    //      AGI - Agility of the given combat unit performing the action.
    //      VIT - The Vitality of the given combat unit
    //      MOB - Either 1 or 0. 0 means the unit is stunned. Used to write stun abilities
    //      ...
    // *** TERM SEPARATOR *** The term separator is '+' indicates that a new coefficient and attribute
    //      is to be read next.
    // *** SCHOOL TYPE ***  The school of magic is specified by a ',' followed by the name of the school
    //      e.g. ",Physical" and should be placed before
    // *** TERMINATOR CHAR *** A semi-colon ";" is placed at the end to indicate the end of the expression
    //      This separates multiple expressions.
    //-------------------------------------------------------------------------
    // Examples of formulas are as follows.
    // <(0.1,0.2)STR+(0.1,0.2)WPN,Physical;/bleed6;

}