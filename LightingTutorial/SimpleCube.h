#pragma once

#include "engine/GameObject.h"
#include "ogl/Vertex.h"
#include "ogl/Program.h"

class SimpleCube : public engine::GameObject
{
public:
    SimpleCube();
    virtual ~SimpleCube();

    void Update(float dtime) override;
    void Render(ogl::Program& program) override;
    ogl::Program* GetProgram() { return program_; }
private:
    ogl::VertexArray*   vao_;
    ogl::VertexBuffer*  vbo_;
    ogl::Program*       program_;
};