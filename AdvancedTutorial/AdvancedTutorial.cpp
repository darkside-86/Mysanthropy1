#include "AdvancedTutorial.h"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"
#include "engine/ui/Root.h"
#include "ogl/Texture.h"

AdvancedTutorial::AdvancedTutorial()
{
    {
        std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/atshader.vs");
        std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/atshader.frag");
        ogl::Shader vertexShader(GL_VERTEX_SHADER, vsSrc.c_str());
        ogl::Shader fragmentShader(GL_FRAGMENT_SHADER, fsSrc.c_str());
        program_.CompileShaders(vertexShader, fragmentShader);
    }
    {
        std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/stencils.vs");
        std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/stencils.frag");
        ogl::Shader vertexShader(GL_VERTEX_SHADER, vsSrc.c_str());
        ogl::Shader fragmentShader(GL_FRAGMENT_SHADER, fsSrc.c_str());
        colorProgram_.CompileShaders(vertexShader, fragmentShader);
    }
    simpleCube_ = new SimpleCube();
    cubeTexture_ = engine::GameEngine::Get().GetTextureManager().GetTexture("res/textures/container.png");
    simplePlane_ = new SimplePlane();
    scripting_ = luaL_newstate();
    luaL_openlibs(scripting_);
    luaBindings_ = new engine::ui::LuaBindings(scripting_);
}

AdvancedTutorial::~AdvancedTutorial()
{
    engine::GameEngine::Get().GetTextureManager().UnloadTexture("res/textures/container.png");
    delete simpleCube_;
    delete simplePlane_;
    delete luaBindings_;
    lua_close(scripting_);
}

bool AdvancedTutorial::Initialize()
{
    camera_.SetPosition(glm::vec3(0.f,0.f,5.5f));
    camera_.SetDirection(glm::vec3(0.f,0.f,-1.f));

    engine::ui::Root::Get()->Initialize();

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

    return true;
}

void AdvancedTutorial::Cleanup()
{

}

void AdvancedTutorial::Update(float dtime)
{

}

void AdvancedTutorial::Render(engine::GraphicsContext& gc)
{

    glm::mat4 model(1.f), view(1.f), projection(1.f);
    float width = (float)engine::GameEngine::Get().GetWidth();
    float height = (float)engine::GameEngine::Get().GetHeight();

    view = camera_.CalculateView();
    projection = glm::perspective(45.f, width/height, 0.01f, 100.f);
    glClearColor(0.0f,0.1f,0.1f,1.f);

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
    simpleCube_->Render(program_);
    model = glm::translate(model, glm::vec3(0.5f,0.f,-1.f));
    colorProgram_.SetUniform<glm::mat4>("u_model", model);
    simpleCube_->Render(program_);
    glStencilMask(0xff);
    glEnable(GL_DEPTH_TEST);

    engine::ui::Root::Get()->Render(gc);
}