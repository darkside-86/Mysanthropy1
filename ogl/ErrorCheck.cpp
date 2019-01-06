// ErrorCheck.cpp
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

#include "ErrorCheck.h"

#include <cstdarg>
#include <iostream>

#include <GL/glew.h>

namespace ogl
{
	void ErrorCheck::CheckAndReport(const char* location)
	{
		int errorCode = glGetError();
		while (errorCode != GL_NO_ERROR)
		{
			std::cerr << "[OpenGL error @ " << location << "] ";
			switch (errorCode)
			{
			case GL_INVALID_ENUM:
				std::cerr << "An unacceptable value is specified for an enumerated argument. " << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cerr << "A numeric argument is out of range." << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cerr << "The specified operation is not allowed in the current state." << std::endl;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cerr << "The framebuffer object is not complete." << std::endl;
				break;
			case GL_OUT_OF_MEMORY:
				std::cerr << "There is not enough memory left to execute the command." << std::endl;
				break;
			case GL_STACK_UNDERFLOW:
				std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to underflow." 
						  << std::endl;
				break;
			case GL_STACK_OVERFLOW:
				std::cerr << "An attempt has been made to perform an operation that would cause an internal stack to overflow." 
						  << std::endl;
				break;
			}

			errorCode = glGetError();
		}
	}

	void ErrorCheck::PrintErrorf(const char* msg, ...)
	{
        const unsigned int BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        va_list argList;
        va_start(argList, msg);
        vsnprintf(buffer, BUFFER_SIZE, msg, argList);
        fprintf(stderr, "%s\n", buffer);
        fflush(stderr);
        va_end(argList);
	}
}