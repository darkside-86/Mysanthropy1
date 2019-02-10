#pragma once

#include "engine/Game.h"
#include "engine/Camera.h"
#include "../LightingTutorial/LampObject.h"

#include "ogl/Program.h"
#include "engine/Model.h"

using engine::Model;

class ModelTutorial : public engine::Game 
{
public:
    ModelTutorial();
    virtual ~ModelTutorial();
    bool Initialize() override;
    void Cleanup() override;
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
private:
    engine::Camera* camera_;
    LampObject*     lampObject_;
    ogl::Program*   modelProgram_;
    ogl::Program*   lightProgram_;
    Model*          model_;
};