#include "LightingTutorial.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"

LightingTutorial::LightingTutorial()
{
    lampObject_ = new LampObject();
    simpleCube_ = new SimpleCube();
    camera_ = new engine::Camera();
}
    
LightingTutorial::~LightingTutorial()
{
    delete lampObject_;
    delete simpleCube_;
    delete camera_;
}
    
bool LightingTutorial::Initialize()
{
    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
        switch(e.keysym.sym)
        {
        case SDLK_a:
            xPos_ -= 0.5f;
            break;
        case SDLK_d:
            xPos_ += 0.5f;
            break;
        case SDLK_w:
            zPos_ += 0.5f;
            break;
        case SDLK_s:
            zPos_ -= 0.5f;
            break;
        }
    });
    return true;
}
    
void LightingTutorial::Update(float dtime)
{

}
    
void LightingTutorial::Render(engine::GraphicsContext& gc)
{
    float width = (float)engine::GameEngine::Get().GetWidth();
    float height = (float)engine::GameEngine::Get().GetHeight();
    // render lamp with stock program
    gc.ResetMVP();
    gc.SetPerspectiveProjection(45.f, width/height, 0.01f, 45.f);
    gc.PushModel();
    gc.TranslateModel(1.f+xPos_,1.f,-5.f+zPos_);
    // glm::mat4 v = glm::lookAt(glm::vec3(0.f,0.f,-5.f), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
    // gc.SetView(v);
    gc.SetMVP();
    lampObject_->Render(gc);
    gc.PopModel();
    ogl::Program* program = simpleCube_->GetProgram();
    program->Use();
    glm::mat4 model(1.f), view(1.f), projection(1.f);
    projection = glm::perspective(glm::radians(45.f), width/height, 0.01f, 45.f);
    model = glm::translate(model, glm::vec3(0.f,0.f,-6.f));
    view = glm::lookAt(glm::vec3(xPos_,0.f,zPos_), glm::vec3(0.f,0.f,0.f), glm::vec3(0.f,1.f,0.f));
    program->SetUniform<glm::mat4>("u_model", model);
    program->SetUniform<glm::mat4>("u_view", view);
    program->SetUniform<glm::mat4>("u_projection", projection);

    program->SetUniform<glm::vec3>("u_lightColor", glm::vec3(1.f,1.f,1.f));
    program->SetUniform<glm::vec3>("u_lightPos", glm::vec3(1.f,1.f,-5.f));
    program->SetUniform<glm::vec3>("u_viewPos", glm::vec3(0.f,0.f,1.f));
    program->SetUniform<glm::vec3>("u_objectColor", glm::vec3(1.f,1.f,0.f));
    simpleCube_->Render(gc);
}

void LightingTutorial::Cleanup()
{

}
