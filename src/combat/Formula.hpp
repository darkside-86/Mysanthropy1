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
    class CombatUnit; // forward declaration

    // encapsulates a floating point numerical range inclusive of the lower and upper bound
    // and returns a random float between the range
    class NumericRange 
    { 
    public:
        NumericRange(float lower, float upper) : lower_(lower), upper_(upper) 
        {   // validate bounds
            if(lower > upper)
            {
                float swap = upper;
                upper = lower;
                lower = swap;
            }
        }
        NumericRange() {}
        float Next() const;
    private:
        float lower_=1.0f, upper_=1.0f;
    };

    // defines the school of magic associated with output
    enum class School 
    {
        Physical,
        Earth, Air, Fire, Frost,
        Shadow, Holy, Nature, Life
    };

    std::string SchoolToString(const School s);

    // defines the source of an input variable in an expression. can be primary or derived stats
    enum class AttributeInput
    {
        NIL, // no stat (always have value of 1)
        // core stats
        STR, AGI, INT, DEX, WIS, KNO, VIT,
        // derived stats Melee AP, Ranged AP, health
        MAP, RAP, HP, // ..., etc.
        // level
        LVL //, ..., etc.
    };

    // formulas are just a series of terms added together
    class FormulaTerm
    { public:
        NumericRange coefficient;
        AttributeInput variable;
    };

    // defines the output of the formula. Can be just flat damage or healing or applying a de/buff
    //  or even both. Stack allocated "polymorphism" ftwwtf
    class Output
    { 
    public:
        enum class Type { Direct, OverTime, StatusEffect };
        enum class Target { Self, Friendly, Enemy };

        Output() {}

        // these weird "constructors" for the "subclasses" ffs
        static Output Direct(Target t, School s, int amount) 
        {
            Output o;
            o.type = Type::Direct; o.target = t; o.school = s; o.direct.amount = amount;
            return o;
        }

        static Output OverTime(Target t, School s, int amount, int duration)
        {
            Output o;
            o.type = Type::OverTime; o.target = t; o.school = s;
            o.overTime.amount = amount; o.overTime.duration = duration;
            return o;
        }

        static Output StatusEffect(Target t, School s, float m, AttributeInput a)
        {
            Output o;
            o.type = Type::StatusEffect; o.target = t; o.school = s;
            o.statusEffect.multiplier = m; o.statusEffect.attribute = a;
            return o;
        }

        Type type = Type::Direct;
        Target target = Target::Self; // default
        School school = School::Physical; // default
        // C++ can't upcast inheritance trees of objects by value so...
        union 
        {
            struct 
            {
                int amount = 0;
            } direct;

            struct
            {
                int amount = 0;
                int duration = 0;
            } overTime;

            struct 
            {
                float multiplier = 1.0f;
                int duration = 0;
                AttributeInput attribute = AttributeInput::NIL;
            } statusEffect;
        };
    };

    // a formula can be any number of expression -> output
    class Expression
    { public:
        Expression(Output::Type t, Output::Target tg, School s) : outputType(t), outputTarget(tg),
            outputSchool(s) {}
        // Output factory information
        Output::Type outputType;
        Output::Target outputTarget;
        School outputSchool;
        // generic data to create each output
        int duration = 0; // good for buff or dots
        float multiplier = 1.0f; // buff
        AttributeInput buffAttrib = AttributeInput::NIL; // buff
        std::vector<FormulaTerm> terms;
        // create the output subclass based on input
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
    // *** SPECIFIERS *** indicates the type of target (self/friendly/enemy)
    //                    these always go first in the expression.
    //                    '!' for enemy, '@' for can use on friendly target, '#' self only
    // *** OUTPUT TYPE *** indicates whether flat damage or healing, DOT/HOT, buff/debuff.
    //                     always comes after specifier in formula (it is the second char)
    //                     '<' for flat damage, '>' for DOT/HOT, ? for buff/debuff
    // *** COEFFICIENT *** is a range enclosed in () pairs with lower followed by , then upper.
    //                     for example (1,5) means a random number between 1 and 5 inclusive.
    //                     constant numbers can also be used as coefficients such as 3.14159
    // *** ATTRIBUTE *** indicates the input variable to multiply, such as STR for strneght.
    //  STR - Strength of the given combat unit performing the action.
    //  AGI - Agility of the given combat unit performing the action.
    //  VIT - The Vitality of the given combat unit
    //  ...
    // *** TERM SEPARATOR *** The term separator is '+' indicates that a new coefficient and attribute
    //                        is to be read next.
    // *** SCHOOL TYPE ***  The school of magic is specified by a ',' followed by the name of the school
    //                      e.g. ",Physical" and should be placed before
    // *** COOLDOWN *** The cooldown of abilities is configured separately and not a part of the formula
    // *** CAST TIME *** The cast time of abilities, or that they are instant/channeled, are configured separately
    // *** TERMINATOR CHAR *** A semi-colon ";" is placed at the end to indicate the end of the expression
    //                      And the beginning of a new one.
    //-------------------------------------------------------------------------
    // Examples of formulas are as follows.
    // _Sacrifice 5% of user's current HP over 10 seconds in exchange for 10% increased strength for 5 seconds_
    // #>(0.05)HP:10,Physical;#?(1.1)STR:5,Physical;
    // _Basic Attack with no weapon for 1/3.5 of Strength and 1 point per user level_
    // !<(0.285)STR+1LVL+(-1),Physical;
    // _Heal a friendly target for 0.35 of SP Holy instantly and 0.15 of SP Holy over 10 seconds_
    // @>(-0.35)SP,Holy;@<(-0.15)SP,Holy;

}