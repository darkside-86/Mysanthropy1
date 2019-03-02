// SplashScreen.cpp
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

#include "SplashScreen.h"

#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"
#include "ogl/Vertex.h"

SplashScreen::SplashScreen()
{
    // define the vertices for the background.
    float scrW = (float)engine::GameEngine::Get().GetWidth();
    float scrH = (float)engine::GameEngine::Get().GetHeight();

    ogl::Vertex vertices[6] = {
        {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f,0.f}, {0.f,0.f,1.f}}, // top left
        {{0.f,scrH, 0.f}, {255,255,255,255}, {0.f,1.f}, {0.f,0.f,1.f}}, // bottom left
        {{scrW,scrH, 0.f}, {255,255,255,255}, {1.f,1.f}, {0.f,0.f,1.f}}, // bottom right
        {{0.f, 0.f, 0.f}, {255,255,255,255}, {0.f,0.f}, {0.f,0.f,1.f}}, // top left
        {{scrW,scrH, 0.f}, {255,255,255,255}, {1.f,1.f}, {0.f,0.f,1.f}}, // bottom right
        {{scrW, 0.f, 0.f}, {255,255,255,255}, {1.f,0.f}, {0.f,0.f,1.f}}, // top right
    };

    bgVbo_.SetData(sizeof(ogl::Vertex)*6, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    bgVao_.AddBuffer(bgVbo_, vbl);

    // load the background texture of the splashscreen
    bgTexture_ = new ogl::Texture("res/textures/splash.png", false, false);
}

SplashScreen::~SplashScreen()
{
    delete bgTexture_;
}

void SplashScreen::Initialize()
{
    engine::GameEngine::Get().GetSoundManager().PlayMusic("res/music/island3.ogg", -1);
}

void SplashScreen::Update(float dtime)
{

}

void SplashScreen::Render(ogl::Program& program)
{
    glm::mat4 model(1.f), view(1.f), projection(1.f);
    float scrW = (float)engine::GameEngine::Get().GetWidth();
    float scrH = (float)engine::GameEngine::Get().GetHeight();

    projection = glm::ortho(0.f, scrW, scrH, 0.f);
    // first draw the background fullscreen image
    program.Use();
    program.SetUniform<int>("u_useTexture", true);
    program.SetUniform<int>("u_useColorBlending", false);
    program.SetUniform("u_model", model);
    program.SetUniform("u_view", view);
    program.SetUniform("u_projection", projection);
    bgVao_.Bind();
    bgTexture_->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // TODO: draw the UI
}

void SplashScreen::Cleanup()
{
    engine::GameEngine::Get().GetSoundManager().StopMusic();
}