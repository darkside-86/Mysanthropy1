// Shader.hpp
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

namespace ogl
{
    // Encapsulates individual shaders. Used by Program to compile a full
    //  GLSL program of vertex, fragment, etc. shaders.
    class Shader
    {
    public:
        // Constructor.
        //  type - e.g. GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.
        Shader(unsigned int type, const char* src);
        // Destructor. Cleans up GL allocated shader object
        ~Shader();
        // Compiles the source code. Returns true if successful, otherwise false
        bool Compile() const;
    private:
        Shader(const Shader&) {} // disable copying
        void operator=(const Shader&) {}
        // GL object handle
        unsigned int id_;
        // Only Program should access id_
        friend class Program;
    };
}