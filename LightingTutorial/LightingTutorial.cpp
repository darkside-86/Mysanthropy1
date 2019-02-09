#include "LightingTutorial.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "engine/GameEngine.h"

LightingTutorial::LightingTutorial()
{
    lampObject_ = new LampObject();
    simpleCube_ = new SimpleCube();
    camera_ = new engine::Camera();
    texture_ = new ogl::Texture("res/textures/container.png", true);
    specular_ = new ogl::Texture("res/textures/specular.png", true);
}
    
LightingTutorial::~LightingTutorial()
{
    delete lampObject_;
    delete simpleCube_;
    delete camera_;
    delete texture_;
    delete specular_;
}
    
bool LightingTutorial::Initialize()
{
    engine::GameEngine::Get().AddKeyboardListener([this](const SDL_KeyboardEvent& e){
        if(e.type == SDL_KEYDOWN)
        {
            switch(e.keysym.sym)
            {
            case SDLK_a:
                xPos_ -= 0.5f;
                break;
            case SDLK_d:
                xPos_ += 0.5f;
                break;
            case SDLK_w:
                zPos_ -= 0.5f;
                break;
            case SDLK_s:
                zPos_ += 0.5f;
                break;
            case SDLK_SPACE:
                yPos_ += 0.5f;
                break;
            case SDLK_LSHIFT:
                yPos_ -= 0.5f;
                break;
            case SDLK_LEFT:
                glm::vec3 dir = camera_->GetDirection();

                camera_->SetDirection(dir);
                break;
            }
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
    const size_t NUM_POSITIONS = 10;
    glm::vec3 cubePositions[NUM_POSITIONS] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    glm::vec3 lampPos = {0.2f, 1.0f, 0.3f};
    glClearColor(0.1f, 0.f, 0.f, 1.f);
    ogl::Program& lampShader = gc.GetProgram();
    ogl::Program* cubeShader = simpleCube_->GetProgram();
    glm::mat4 model(1.0f), view(1.0f), projection(1.0f);
    model = glm::translate(model, glm::vec3(0.f,0.f,0.f));
    // view = glm::lookAt(glm::vec3(xPos_,yPos_,5.f+zPos_), glm::vec3(xPos_,yPos_,4.f+zPos_), glm::vec3(0.f,1.f,0.f));
    camera_->SetDirection({0.f,0.f,-1.f});
    camera_->SetPosition({xPos_, yPos_, zPos_});
    view = camera_->CalculateView();
    projection = glm::perspective(glm::radians(45.f), width/height, 0.01f, 45.f);
    cubeShader->Use();
    // mvp
    cubeShader->SetUniform<glm::mat4>("u_model", model);
    cubeShader->SetUniform<glm::mat4>("u_view", view);
    cubeShader->SetUniform<glm::mat4>("u_projection", projection);
    // viewpos
    cubeShader->SetUniform<glm::vec3>("u_viewPos", glm::vec3(xPos_,yPos_,5.f+zPos_));
    // material diffuse, specular, and shininess
    cubeShader->SetUniform<int>("u_material.diffuse", 0);
    cubeShader->SetUniform<int>("u_material.specular", 1);
    cubeShader->SetUniform<float>("u_material.shininess", 32.f);
    // light attributes
    cubeShader->SetUniform<glm::vec3>("u_light.position", camera_->GetPosition());
    cubeShader->SetUniform<glm::vec3>("u_light.direction", camera_->GetDirection());
    cubeShader->SetUniform<glm::vec3>("u_light.ambient", glm::vec3(0.2f,0.2f,0.2f));
    cubeShader->SetUniform<glm::vec3>("u_light.diffuse", glm::vec3(0.5f,0.5f,0.5f));
    cubeShader->SetUniform<glm::vec3>("u_light.specular", glm::vec3(1.f,1.f,1.f));
    cubeShader->SetUniform<float>("u_light.constant", 1.0f);
    cubeShader->SetUniform<float>("u_light.linear", 0.09f);
    cubeShader->SetUniform<float>("u_light.quadratic", 0.032f);
    cubeShader->SetUniform<float>("u_light.cutOff", glm::cos(glm::radians(12.5f)));
    cubeShader->SetUniform<float>("u_light.outerCutOff", glm::cos(glm::radians(17.5f)));
    texture_->Bind();
    specular_->Bind(1);
    for(int i=0; i < NUM_POSITIONS; ++i)
    {
        model = glm::translate(glm::mat4(1.f), cubePositions[i]);
        float angle = 20.f * (float)i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f,0.3f,0.5f));
        cubeShader->SetUniform<glm::mat4>("u_model", model);
        simpleCube_->Render(gc);
    }
    // simpleCube_->Render(gc);
    model = glm::translate(glm::mat4(1.f), lampPos);
    lampShader.Use();
    lampShader.SetUniform<glm::mat4>("u_model", model);
    lampShader.SetUniform<glm::mat4>("u_view", view);
    lampShader.SetUniform<glm::mat4>("u_projection", projection);
    // lampObject_->Render(gc);
}

void LightingTutorial::Cleanup()
{

}
