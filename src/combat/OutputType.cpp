// OutputType.cpp
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

#include "OutputType.hpp"

namespace combat
{
    OutputType StringToOutputType(const std::string& str)
    {
        if(str=="Unspecified") return OutputType::Unspecified;
        else if(str=="MagicUnspecified") return OutputType::MagicUnspecified;
        else if(str=="Physical") return OutputType::Physical;
        else if(str=="Earth") return OutputType::Earth;
        else if(str=="Air") return OutputType::Air;
        else if(str=="Fire") return OutputType::Fire;
        else if(str=="Frost") return OutputType::Frost;
        else if(str=="Shadow") return OutputType::Shadow;
        else if(str=="Holy") return OutputType::Holy;
        else if(str=="Nature") return OutputType::Nature;
        else if(str=="Life") return OutputType::Life;

        return OutputType::Unspecified;
    }

    std::string OutputTypeToString(const OutputType t)
    {
        switch(t)
        {
        case OutputType::Unspecified: return "Unspecified";
        case OutputType::MagicUnspecified: return "MagicUnspecified";
        case OutputType::Physical: return "Physical";
        case OutputType::Earth: return "Earth";
        case OutputType::Air: return "Air";
        case OutputType::Fire: return "Fire";
        case OutputType::Frost: return "Frost";
        case OutputType::Shadow: return "Shadow";
        case OutputType::Holy: return "Holy";
        case OutputType::Nature: return "Nature";
        case OutputType::Life: return "Life";
        }

        return "Unspecified";
    }
}