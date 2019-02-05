// GraphicsContext.cpp
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

#include "GraphicsContext.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"
#include "ogl/ElementBuffer.h"
#include "ogl/Shader.h"

namespace engine
{
    static const char* FRAGMENT_SHADER_SRC = R"(
        #version 330 core

        in vec4 v_color;
        in vec2 v_texCoord;

        uniform sampler2D u_texture;
        uniform bool u_useTexture;
        uniform bool u_useColorBlending;

        out vec4 color;

        void main() 
        {
            if(u_useTexture)
            {
                if(u_useColorBlending)
                    color = texture(u_texture, v_texCoord) * v_color;
                else
                    color = texture(u_texture, v_texCoord);
            }
            else
            {
                color = v_color;
                // color = vec4(1.0,1.0,1.0,1.0);
            }

        }
    )";

    static const char* VERTEX_SHADER_SRC = R"(
        #version 330 core

        // vertex position
        layout (location=0) in vec3 position;

        // 4 component RGBA normalized color
        layout (location=1) in vec4 color;

        // tex coords
        layout (location=2) in vec2 texCoords;

        // send texture coordinates and color to fragment shader
        out vec4 v_color;
        out vec2 v_texCoord;

        // model view projection matrix
        uniform mat4 u_MVP;

        void main() 
        {
            // apply the matrix to the positon
            gl_Position = u_MVP * vec4(position, 1.0);
            // gl_Position = vec4(position,1.0);
            // set the texture coordinates to their value for fragment shader
            v_texCoord = texCoords;
            // set the color
            v_color = color;
        }
    )";

    GraphicsContext::GraphicsContext()
    {
        ogl::Shader fragment(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
        ogl::Shader vertex(GL_VERTEX_SHADER, VERTEX_SHADER_SRC);
        if(!program_.CompileShaders(vertex, fragment))
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                "%s: Shader compilation failed.", __FUNCTION__);
        }
        model_ = glm::mat4(1.0f);
        view_ = glm::mat4(1.0f);
        projection_ = glm::mat4(1.0f);
    }

    GraphicsContext::~GraphicsContext()
    {

    }

    void GraphicsContext::ResetMVP()
    {
        model_ = glm::mat4(1.0f);
        view_ = glm::mat4(1.0f);
        projection_ = glm::mat4(1.0f);
    }

    void GraphicsContext::SetMVP()
    {
        glm::mat4 mvp = projection_ * view_ * model_;
        program_.Use();
        program_.SetUniform<glm::mat4>("u_MVP", mvp);
    }

    void GraphicsContext::SetOrthoProjection(float left, float right, float top, float bottom)
    {
        projection_ = glm::ortho(left, right, top, bottom, -1.0f, 1.0f);
    }

    void GraphicsContext::SetOrthoProjection()
    {
        float left = 0.f, bottom=0.0f;
        float right = (float)GameEngine::Get().GetWidth();
        float top = (float)GameEngine::Get().GetHeight();
        projection_ = glm::ortho(left, right, top, bottom, -1.0f, 1.0f);
    }

    void GraphicsContext::SetPerspectiveProjection(float fovy, float ratio, float near, float far)
    {
        projection_ = glm::perspective(glm::radians(fovy), ratio, near, far);
    }

    void GraphicsContext::TranslateModel(float x, float y, float z)
    {
        model_ = glm::translate(model_, glm::vec3(x,y,z));
    }

    void GraphicsContext::RotateModel(float angle, float x, float y, float z)
    {
        model_ = glm::rotate(model_, glm::radians(angle), glm::vec3(x,y,z));
    }

    void GraphicsContext::PushModel()
    {
        modelStack_.push(model_);
    }

    void GraphicsContext::PopModel()
    {
        if(modelStack_.size() > 0)
        {
            model_ = modelStack_.top();
            modelStack_.pop();
        }
    }

}