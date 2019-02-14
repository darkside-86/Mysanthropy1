// Skybox.cpp
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

#include "Skybox.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "engine/GameEngine.h"
#include "ogl/ErrorCheck.h"

Skybox::Skybox(const std::vector<std::string>& faces)
{
    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID_);
    for(unsigned int i=0; i < faces.size(); ++i)
    {
        SDL_Surface* loadedSurface = IMG_Load(faces[i].c_str());
        if(loadedSurface == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                    "%s", IMG_GetError());
            return;
        }
        SDL_Surface* formatted = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA32, 0);
        SDL_FreeSurface(loadedSurface);
        if(formatted == nullptr)
        {
            engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                    "%s", SDL_GetError());
            return;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, formatted->w, formatted->h, 0, 
                     GL_RGBA, GL_UNSIGNED_BYTE, formatted->pixels);
        SDL_FreeSurface(formatted);        
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    float vertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    vbo_.SetData(36*3*sizeof(float), vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    vbl.Push<float>(3, false);
    vao_.AddBuffer(vbo_, vbl);
}

Skybox::~Skybox()
{
    glDeleteTextures(1, &textureID_);
}

void Skybox::Render(ogl::Program& program)
{
    program.Use();
    glDepthFunc(GL_LEQUAL);
    vao_.Bind();
    program.SetUniform<int>("u_skybox", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    OGL_ERROR_CHECK();
    glDepthFunc(GL_LESS);
}
