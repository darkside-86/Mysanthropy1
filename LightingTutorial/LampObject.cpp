#include "LampObject.h"

#include "ogl/Vertex.h"

LampObject::LampObject()
{
    vertexBuffer_ = new ogl::VertexBuffer();
    vertexArray_ = new ogl::VertexArray();
    elementBuffer_ = new ogl::ElementBuffer();

    vertices_[0] = {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f,0.f}};
    vertices_[1] = {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f,1.f}};
    vertices_[2] = {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {1.f,1.f}};
    vertices_[3] = {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {1.f,0.f}};

    vertices_[4] = {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f,0.f}};
    vertices_[5] = {{-0.5f,-0.5f,-0.5f}, {255,255,255,255}, {0.f,1.f}};
    vertices_[6] = {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f,1.f}};
    vertices_[7] = {{ 0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f,0.f}};

    unsigned int indices[24] = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        0, 4, 7, 0, 3, 7,
        3, 7, 6, 3, 6, 2,
    };

    vertexBuffer_->SetData(sizeof(ogl::Vertex)*NUM_VERTICES, vertices_, GL_STATIC_DRAW);
    elementBuffer_->SetData(sizeof(unsigned int) * 24, indices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vertexArray_->AddBuffer(*vertexBuffer_, vbl);
}

LampObject::~LampObject()
{

}

void LampObject::Update(float dtime)
{

}

void LampObject::Render(ogl::Program& program)
{
    vertexBuffer_->Bind();
    vertexArray_->Bind();
    elementBuffer_->Bind();
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);
}