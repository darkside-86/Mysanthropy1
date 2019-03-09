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

    Output Expression::RunCalculation(CombatUnit& unit) const
    {
        // first get the sum of the algebraic expression
        float sum = 0;
        for(const auto& term : terms)
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
            case AttributeInput::LVL:
                var = (float)unit.GetAttributeSheet().GetLevel();
                break;
            case AttributeInput::NIL:
            default:
                var = 1.0f;
            }
            sum += coef * var;
        }
        // the expression describes the type of output to create and return.
        switch(outputType)
        {
        case Output::Type::Direct:
            return Output::Direct(outputTarget, outputSchool, (int)sum);
            break;
        case Output::Type::OverTime:
            return Output::OverTime(outputTarget, outputSchool, (int)sum, duration);
            break;
        case Output::Type::StatusEffect:
            return Output::StatusEffect(outputTarget, outputSchool, multiplier, buffAttrib);
            break;
        default:
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL, 
                "%s: Invalid/no output specified for expression result", __FUNCTION__);
        }
        return *(Output*)nullptr; // never reached due to FATAL but here to satisfy compiler
    }

    Formula::Formula(const std::string& formulaText)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::FATAL,
            "%s: Not implemented yet!", __FUNCTION__);
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