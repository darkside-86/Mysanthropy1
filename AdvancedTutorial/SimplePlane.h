#pragma once

#include "ogl/Texture.h"
#include "ogl/VertexArray.h"
#include "ogl/VertexBuffer.h"

class SimplePlane
{
public:
    SimplePlane();
    virtual ~SimplePlane();
    void Render();
private:
    ogl::VertexArray* vao_;
    ogl::VertexBuffer* vbo_;
    ogl::Texture* groundTexture_;
};