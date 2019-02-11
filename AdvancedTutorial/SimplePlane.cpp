#include "SimplePlane.h"

#include "engine/GameEngine.h"
#include "ogl/Vertex.h"

SimplePlane::SimplePlane()
{
    vao_ = new ogl::VertexArray();
    vbo_ = new ogl::VertexBuffer();

    ogl::Vertex vertices[4] = {
        {{-10.f,0.f,-10.f},{255,255,255,255},{0.f,0.f},{0.f,1.f,0.f}},
        {{-10.f,0.f, 10.f},{255,255,255,255},{0.f,1.f},{0.f,1.f,0.f}},
        {{ 10.f,0.f,-10.f},{255,255,255,255},{1.f,0.f},{0.f,1.f,0.f}},
        {{ 10.f,0.f, 10.f},{255,255,255,255},{1.f,1.f},{0.f,1.f,0.f}},
    };
    vbo_->SetData(sizeof(ogl::Vertex)*4, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_->AddBuffer(*vbo_, vbl);

    engine::GameEngine::Get().GetTextureManager().LoadTexture("ground", "res/textures/ground.jpg");
    groundTexture_ = engine::GameEngine::Get().GetTextureManager().GetTexture("ground");
}

SimplePlane::~SimplePlane()
{
    delete vao_;
    delete vbo_;
    engine::GameEngine::Get().GetTextureManager().UnloadTexture("ground");
}

void SimplePlane::Render()
{
    vao_->Bind();
    groundTexture_->Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}