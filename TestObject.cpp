#include "TestObject.h"

#include "ogl/Vertex.h"

TestObject::TestObject(ogl::Texture* t) : texture_(t)
{
    vertexBuffer_ = new ogl::VertexBuffer();
    vertexArray_ = new ogl::VertexArray();
    elementBuffer_ = new ogl::ElementBuffer();

    vertices_[0] = {{-0.5f, 0.5f, 0.5f}, {255,0,0,255}, {0.f,0.f}};
    vertices_[1] = {{-0.5f,-0.5f, 0.5f}, {255,0,0,255}, {0.f,1.f}};
    vertices_[2] = {{ 0.5f,-0.5f, 0.5f}, {255,0,0,255}, {1.f,1.f}};
    vertices_[3] = {{ 0.5f, 0.5f, 0.5f}, {255,0,255,255}, {1.f,0.f}};

    vertices_[4] = {{-0.5f, 0.5f,-0.5f}, {255,0,0,255}, {0.f,0.f}};
    vertices_[5] = {{-0.5f,-0.5f,-0.5f}, {255,255,0,255}, {0.f,1.f}};
    vertices_[6] = {{ 0.5f,-0.5f,-0.5f}, {255,0,0,255}, {1.f,1.f}};
    vertices_[7] = {{ 0.5f, 0.5f,-0.5f}, {255,0,255,255}, {1.f,0.f}};

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

TestObject::~TestObject()
{
    delete vertexBuffer_;
    delete vertexArray_;
    delete elementBuffer_;
}

void TestObject::Update(float dtime)
{

}
    
void TestObject::Render(ogl::Program& program)
{
    vertexArray_->Bind();
    vertexBuffer_->Bind();
    elementBuffer_->Bind();
    if(texture_) texture_->Bind();
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);
}