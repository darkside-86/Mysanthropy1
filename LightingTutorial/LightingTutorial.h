#pragma once

#include "engine/Game.h"

#include "engine/Camera.h"
#include "ogl/Program.h"
#include "LampObject.h"
#include "SimpleCube.h"

class LightingTutorial : public engine::Game 
{
public:
    LightingTutorial();
    virtual ~LightingTutorial();

    bool Initialize() override;
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
    void Cleanup() override;
private:
    LampObject*     lampObject_;
    SimpleCube*     simpleCube_;
    engine::Camera* camera_;
    float xPos_=0.0f, zPos_=-10.0f;
};