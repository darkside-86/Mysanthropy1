// Image2D.cpp
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

#include "Image2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "ogl/ErrorCheck.h"
#include "ogl/Vertex.h"

Image2D::Image2D(int width, int height)
{
    float w = (float)width;
    float h = (float)height;
    ogl::Vertex vertices[6] = {
        {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.0f, 0.0f}, {0.f, 0.f, 1.f}},
        {{0.f, h, 0.f}, {255,255,255,255}, {0.0f, 1.0f}, {0.f, 0.f, 1.f}},
        {{w, h, 0.f}, {255,255,255,255}, {1.0f, 1.0f}, {0.f, 0.f, 1.f}},

        {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.0f, 0.0f}, {0.f, 0.f, 1.f}},
        {{w, h, 0.f}, {255,255,255,255}, {1.0f, 1.0f}, {0.f, 0.f, 1.f}},
        {{w, 0, 0.f}, {255,255,255,255}, {1.0f, 0.0f}, {0.f, 0.f, 1.f}},
    };
    vbo_.SetData(sizeof(ogl::Vertex)*6, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_.AddBuffer(vbo_, vbl);
    Pixel* pixels = new Pixel[width*height];
    for(int i=0; i < width*height; ++i)
    {
        pixels[i] = {255,255,255,255}; // fill with...
    }
    texture_ = new ogl::Texture(width, height, pixels, false, false, ogl::Texture::TYPE::DIFFUSE);
    // texture_ = new ogl::Texture("res/textures/sprite.png", false);
    delete [] pixels;
}

Image2D::~Image2D()
{
    delete texture_;
}

void Image2D::Draw(int x, int y, float scaleX, float scaleY, ogl::Program& program)
{
    program.Use();
    vao_.Bind();
    texture_->Bind();
    
    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3((float)x, (float)y, 0.f));
    model = glm::scale(model, glm::vec3(scaleX, scaleY, 1.f));
    program.SetUniform<glm::mat4>("u_model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Image2D::WriteToPNG(const std::string& path)
{
    unsigned int rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int shift = 8;
    rmask = 0xff000000 >> shift;
    gmask = 0x00ff0000 >> shift;
    bmask = 0x0000ff00 >> shift;
    amask = 0x000000ff >> shift;
#else // little endian, like x86
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    int depth, pitch;
    ogl::Texture* t = GetTexture();
    int width = t->GetWidth();
    int height = t->GetHeight();
    depth = 32;
    pitch = 4*width;
    unsigned char* data;
    data = new unsigned char[4*width*height];
    t->Bind();
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, width, height, depth, pitch, rmask, gmask, bmask, amask);
    IMG_SavePNG(surface, path.c_str());
    SDL_FreeSurface(surface);
    delete [] data;
}