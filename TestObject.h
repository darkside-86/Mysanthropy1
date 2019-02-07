#pragma once

#include "engine/GameObject.h"
#include "ogl/ElementBuffer.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"

class TestObject : public engine::GameObject
{
public:
    TestObject(ogl::Texture* t);
    virtual ~TestObject();

    void Update(float dtime) override;
    void Render(engine::GraphicsContext& gc) override;

    ogl::Vertex* GetVertices() { return vertices_; }
    void SetVerticesData() 
        { vertexBuffer_->SetData((sizeof(ogl::Vertex)*NUM_VERTICES), vertices_, GL_STATIC_DRAW);}
    static constexpr size_t NUM_VERTICES = 8;
private:
    ogl::Vertex vertices_[NUM_VERTICES];
    ogl::ElementBuffer* elementBuffer_;
    ogl::VertexArray* vertexArray_;
    ogl::VertexBuffer* vertexBuffer_;
    ogl::Texture* texture_;
};