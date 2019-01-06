// Program.cpp
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
#include "Program.h"

#include <GL/glew.h>

#include "ErrorCheck.h"

namespace ogl
{
    Program::Program()
    {
        id_ = glCreateProgram();
    }

    Program::~Program()
    {
        glDeleteProgram(id_);
    }
        
    void Program::Use()
    {
        glUseProgram(id_);
    }
        
    void Program::Unuse()
    {
        glUseProgram(0);
    }

    bool Program::CompileShaders(const Shader& vertex, const Shader& fragment)
    {
        // compile
        bool ok = vertex.Compile() &&
                  fragment.Compile();
        if(!ok) return false;
        glAttachShader(id_, vertex.id_);
        glAttachShader(id_, fragment.id_);
        glLinkProgram(id_);
        // link
        int result;
        glGetProgramiv(id_, GL_LINK_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            std::string errMsg;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);
            char* message = new char[length];
            glGetProgramInfoLog(id_, length, &length, message);
            errMsg = message;
            ErrorCheck::PrintErrorf("%s: %s", __FUNCTION__, message);
            delete[] message;
            return false;
        }
        // validate
        glValidateProgram(id_);
        glGetProgramiv(id_, GL_VALIDATE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            std::string errMsg;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);
            char* message = new char[length];
            glGetProgramInfoLog(id_, length, &length, message);
            errMsg = message;
            ErrorCheck::PrintErrorf("%s: %s", __FUNCTION__, message);
            delete[] message;
            return false;
        }
        return true;
    }

    int Program::FindUniform(const std::string &name)
    {
        int location = -1; // -1 is not found

		// look in cache first
		auto cachedLocation = uniformCache_.find(name);
		if (cachedLocation == uniformCache_.end())
		{
			// look in opengl and cache it
			location = glGetUniformLocation(id_, name.c_str());
			if (location == -1)
			{
				ErrorCheck::PrintErrorf("%s: Unable to find uniform `%s'", 
                                        __FUNCTION__,  name.c_str());
				return -1;
			}
			// we found it so add it to the cache
			uniformCache_[name] = location;
		}
		else
		{
			location = cachedLocation->second;
		}

		return location;
    }
}