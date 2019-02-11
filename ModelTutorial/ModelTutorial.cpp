#include "ModelTutorial.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/GameEngine.h"

#include "ogl/ErrorCheck.h"

ModelTutorial::ModelTutorial()
{
    camera_ = new engine::Camera();
    lampObject_ = new LampObject();
    {
        modelProgram_ = new ogl::Program();
        std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/model.vs");
        std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/model.frag");
        ogl::Shader vs(GL_VERTEX_SHADER, vsSrc.c_str());
        ogl::Shader fs(GL_FRAGMENT_SHADER, fsSrc.c_str());
        modelProgram_->CompileShaders(vs, fs);
    }
    {
        lightProgram_ = new ogl::Program();
        std::string vsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/lighting.vs");
        std::string fsSrc = engine::GameEngine::Get().ReadFileAsString("res/shaders/lighting.frag");
        ogl::Shader vs(GL_VERTEX_SHADER, vsSrc.c_str());
        ogl::Shader fs(GL_FRAGMENT_SHADER, fsSrc.c_str());
        lightProgram_->CompileShaders(vs, fs);
    }
    engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::INFO, "Loading model...");
    // model_ = new Model("res/models/nanosuit/nanosuit.obj");
    model_ = new Model("res/models/elefante/elefante.obj");
    // model_ = new Model("res/models/Z3_OBJ.obj");
}
    
ModelTutorial::~ModelTutorial()
{
    delete camera_;
    delete lampObject_;
    delete modelProgram_;
    delete lightProgram_;
    delete model_;
}
    
bool ModelTutorial::Initialize()
{
    camera_->SetPosition(glm::vec3(0.0f, 0.0f, 5.f));

    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
        if(e.type == SDL_KEYDOWN)
        {
            switch(e.keysym.sym)
            {
            case SDLK_a:        camera_->MoveSide(-0.5f); break;
            case SDLK_d:        camera_->MoveSide(0.5f); break;
            case SDLK_w:        camera_->MoveFront(0.5f); break;
            case SDLK_s:        camera_->MoveFront(-0.5f); break;
            case SDLK_q:        camera_->RotateUp(-15.f); break;
            case SDLK_e:        camera_->RotateUp(15.f); break;
            case SDLK_SPACE:    camera_->MoveUp(0.5f); break;
            case SDLK_LSHIFT:   camera_->MoveUp(-0.5f); break;
            case SDLK_LEFT:     camera_->RotateDirection(15.f); break;
            case SDLK_RIGHT:    camera_->RotateDirection(-15.f); break;
            case SDLK_UP:       camera_->RotateUpSide(15.f);  break;
            case SDLK_DOWN:     camera_->RotateUpSide(-15.f); break;
            }
        }
    });

    return true;
}

void ModelTutorial::Cleanup()
{

}

void ModelTutorial::Update(float dtime)
{

}

void ModelTutorial::Render(engine::GraphicsContext& gc)
{
    float width = (float)engine::GameEngine::Get().GetWidth();
    float height = (float)engine::GameEngine::Get().GetHeight();
    glClearColor(0.2f, 0.1f, 0.2f, 1.f);
    glm::mat4 model(1.f), view(1.f), projection(1.f);
    ogl::Program& program = gc.GetProgram();
    view = camera_->CalculateView();
    projection = glm::perspective(45.f, width/height, 0.01f, 100.f);
    program.Use();
    program.SetUniform<glm::mat4>("u_model", model);
    program.SetUniform<glm::mat4>("u_view", view);
    program.SetUniform<glm::mat4>("u_projection", projection);
    lampObject_->Render(program);
    // render model
    modelProgram_->Use();
    modelProgram_->SetUniform<glm::mat4>("u_model", model);
    modelProgram_->SetUniform<glm::mat4>("u_view", view);
    modelProgram_->SetUniform<glm::mat4>("u_projection", projection);
    // model_->Draw(*modelProgram_);
    // attempt to render smaller version with lighting shader
    model = glm::translate(model, glm::vec3(0.0f,1.f,0.f));
    model = glm::scale(model, glm::vec3(0.1f,0.1f,0.1f));
    lightProgram_->Use(); 
    lightProgram_->SetUniform<glm::mat4>("u_model", model);
    lightProgram_->SetUniform<glm::mat4>("u_view", view);
    lightProgram_->SetUniform<glm::mat4>("u_projection", projection);
    lightProgram_->SetUniform<glm::vec3>("u_dirLight.direction", glm::vec3(-1.f, -1.f, 0.f));
    lightProgram_->SetUniform<glm::vec3>("u_dirLight.ambient", glm::vec3(0.1f,0.1f,0.1f));
    lightProgram_->SetUniform<glm::vec3>("u_dirLight.diffuse", glm::vec3(0.3f,0.3f,0.3f));
    lightProgram_->SetUniform<glm::vec3>("u_dirLight.specular", glm::vec3(0.5f,0.4f,0.4f));
    lightProgram_->SetUniform<glm::vec3>("u_spotLight.position", camera_->GetPosition());
    lightProgram_->SetUniform<glm::vec3>("u_spotLight.direction", camera_->GetDirection());
    lightProgram_->SetUniform<float>("u_spotLight.cutOff", 12.f);
    lightProgram_->SetUniform<float>("u_spotLight.outerCutOff", 15.f);
    lightProgram_->SetUniform<float>("u_spotLight.constant", 1.f);
    lightProgram_->SetUniform<float>("u_spotLight.linear", 0.09f);
    lightProgram_->SetUniform<float>("u_spotLight.quadratic", 0.032f);
    lightProgram_->SetUniform<glm::vec3>("u_spotLight.ambient", glm::vec3(0.1f,0.1f,0.1f));
    lightProgram_->SetUniform<glm::vec3>("u_spotLight.diffuse", glm::vec3(0.2f,0.2f,0.2f));
    lightProgram_->SetUniform<glm::vec3>("u_spotLight.specular", glm::vec3(0.2f,0.2f,0.5f));
    model_->Draw(*lightProgram_);

}