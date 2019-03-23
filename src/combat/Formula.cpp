// CombatFormula.cpp
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

#include "CombatUnit.hpp"
#include "engine/GameEngine.hpp"
#include "Formula.hpp"
#include "StatusEffectTable.hpp"

namespace combat
{
    Output Expression::RunCalculation(CombatUnit& src) const
    {
        Output o;
        o.outputType = outputType;
        if(statusEffectName != "")
        {
            o.statusEffect = statusEffectName;
            o.duration = (float)statusEffectDuration;
        }
        else 
        {
            o.damage = damage;
            for(const auto& term : terms)
            {
                float out = 0.0f;
                switch(term.variable)
                {
                case Attribute::Strength: 
                case Attribute::Agility:
                case Attribute::Intellect:
                case Attribute::Dexterity:
                case Attribute::Wisdom:
                case Attribute::Knowledge:
                case Attribute::Vitality:
                case Attribute::Weapon:
                    out += (float)src.GetCharacterSheet().CalculateAttribute(term.variable);
                    break;
                case Attribute::MeleeAttackPower:
                    out += (float)src.GetCharacterSheet().GetMeleeAttackPower();
                    break;
                case Attribute::RangedAttackPower:
                    out += (float)src.GetCharacterSheet().GetRangedAttackPower();
                    break;
                case Attribute::SpellPower:
                    out += (float)src.GetCharacterSheet().GetSpellPower();
                    break;
                case Attribute::MaxHealth:
                    out += (float)src.GetCharacterSheet().GetMaxHealth();
                    break;
                case Attribute::MaxMana:
                    out += (float)src.GetCharacterSheet().GetMaxMana();
                    break;
                case Attribute::Level:
                    out += (float)src.GetCharacterSheet().GetLevel();
                    break;
                // TODO: handle rest of possible inputs
                }
                out *= term.coefficient.Next();
                o.amount += (int)out;
            }
        }
        return o;
    }

    // a basic tokenizer. Formula handles the meaning of each token as well as validty of numerical
    // strings.
    class FormulaParser
    {
    public:
        FormulaParser(const std::string& str) : code_(str) {}
        ~FormulaParser() {}

        // does the char represent a single-char token (as opposed to part of a string or number)
        static bool IsCharToken(const char ch)
        {
            switch(ch)
            {
            case '!': case '@': case '#': case '<': case '>': case '?':
            case '~': case '{': case '}': case '(': case ')': case ',':
            case '+': case '/': case ';': case ':':
            case   0:
                return true;
            }
            return false;
        }

        static bool IsCharNumber(const char ch)
        {
            switch(ch)
            {
            case '-': case '.': case '0': case '1': case '2': case '3':
            case '4': case '5': case '6': case '7': case '8': case '9':
                return true;
            }
            return false;
        }

        static bool IsCharName(const char ch)
        {
            return (ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == ' ' || ch == '_');
        }

        char GetNextChar()
        {
            if(chCounter_ >= code_.size())
                return 0;
            else 
                return code_[chCounter_++];
        }

        char PeekNextChar()
        {
            if(chCounter_ >= code_.size())
                return 0;
            else
                return code_[chCounter_];
        }

        std::string GetNextToken()
        {
            std::string tokenBuilder;
            char ch = GetNextChar();
            if(ch != 0)
            {
                // only one char to read
                if(IsCharToken(ch))
                {
                    tokenBuilder = ch;
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                        "Formula compiler reads token \"%s\"", tokenBuilder.c_str());
                    return tokenBuilder;
                }
                else if(IsCharNumber(ch))
                {
                    tokenBuilder += ch;
                    while(IsCharNumber(PeekNextChar()))
                    {
                        tokenBuilder += GetNextChar();
                    }
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                        "Formula compiler reads token \"%s\"", tokenBuilder.c_str());
                    return tokenBuilder;
                }
                else if(IsCharName(ch))
                {
                    tokenBuilder += ch ;
                    while(IsCharName(PeekNextChar()))
                    {
                        tokenBuilder += GetNextChar();
                    }
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                        "Formula compiler reads token \"%s\"", tokenBuilder.c_str());
                    return tokenBuilder;
                }
            }
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                        "Formula compiler reads token \"%s\"", tokenBuilder.c_str());
            return "";
        }

    private:
        std::string code_;
        int chCounter_ = 0;
    };

    // a string is considered a valid number on the following conditions:
    // it has 0 or 1 '-' and if 1, it must be the first char.
    // exactly 0 or 1 '.'
    // it has a length > 0
    // it otherwise contains chars '0'-'9'
    static bool StringIsAValidNumber(const std::string& str)
    {
        if(str.size() == 0)
            return false;
        int dashCounter = 0;
        int dotCounter = 0;
        for(int i=0; i < str.length(); ++i)
        {
            if(!(str[i] >= '0' && str[i] <= '9'))
            {
                if(str[i] == '-')
                    dashCounter++;
                else if(str[i] == '.')
                    dotCounter++;
                else // not a number, '.', or '-' so invalid for our purposes
                    return false;
            }
            if(dashCounter > 1)
                return false;
            if(dotCounter > 1)
                return false;
        }
        if(dashCounter == 1 && str[0] != '-') // dash is in the wrong place
            return false;
        return true;
    }

    Formula::Formula(const std::string& formulaText)
    {
        FormulaParser parser(formulaText);
        std::string token;
        while(parser.PeekNextChar())
        {
            // start building an expression object
            Expression currentExpression;
            // look for the specifier
            token = parser.GetNextToken();
            // the first char should be '<', '>', or '/'. Anything else is a compile error.

            if(token == "<")
            {
                currentExpression.damage = true;
            }
            else if(token == ">")
            {
                currentExpression.damage = false;
            }
            else if(token == "/")
            {
                token = parser.GetNextToken();
                currentExpression.statusEffectName = token;
                token = parser.GetNextToken();
                currentExpression.statusEffectDuration = (int)atoi(token.c_str());
                token = parser.GetNextToken();
                if(token == ";")
                {
                    expressions.push_back(currentExpression);
                    continue;
                }
                else
                {
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                        "%s: Expected `;' at end of status effect expression", __FUNCTION__);
                }
            }
            else 
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                    "%s: Wrong type of token `%s' for first expression token!", 
                    __FUNCTION__, token.c_str());                
            }

            token = parser.GetNextToken();

            // Get the coefficient var pairs separated by +
            while(token != "," && token != ";" && token != "")
            {
                // can't create the numeric range until we get both lower and upper
                float lower=1.0f, upper=1.0f;
                Attribute var;
                // try to read a '(' or single number
                if(token == "(")
                {
                    // parse "(lower,upper)" for the numeric range
                    token = parser.GetNextToken();
                    if(!StringIsAValidNumber(token))
                    {
                        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                            "%s: Invalid number `%s' for lower range", __FUNCTION__, token.c_str());
                    }
                    lower = (float)atof(token.c_str());
                    token = parser.GetNextToken(); 
                    // can either be , or ) to indicate lower=upper
                    if(token == ",")
                    {
                        token = parser.GetNextToken();
                        if(!StringIsAValidNumber(token))
                        {
                            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                                "%s: Invalid number `%s' for upper range", __FUNCTION__, token.c_str());
                        }
                        upper = (float)atof(token.c_str());
                        // still have to read a ")" to close
                        token = parser.GetNextToken();
                        if(token != ")")
                        {
                            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                                "%s: Expected `)'", __FUNCTION__);
                        }
                    }
                    else if(token == ")")
                    {
                        lower = upper;
                    }

                }
                else 
                {
                    // all variable must be preceded by a (n,n) (n) or n so here we expect to read
                    // just a number.
                    if(StringIsAValidNumber(token))
                    {
                        lower = upper = (float)atof(token.c_str());
                    }
                    else 
                    {
                        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                            "%s: Numeric value expected. Got `%s'", __FUNCTION__, token.c_str());
                    }
                }
                // next read the attribute input var name
                token = parser.GetNextToken();
                var = StringToAttribute(token);
                // now we can create the Term
                ExpressionTerm term = { NumericRange(lower,upper), var };
                currentExpression.terms.push_back(term);
                token = parser.GetNextToken();
                // now we reach a '+' indicating to continue this loop to read another term or the while condition
                // will break upon finding a ',' for the school or ';' to end the expression
                if(token == "+")
                {
                    token = parser.GetNextToken();
                    continue;
                }
            }
            // check to see if a ',' school indicator was found
            if(token == ",")
            {
                token = parser.GetNextToken();
                // defaults to physical so no use checking validity
                currentExpression.outputType = StringToOutputType(token);
                token = parser.GetNextToken();
                // expect a ';' immediately after school. indicated end of expression. 
            }
            if(token == ";") 
            {
                // TODO: learn how to write compilers and fix this spaghetti code
                expressions.push_back(currentExpression);
            }
            else 
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                    "%s: Expecting `;' got `%s' (Did you forget a semi-colon?)", __FUNCTION__, token.c_str());
            }
        }
    }

    std::vector<Output> Formula::CalculateResults(CombatUnit& cu) const
    {
        std::vector<Output> outputs;
        for(const auto& expr : expressions)
        {
            outputs.push_back(expr.RunCalculation(cu));
        }
        return outputs;
    }
}