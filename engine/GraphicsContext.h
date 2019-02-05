// GraphicsContext.h
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

#include <glm/glm.hpp>

#include <stack>

#include "ogl/ElementBuffer.h"
#include "ogl/Program.h"
#include "ogl/Texture.h"
#include "ogl/Vertex.h"
#include "ogl/VertexArray.h"

namespace engine
{
    class GraphicsContext
    {
    public:
        GraphicsContext();
        virtual ~GraphicsContext();
        //=== Core API ===
        // Select the program for use
        inline void UseProgram() { program_.Use(); }
        // Toggle texture usage in program
        inline void SetUseTexture(bool b) 
        { 
            UseProgram();
            program_.SetUniform<int>("u_useTexture", b ? GL_TRUE : GL_FALSE);
        }
        // Toggle color blending with textures (for when u_useTexture is on)
        inline void SetUseColorBlending(bool b)
        {
            UseProgram();
            program_.SetUniform<int>("u_useColorBlending", b ? GL_TRUE : GL_FALSE);
        }
        // Begin the rendering process by resetting the MVP matrix data
        void ResetMVP();
        // Store the calculated MVP as a uniform in the shader
        void SetMVP();
        // Sets the orthographic projection. Called between Begin() and End()
        void SetOrthoProjection(float left, float right, float top, float bottom);
        // Sets the orthographic projection to the logical dimensions of the
        //  game screen. Called between Begin() and End()
        void SetOrthoProjection();
        // Sets a perspective (3D) projection. Called between Begin() and End()
        //  fovy - Field of view angle in degrees
        //  ratio - The aspect ratio
        void SetPerspectiveProjection(float fovy, float ratio, float near, float far);
        // Sets the view matrix
        inline void SetView(const glm::mat4 view) { view_ = view; }
        // performs a translate operation on the model matrix
        void TranslateModel(float x, float y, float z);
        // rotate
        void RotateModel(float angle, float x, float y, float z);
        // resets the model matrix
        inline void ResetModel() { model_ = glm::mat4(1.0f); }

        void PushModel();
        void PopModel();
    private:
        // A basic GLSL program for implementing functions of this class.
        //  Uniforms are used to set the MVP and texture.
        // TODO: write separate program for 2D primitive rendering.
        ogl::Program program_;
        // The components for building the MVP matrix in End()
        glm::mat4 model_, view_, projection_;

        std::stack<glm::mat4> modelStack_;
    };
}