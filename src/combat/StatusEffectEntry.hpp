// StatusEffectEntry.hpp
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
#include "Formula.hpp"
#include "NumericRange.hpp"
#include "OutputType.hpp"

namespace combat
{
    // defines a debuff/buff/hot/dot in database
    class StatusEffectEntry
    { public:
        class Add { public: NumericRange range; Attribute attribute; };
        class Mult { public: NumericRange range; Attribute attribute; };

        // key into database
        std::string name; 
        // title for UI
        std::string title;
        // tooltip for UI
        std::string tooltip;
        // icon for the effect
        std::string icon;
        // whether or not it should be hidden from UI
        bool hidden = false;
        // rate at which damage effect ticks in seconds
        int tickRate = 0;
        // whether or not this ability is offensive (for determining dispels)
        bool offensive = false;
        // an ID for category of buff, to limit the number of the type
        std::string group;
        // whether or not multiples of the group type can be stacks
        bool groupUnique = true;
        // whether or not this is considered a harmful effect
        bool harmful = true;
        // maximum amount of the same effect type on one unit, -1 for unlimited
        int maxStacks = 1;
        // damage/healing info for each tick
        Formula formula;
        // buff info
        std::vector<Add> adds;
        std::vector<Mult> mults;
        // output type
        OutputType outputType = OutputType::Physical;
        // can be dispelled
        bool dispel = false;
    };
}