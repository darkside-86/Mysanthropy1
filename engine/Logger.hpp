// Logger.hpp
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

namespace engine
{
    // Defines a basic logging utility.
    // Only accessible through GameEngine singleton. This is to prevent multiple
    // Logger instances writing to the same log.txt file.
    class Logger
    {
    public:
        // Level of severity of message
        enum class Severity { WARNING, INFO, ERROR, FATAL };
        // Logs a formatted string with sprintf.
        //  sev - If FATAL, an assert(false) statement will be called.
        static void Logf(const Severity& sev, const char* format, ...);
    private:
        // constructor
        Logger();
        // destructor
        virtual ~Logger();

        friend class GameEngine;
    };
}