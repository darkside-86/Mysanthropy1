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

#include <string>
#include <vector>

namespace combat
{
    // encapsulates a floating point numerical range inclusive of the lower and upper bound
    class NumericRange 
    { public:
        float lower=1.0f, upper=1.0f;
        float Next();
    };

    // defines the school of magic associated with output
    enum class School 
    {
        Physical,
        Earth, Air, Fire, Frost,
        Shadow, Holy, Nature, Life
    };

    // defines the source of an input variable in an expression. can be primary or derived stats
    enum class StatInput
    {
        None,
        // core stats
        Strength, Agility, Vitality, //..., etc.
        // derived stats
        MeleeAP, RangedAP, Health, // ..., etc.
        // other
        Level //, ..., etc.
    };

    // formulas are just a series of terms added together
    class FormulaTerm
    {
        NumericRange coefficient;
        std::vector<StatInput> variables;
        NumericRange exponent;
    };

    // defines the output of the formula. Can be just flat damage or healing or applying a de/buff
    //  or even both.
    class Output
    { public:
        // light RTTI
        bool useOnEnemy = true;
        School school = School::Physical;
        bool isBuffOrDebuff = false;
        // only applies if above is true--
        float buffDuration; // in seconds
        float buffDamageOrHealing; // total damage or healing (a negative number) over duration
        float buffStatCoefficient = 1.0f;
        StatInput buffStat = StatInput::None;
    };

    // encapsulates an algebraic expression and damage output type for calculating combat unit moves
    class Formula
    {
    public:
        Formula(const std::string& formulaText);
        Formula() {}

        std::vector<FormulaTerm> terms;
    };

    // formula language could be
    // (1,5)STR+(2,3.5)AGI^(1.1,1.2)+(1,5)NUL
    // 1 to 5 * strength + 2 to 3.5 agility + 1 to 5
    // or for buffs
}