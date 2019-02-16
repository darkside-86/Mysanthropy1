// Program.h
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

#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Shader.h"

namespace ogl
{

    class Program
    {
    public:
        Program();
        virtual ~Program();
        // Binds this GLSL program
        void Use();
        void Unuse();
        // Compiles shaders.
        bool CompileShaders(const Shader& vertex, const Shader& fragment);
        bool CompileShaders(const Shader& vertex, const Shader& fragment, const Shader& geometry);
        // Sets a uniform value associated with the GLSL program. Use()
        //  should be called before SetUniform() if program is not bound.
        template <typename T>
        void SetUniform(const std::string& name, const T& u) 
        { static_assert(false); } // not handled
        template<>
        void SetUniform(const std::string& name, const int& i)
        {
            glUniform1i(FindUniform(name), i);
        }
        template<>
        void SetUniform(const std::string& name, const float& f)
        {
            glUniform1f(FindUniform(name), f);
        }
        template<>
        void SetUniform(const std::string& name, const glm::vec2& v) 
        {
            glUniform2f(FindUniform(name), v.x, v.y);
        }
        template<>
        void SetUniform(const std::string& name, const glm::vec3& v) 
        {
            glUniform3f(FindUniform(name), v.x, v.y, v.z);
        }
        template<>
        void SetUniform(const std::string& name, const glm::vec4& v) 
        {
            glUniform4f(FindUniform(name), v.x, v.y, v.z, v.w);
        }
        template<>
        void SetUniform(const std::string& name, const glm::mat4& m)
        {
            glUniformMatrix4fv(FindUniform(name), 1, GL_FALSE, &m[0][0]);
        }
    private:
        int FindUniform(const std::string &name);
        unsigned int id_;
        std::unordered_map<std::string, int> uniformCache_;
    };

}