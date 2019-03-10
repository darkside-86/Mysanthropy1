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

namespace combat
{
    float NumericRange::Next() const
    {
        float r = upper_ - lower_;
        if(r == 0.0f) // don't waste RNG time if no range
            return lower_;
        auto& rng = engine::GameEngine::Get().GetRNG();
        float v = (float)rng() / (float)rng.max();
        return lower_ + r * v;
    }

    // TODO: add future inputs
    AttributeInput StringToAttributeInput(const std::string str)
    {
        if(str == "NIL")
            return AttributeInput::NIL;
        else if(str == "STR")
            return AttributeInput::STR;
        else if(str == "AGI")
            return AttributeInput::AGI;
        else if(str == "INT")
            return AttributeInput::INT;
        else if(str == "DEX")
            return AttributeInput::DEX;
        else if(str == "WIS")
            return AttributeInput::WIS;
        else if(str == "KNO")
            return AttributeInput::KNO;
        else if(str == "VIT")
            return AttributeInput::VIT;
        else if(str == "MAP")
            return AttributeInput::MAP;
        else if(str == "RAP")
            return AttributeInput::RAP;
        else if(str == "ARM")
            return AttributeInput::ARM;
        else if(str == "SPD")
            return AttributeInput::SPD;
        else if(str == "HP")
            return AttributeInput::HP;
        else if(str == "LVL")
            return AttributeInput::LVL;
        
        return AttributeInput::INV;
    }

    std::string SchoolToString(const School s)
    {
        switch(s)
        {
        case School::Air: return "Air";
        case School::Earth: return "Earth";
        case School::Fire: return "Fire";
        case School::Frost: return "Frost";
        case School::Holy: return "Holy";
        case School::Life: return "Life";
        case School::Nature: return "Nature";
        case School::Physical: return "Physical";
        case School::Shadow: return "Shadow";
        default: return "Unknown"; // compiler gave a warning but all cases are handled?
        }
    }

    School StringToSchool(const std::string& str)
    {
        if(str == "Air")
            return School::Air;
        if(str == "Earth")
            return School::Earth;
        if(str == "Fire")
            return School::Fire;
        if(str == "Frost")
            return School::Frost;
        if(str == "Holy")
            return School::Holy;
        if(str == "Life")
            return School::Life;
        if(str == "Nature")
            return School::Nature;
        if(str == "Physical")
            return School::Physical;
        if(str == "Shadow")
            return School::Shadow;

        return School::Physical; // sane default
    }

    Output Output::StatusEffect(Target t, School s, const std::vector<StatusEffectData>& sed)
    {
        Output o;
        o.type = Type::StatusEffect; o.target = t; o.school = s;
        for(int i=0; i < MAX_STATUS_EFFECTS; ++i)
        {
            if(i >= sed.size())
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "%s: Status effects are discarded due to limit of %lu", 
                    __FUNCTION__, MAX_STATUS_EFFECTS);
                break;
            }
            o.statusEffect.data[i] = sed[i];
        }
        return o;
    }

    Output Expression::RunCalculation(CombatUnit& unit) const
    {
        // first get the sum of the algebraic expression
        float sum = 0;
        std::vector<StatusEffectData> data; 

        for(const auto& term : terms)
        {
            if(outputType == Output::Type::Direct || outputType == Output::Type::OverTime)
            {
                float coef = term.coefficient.Next();
                float var = 1.0f;
                switch(term.variable)
                {
                case AttributeInput::STR:
                    var = (float)unit.GetAttributeSheet().GetStrength();
                    break;
                case AttributeInput::AGI:
                    var = (float)unit.GetAttributeSheet().GetAgility();
                    break;
                case AttributeInput::INT:
                    var = (float)unit.GetAttributeSheet().GetIntellect();
                    break;
                case AttributeInput::DEX:
                    var = (float)unit.GetAttributeSheet().GetDexterity();
                    break;
                case AttributeInput::WIS:
                    var = (float)unit.GetAttributeSheet().GetWisdom();
                    break;
                case AttributeInput::KNO:
                    var = (float)unit.GetAttributeSheet().GetKnowledge();
                    break;
                case AttributeInput::VIT:
                    var = (float)unit.GetAttributeSheet().GetVitality();
                    break;
                case AttributeInput::HP:
                    var = (float)unit.GetAttributeSheet().GetMaxHealth();
                    break;
                case AttributeInput::MAP:
                    var = (float)unit.GetAttributeSheet().GetMeleeAttackPower();
                    break;
                case AttributeInput::RAP:
                    var = (float)unit.GetAttributeSheet().GetRangedAttackPower();
                    break;
                // TODO: handle all inputs
                case AttributeInput::LVL:
                    var = (float)unit.GetAttributeSheet().GetLevel();
                    break;
                case AttributeInput::NIL:
                default:
                    var = 1.0f;
                }
                sum += coef * var;
            }
            else 
            {
                data.push_back({term.coefficient.Next(), term.variable});
            }
        }
        Output o;        
        o.target = outputTarget;
        o.school = outputSchool;
        o.type = outputType;
        o.itemCostName = outputItemRequired;
        o.itemCostCount = outputItemCount;
        for(const auto& wpn : weaponsRequired)
        {
            o.requiredWeapons.push_back(wpn);
        }
        // the expression describes the type of output to create and return.
        switch(outputType)
        {
        case Output::Type::Direct:
            o.direct.amount = (int)sum;
            break;
        case Output::Type::OverTime:
            // return Output::OverTime(outputTarget, outputSchool, (int)sum, duration);
            o.overTime.amount = (int)sum;
            o.overTime.duration = duration;
            break;
        case Output::Type::StatusEffect:
            // return Output::StatusEffect(outputTarget, outputSchool, data);
            o.statusEffect.duration = duration;
            for(int i=0; i < data.size(); ++i)
            {
                if(i >= Output::MAX_STATUS_EFFECTS)
                {
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                        "Ran out of status effect slots for output");
                    break;
                }
                o.statusEffect.data[i] = data[i];
            }
            break;
        default:
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Invalid/no output specified for expression result", __FUNCTION__);
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
            // the first char should be '!', '@', or '#'. Anything else is a compile error.
            if(token.size() == 1)
            {
                switch(token[0])
                {
                case '!': currentExpression.outputTarget = Output::Target::Enemy; break;
                case '@': currentExpression.outputTarget = Output::Target::Friendly; break;
                case '#': currentExpression.outputTarget = Output::Target::Self; break;
                default: engine::GameEngine::Get().GetLogger().Logf(
                            engine::Logger::Severity::FATAL,
                            "%s: Wrong target specifier token `%s'!", 
                            __FUNCTION__, token.c_str());
                }
            }
            else 
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                    "%s: Wrong type of token `%s' at beginning of expression!", __FUNCTION__, token.c_str());
            }
            // the next char should be < > or ?
            token = parser.GetNextToken();
            if(token.size() == 1)
            {
                switch(token[0])
                {
                case '<': currentExpression.outputType = Output::Type::Direct; break;
                case '>': currentExpression.outputType = Output::Type::OverTime; break; 
                case '?': currentExpression.outputType = Output::Type::StatusEffect; break;
                default: engine::GameEngine::Get().GetLogger().Logf(
                            engine::Logger::Severity::FATAL,
                            "%s: Incorrect type specifier token `%s'!", 
                            __FUNCTION__, token.c_str());
                }
            }
            else 
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                    "%s: Wrong type of token `%s' for type specifier!", __FUNCTION__, token.c_str());                
            }
            // check weapon requirement (TODO: process this in CombatUnit)
            token = parser.GetNextToken();
            if(token == "~")
            {
                token = parser.GetNextToken();
                for(int i=0; i < token.length(); ++i)
                {
                    switch(token[i])
                    {
                    case 'a': currentExpression.weaponsRequired.push_back(WeaponRequired::Axe); break;
                    case 'h': currentExpression.weaponsRequired.push_back(WeaponRequired::Hammer); break;
                    case 't': currentExpression.weaponsRequired.push_back(WeaponRequired::Staff); break;
                    case 's': currentExpression.weaponsRequired.push_back(WeaponRequired::Sword); break;
                    case 'u': currentExpression.weaponsRequired.push_back(WeaponRequired::Unarmed); break;
                    default: engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                        "%s: Invalid weapon requirement `%c'", __FUNCTION__, token[i]);
                    }
                }
                token = parser.GetNextToken();
            }
            // Check for name of debuff. Order is important btw
            if(token.size() == 0)
            {
                engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                    "%s: Unexpected end of expressions!", __FUNCTION__);
            }
            if(token == "{")
            {
                token = parser.GetNextToken();
                // the semantics of the token are not checked so this allows for a stupid name for
                // an effect such as "-4-5-.-..2" or "!"
                currentExpression.outputName = token;
                token = parser.GetNextToken();
                if(token != "}")
                {
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                        "%s: Incorrect termination of effect name", __FUNCTION__);
                }
                token = parser.GetNextToken();
            }
            
            // Get the coefficient var pairs separated by +
            while(token != "," && token != ";" && token != "")
            {
                // can't create the numeric range until we get both lower and upper
                float lower=1.0f, upper=1.0f;
                AttributeInput var;
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
                var = StringToAttributeInput(token);
                if(var == AttributeInput::INV)
                {
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                        "%s: `%s' is not a valid input attribute", __FUNCTION__, token.c_str());
                }
                // now we can create the Term
                FormulaTerm term = { NumericRange(lower,upper), var };
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
                currentExpression.outputSchool = StringToSchool(token);
                token = parser.GetNextToken();
                // expect a ';' immediately after school. indicated end of expression. 
            }
            // a duration can go after school
            if(token == ":")
            {
                token = parser.GetNextToken();
                if(!StringIsAValidNumber(token))
                {
                    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
                        "%s: Invalid numerical value `%s' for duration", __FUNCTION__, token.c_str());
                }
                // parser does not distinguish between ints and floats so just cast to int
                currentExpression.duration = (int)atof(token.c_str());
                // read the next token (hopefully a ';')
                token = parser.GetNextToken();
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