#pragma once

#include "engine/GameObject.h"

class LampObject : public engine::GameObject
{
public:
    LampObject();
    virtual ~LampObject();

    void Update(float dtime) override;
    void Render(ogl::Program& program) override;

    static constexpr size_t NUM_VERTICES = 8;
private:
    ogl::Vertex         vertices_[NUM_VERTICES];
    ogl::ElementBuffer* elementBuffer_;
    ogl::VertexArray*   vertexArray_;
    ogl::VertexBuffer*  vertexBuffer_;
};