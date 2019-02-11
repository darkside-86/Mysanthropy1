#include "LightingTutorial.h"

#include <sstream>
#include <string>

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
                camera_->MoveSide(-0.5f);
                break;
            case SDLK_d:
                camera_->MoveSide(0.5f);
                break;
            case SDLK_w:
                camera_->MoveFront(0.5f);
                break;
            case SDLK_s:
                camera_->MoveFront(-0.5f);
                break;
            case SDLK_SPACE:
                camera_->MoveUp(0.5f);
                break;
            case SDLK_LSHIFT:
                camera_->MoveUp(-0.5f);
                break;
            case SDLK_LEFT:
                // camera_->SetYaw(camera_->GetYaw() + 15.f);
                break;
            case SDLK_RIGHT:
                // camera_->SetYaw(camera_->GetYaw() - 15.f);
                break;
            case SDLK_UP:
                // camera_->SetPitch(camera_->GetPitch() + 15.f);
                break;
            case SDLK_DOWN:
                // camera_->SetPitch(camera_->GetPitch() - 15.f);
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
    const size_t NUM_POINTLIGHTS = 4;
    glm::vec3 pointLightPositions[NUM_POINTLIGHTS] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    glClearColor(0.1f, 0.f, 0.f, 1.f);
    ogl::Program& lampShader = gc.GetProgram();
    ogl::Program* cubeShader = simpleCube_->GetProgram();
    glm::mat4 view(1.0f), projection(1.0f);
    // view = glm::lookAt(glm::vec3(xPos_,yPos_,5.f+zPos_), glm::vec3(xPos_,yPos_,4.f+zPos_), glm::vec3(0.f,1.f,0.f));
    // camera_->SetDirection({0.f,0.f,-1.f});
    // camera_->SetPosition({xPos_, yPos_, zPos_});
    view = camera_->CalculateView();
    projection = glm::perspective(glm::radians(45.f), width/height, 0.01f, 100.f);
    cubeShader->Use();
    // mvp
    cubeShader->SetUniform<glm::mat4>("u_view", view);
    cubeShader->SetUniform<glm::mat4>("u_projection", projection);
    // viewpos
    cubeShader->SetUniform<glm::vec3>("u_viewPos", glm::vec3(xPos_,yPos_,5.f+zPos_));
    // material diffuse, specular, and shininess
    cubeShader->SetUniform<int>("u_material.diffuse", 0);
    cubeShader->SetUniform<int>("u_material.specular", 1);
    cubeShader->SetUniform<float>("u_material.shininess", 32.f);
    // directional light attributes
    cubeShader->SetUniform<glm::vec3>("u_dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    cubeShader->SetUniform<glm::vec3>("u_dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
    cubeShader->SetUniform<glm::vec3>("u_dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
    cubeShader->SetUniform<glm::vec3>("u_dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    // point lights
    for(int i=0; i < NUM_POINTLIGHTS; ++i)
    {
        std::stringstream ss;
        ss << "u_pointLights[" << i << "]";
        std::string uniformName = ss.str();
        cubeShader->SetUniform<glm::vec3>(uniformName + ".position", pointLightPositions[i]);
        cubeShader->SetUniform<glm::vec3>(uniformName + ".ambient", glm::vec3(0.25f, 0.05f, 0.05f));
        cubeShader->SetUniform<glm::vec3>(uniformName + ".diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        cubeShader->SetUniform<glm::vec3>(uniformName + ".specular", glm::vec3(1.0f, 1.0f, 1.0f));
        cubeShader->SetUniform<float>(uniformName + ".constant", 1.0f);
        cubeShader->SetUniform<float>(uniformName + ".linear", 0.09f);
        cubeShader->SetUniform<float>(uniformName + ".quadratic", 0.032f);
    }
    // spot light
    cubeShader->SetUniform<glm::vec3>("u_spotLight.position", camera_->GetPosition());
    cubeShader->SetUniform<glm::vec3>("u_spotLight.direction", camera_->GetDirection());
    cubeShader->SetUniform<glm::vec3>("u_spotLight.ambient", glm::vec3(0.f, 0.f, 0.f));
    cubeShader->SetUniform<glm::vec3>("u_spotLight.diffuse", glm::vec3(0.f, 0.f, 1.f));
    cubeShader->SetUniform<glm::vec3>("u_spotLight.specular", glm::vec3(0.f, 0.f, 1.f));
    cubeShader->SetUniform<float>("u_spotLight.constant", 1.0f);
    cubeShader->SetUniform<float>("u_spotLight.linear", 0.09f);
    cubeShader->SetUniform<float>("u_spotLight.quadratic", 0.032f);
    cubeShader->SetUniform<float>("u_spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    cubeShader->SetUniform<float>("u_spotLight.outerCutOff", glm::cos(glm::radians(15.f)));
    texture_->Bind();
    specular_->Bind(1);
    for(int i=0; i < NUM_POSITIONS; ++i)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.f), cubePositions[i]);
        float angle = 20.f * (float)i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f,0.3f,0.5f));
        cubeShader->SetUniform<glm::mat4>("u_model", model);
        simpleCube_->Render(*cubeShader);
    }

    lampShader.Use();
    lampShader.SetUniform<glm::mat4>("u_view", view);
    lampShader.SetUniform<glm::mat4>("u_projection", projection);
    for(int i=0; i < NUM_POINTLIGHTS; ++i)
    {
        glm::mat4 model = glm::translate(glm::mat4(1.f), pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.4f,0.4f,0.4f));
        lampShader.SetUniform<glm::mat4>("u_model", model);
        lampObject_->Render(lampShader);
    }
}

void LightingTutorial::Cleanup()
{

}
