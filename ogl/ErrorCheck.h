// ErrorCheck.h
//-----------------------------------------------------------------------------
// Author: darkside-86
// (c) 2018
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

#include <sstream>
#include <string>

// Checks for OpenGL errors and reports the file and line number where the 
//  error check catches the error.
#define OGL_ERROR_CHECK() ogl::ErrorCheck::CheckAndReport((std::stringstream()<<__FILE__<<":"<<__LINE__).str().c_str())

namespace ogl
{
	// A set of static methods for the OpenGL binding classes to report errors
	//  to stderr
	class ErrorCheck
	{
	public:
		// See OGL_ERROR_CHECK
		static void CheckAndReport(const char* location);
		// Prints a formatted string to stderr (for now)
		static void PrintErrorf(const char* msg, ...);
	};
}