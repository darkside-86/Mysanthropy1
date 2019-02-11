#pragma once

#include "engine/Camera.h"
#include "engine/Game.h"
#include "LightingTutorial/SimpleCube.h"
#include "ogl/Program.h"
#include "SimplePlane.h"

class AdvancedTutorial : public engine::Game 
{
public:
    AdvancedTutorial();
    virtual ~AdvancedTutorial();
    bool Initialize() override;
    void Cleanup() override;
    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;
private:
    SimpleCube* simpleCube_;
    ogl::Program program_;
    ogl::Program colorProgram_;
    engine::Camera camera_;
    ogl::Texture* cubeTexture_;
    SimplePlane* simplePlane_;
};