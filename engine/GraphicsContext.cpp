// GraphicsContext.cpp
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

#include <fstream>
#include <string>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.hpp"
#include "GraphicsContext.hpp"
#include "ogl/ElementBuffer.hpp"
#include "ogl/Shader.hpp"

namespace engine
{
    GraphicsContext::GraphicsContext()
    {
        std::string vertexSrc, fragmentSrc, str;
        std::ifstream inFile;
        
        inFile.open("res/shaders/basic.vs");
        if(!inFile.is_open())
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                "%s: Unable to open shader file", __FUNCTION__);
        }
        while(std::getline(inFile, str))
        {
            vertexSrc += str + "\n";
        }
        inFile.close();
        
        inFile.open("res/shaders/basic.fs");
        if(!inFile.is_open())
        {
            GameEngine::Get().GetLogger().Logf(Logger::Severity::WARNING,
                "%s: Unable to open shader file", __FUNCTION__);
        }
        while(std::getline(inFile,str))
        {
            fragmentSrc += str + "\n";
        }
        inFile.close();

        ogl::Shader vertex(GL_VERTEX_SHADER, vertexSrc.c_str());
        ogl::Shader fragment(GL_FRAGMENT_SHADER, fragmentSrc.c_str());
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
        // glm::mat4 mvp = projection_ * view_ * model_;
        program_.Use();
        // program_.SetUniform<glm::mat4>("u_MVP", mvp);
        program_.SetUniform<glm::mat4>("u_model", model_);
        program_.SetUniform<glm::mat4>("u_view", view_);
        program_.SetUniform<glm::mat4>("u_projection", projection_);
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

    void GraphicsContext::ScaleModel(float x, float y, float z)
    {
        model_ = glm::scale(model_, glm::vec3(x,y,z));
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