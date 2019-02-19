// AdvancedTutorial.cpp 
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

#include "AdvancedTutorial.h"

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "engine/GameEngine.h"
#include "engine/ui/Root.h"
#include "ogl/ErrorCheck.h"
#include "ogl/Texture.h"

static void lua_PushMat4(lua_State* L, const glm::mat4 matrix);

AdvancedTutorial::AdvancedTutorial()
{
    {
        std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/atshader.vs");
        std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/atshader.fs");
        ogl::Shader vertexShader(GL_VERTEX_SHADER, vsSrc.c_str());
        ogl::Shader fragmentShader(GL_FRAGMENT_SHADER, fsSrc.c_str());
        program_.CompileShaders(vertexShader, fragmentShader);
    }
    {
        std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/stencils.vs");
        std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/stencils.fs");
        ogl::Shader vertexShader(GL_VERTEX_SHADER, vsSrc.c_str());
        ogl::Shader fragmentShader(GL_FRAGMENT_SHADER, fsSrc.c_str());
        colorProgram_.CompileShaders(vertexShader, fragmentShader);
    }
    {
        std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/screen.vs");
        std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/screen.fs");
        ogl::Shader vertexShader(GL_VERTEX_SHADER, vsSrc.c_str());
        ogl::Shader fragmentShader(GL_FRAGMENT_SHADER, fsSrc.c_str());
        screenProgram_.CompileShaders(vertexShader, fragmentShader);
    }
    {
        std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/skybox.vs");
        std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/skybox.fs");
        ogl::Shader vertexShader(GL_VERTEX_SHADER, vsSrc.c_str());
        ogl::Shader fragmentShader(GL_FRAGMENT_SHADER, fsSrc.c_str());
        skyboxProgram_.CompileShaders(vertexShader, fragmentShader);
    }
    simpleCube_ = new SimpleCube();
    cubeTexture_ = engine::GameEngine::Get().GetTextureManager().GetTexture("res/textures/container.png");
    simplePlane_ = new SimplePlane();
    scripting_ = luaL_newstate();
    luaL_openlibs(scripting_);
    luaBindings_ = new engine::ui::LuaBindings(scripting_);
    for(int i=0; i < NUM_GRASSES; ++i)
    {
        grasses_[i] = new GrassObject(glm::vec3((float)(2*i-4), 0.f, 0.f));
    }
    screenQuad_ = new ScreenQuad();
    fbo_ = new ogl::FrameBuffer();
    fbo_->AttachNewTexture(engine::GameEngine::Get().GetWidth(), engine::GameEngine::Get().GetHeight());
    fbo_->AttachNewRenderBuffer(engine::GameEngine::Get().GetWidth(), engine::GameEngine::Get().GetHeight());
    if(!fbo_->IsComplete())
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                "Incomplete framebuffer!");
    fbo_->Unbind();

    /*skybox_ = LoadCubemap({
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg"
    });*/

    skybox_ = new Skybox({
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/front.jpg",
        "res/textures/skybox/back.jpg"
    });

}

AdvancedTutorial::~AdvancedTutorial()
{
    engine::GameEngine::Get().GetTextureManager().UnloadTexture("res/textures/container.png");
    delete simpleCube_;
    delete simplePlane_;
    delete luaBindings_;
    lua_close(scripting_);
    for(int i=0; i < NUM_GRASSES; ++i)
        delete grasses_[i];
    delete screenQuad_;
    delete fbo_;
    delete skybox_;
}

bool AdvancedTutorial::Initialize()
{
    camera_.SetPosition(glm::vec3(0.f,0.f,5.5f));
    camera_.SetDirection(glm::vec3(0.f,0.f,-1.f));

    engine::ui::Root::Get()->Initialize();
    engine::ui::Root::Get()->AddOnTimer([](const engine::ui::TimerEvent& e){
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, 
                "uiroot after 3 seconds!");
    },  3000);

    engine::GameEngine::Get().AddMouseMotionListener([this](const SDL_MouseMotionEvent& e){
        camera_.RotateDirection(glm::radians((float)-e.xrel*6.f));
        camera_.RotateUpSide(glm::radians((float)e.yrel*6.f));
        camera_.SetUp(glm::vec3(0.f,1.f,0.f));
    });

    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
        if(e.type == SDL_KEYDOWN)
        {
            float speed = 0.5f;
            switch(e.keysym.sym)
            {
            case SDLK_w:
                camera_.MoveFront(speed);
                break;
            case SDLK_s:
                camera_.MoveFront(-speed);
                break;
            case SDLK_a:
                camera_.MoveSide(-speed);
                break;
            case SDLK_d:
                camera_.MoveSide(speed);
                break;
            }
        }
    });

    std::vector<const char*> CORE_UI_LIB = {
        "ui/fonts.lua", "ui/keycodes.lua", "ui/window.lua", "ui/advancedtutorial.lua"
    };
    try 
    {
        int errCode = 0;
        for(auto luaFile : CORE_UI_LIB)
        {
            errCode = luaL_dofile(scripting_, luaFile);
            if(errCode != 0) throw errCode;
        }
    }
    catch(int err)
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING,
                "Lua error %d: %s", err, lua_tostring(scripting_, -1));
        lua_pop(scripting_, 1);
    }
    lua_pushstring(scripting_, "AdvancedTutorial");
    lua_pushlightuserdata(scripting_, this);
    lua_settable(scripting_, LUA_REGISTRYINDEX);
    lua_pushcfunction(scripting_, AdvancedTutorial::GetViewMatrix);
    lua_setglobal(scripting_, "GetViewMatrix");

    return true;
}

void AdvancedTutorial::Cleanup()
{

}

void AdvancedTutorial::Update(float dtime)
{
    engine::ui::Root::Get()->Update(dtime);
}

void AdvancedTutorial::Render(engine::GraphicsContext& gc)
{

    glm::mat4 model(1.f), view(1.f), projection(1.f);
    float width = (float)engine::GameEngine::Get().GetWidth();
    float height = (float)engine::GameEngine::Get().GetHeight();

    view = camera_.CalculateView();
    projection = glm::perspective(45.f, width/height, 0.01f, 100.f);
    
    /*fbo_->Bind();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f,0.1f,0.1f,1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);*/

    program_.Use();
    program_.SetUniform<glm::mat4>("u_view", view);
    program_.SetUniform<glm::mat4>("u_projection", projection);
    program_.SetUniform<int>("u_texture1", 0);

    glStencilMask(0x00);
    program_.Use();
    model = glm::mat4(1.f);
    program_.SetUniform<glm::mat4>("u_model", model);
    simplePlane_->Render();

    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);
    model = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.501f,0.f));
    program_.SetUniform<glm::mat4>("u_model", model);
    cubeTexture_->Bind();
    simpleCube_->Render(program_);
    model = glm::translate(model, glm::vec3(0.5f,0.f,-1.f));
    program_.SetUniform<glm::mat4>("u_model", model);
    simpleCube_->Render(program_);

    for(int i=0; i < NUM_GRASSES; ++i)
    {
        grasses_[i]->Render(program_);
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xff);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    colorProgram_.Use();
    colorProgram_.SetUniform<glm::mat4>("u_view", view);
    colorProgram_.SetUniform<glm::mat4>("u_projection", projection);
    model = glm::translate(glm::mat4(1.f), glm::vec3(0.f,0.501f,0.f));
    model = glm::scale(model, glm::vec3(1.05f,1.05f,1.05f));
    colorProgram_.SetUniform<glm::mat4>("u_model", model);
    cubeTexture_->Bind();
    OGL_ERROR_CHECK();
    simpleCube_->Render(program_);
    model = glm::translate(model, glm::vec3(0.5f,0.f,-1.f));
    colorProgram_.SetUniform<glm::mat4>("u_model", model);
    simpleCube_->Render(program_);
    glStencilMask(0xff);
    glEnable(GL_DEPTH_TEST);

    skyboxProgram_.Use();
    skyboxProgram_.SetUniform<glm::mat4>("u_view", glm::mat4(glm::mat3(camera_.CalculateView())));
    skyboxProgram_.SetUniform<glm::mat4>("u_projection", projection);
    skybox_->Render(skyboxProgram_);

    // update lua data
    lua_PushMat4(scripting_, view);
    lua_setglobal(scripting_, "g_viewMatrix");
    lua_PushMat4(scripting_, projection);
    lua_setglobal(scripting_, "g_projectionMatrix");
    luaL_dostring(scripting_, "UpdateMatrixFrames()");

    engine::ui::Root::Get()->Render(gc);

    /*screenProgram_.Use();
    screenProgram_.Use();
    screenProgram_.SetUniform<int>("u_screenTexture", 0);
    fbo_->Unbind();
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.f,1.f,1.f,1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    fbo_->BindTexture();
    screenQuad_->Render(screenProgram_);*/
}

static void lua_PushMat4(lua_State* L, const glm::mat4 matrix)
{
    lua_newtable(L); 
    for(int i=1; i <= 4; ++i)
    {
        lua_pushinteger(L, i);
        lua_newtable(L);
        for(int j=1; j <=4; ++j)
        {
            lua_pushinteger(L, j);
            lua_pushnumber(L, matrix[i-1][j-1]);
            lua_settable(L, -3);
        }
        lua_settable(L, -3);
    }
}

int AdvancedTutorial::GetViewMatrix(lua_State* L)
{
    lua_pushstring(L, "AdvancedTutorial");
    lua_gettable(L, LUA_REGISTRYINDEX);
    AdvancedTutorial* at = (AdvancedTutorial*)lua_touserdata(L, -1);
    lua_pop(L, 1);
    glm::mat4 view = at->camera_.CalculateView();
    lua_PushMat4(L, view);
    return 1;
}