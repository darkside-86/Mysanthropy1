// NumericRange.hpp
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

#include "engine/GameEngine.hpp"
#include "NumericRange.hpp"

namespace combat
{
    NumericRange::NumericRange(float lower, float upper) : lower_(lower), upper_(upper) 
    {   // validate bounds
        if(lower > upper)
        {
            float swap = upper;
            upper = lower;
            lower = swap;
        }
    }

    float NumericRange::Next() const
    {
        float r = upper_ - lower_;
        if(r == 0.0f) // don't waste RNG time if no range
            return lower_;
        auto& rng = engine::GameEngine::Get().GetRNG();
        float v = (float)rng() / (float)rng.max();
        return lower_ + r * v;
    }
}