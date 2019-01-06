// Shader.cpp
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
#include "Shader.h"

#include <string>

#include <GL/glew.h>

#include "ErrorCheck.h"

namespace ogl 
{
    
    Shader::Shader(unsigned int type, const char* src)
    {
        id_ = glCreateShader(type);
        glShaderSource(id_, 1, &src, nullptr);
    }

    Shader::~Shader()
    {
        glDeleteShader(id_);
    }

    bool Shader::Compile() const
    {
        // compile 
        glCompileShader(id_);
        int result;
        glGetShaderiv(id_, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);
            char* message = new char[length];
            glGetShaderInfoLog(id_, length, &length, message);
            std::string msg = message;
            ErrorCheck::PrintErrorf("%s: %s", __FUNCTION__, message);
            delete message;
            return false;
        }
        return true;
    }

}