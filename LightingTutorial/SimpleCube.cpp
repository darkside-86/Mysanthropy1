#include "SimpleCube.h"

#include <fstream>
#include <string>

#include "engine/GameEngine.h"
#include "ogl/ErrorCheck.h"
#include "ogl/Shader.h"

SimpleCube::SimpleCube()
{
    vao_ = new ogl::VertexArray();
    vbo_ = new ogl::VertexBuffer();

    ogl::Vertex vertices[30] = {
        // {{-0.5f, 0.5f, -0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, -1.0f}} // TOP LEFT BACK
        // {{-0.5f,-0.5f, -0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f, -1.0f}} // BTM LEFT BACK
        // {{ 0.5f,-0.5f, -0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, -1.0f}} // BTM RGHT BACK
        // {{ 0.5f, 0.5f, -0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f, -1.0f}} // TOP RGHT BACK
        
        // {{-0.5f, 0.5f,  0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, -1.0f}} // TOP LEFT FRNT
        // {{-0.5f,-0.5f,  0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f, -1.0f}} // BTM LEFT FRNT
        // {{ 0.5f,-0.5f,  0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, -1.0f}} // BTM RGHT FRNT
        // {{ 0.5f, 0.5f,  0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f, -1.0f}} // TOP RGHT FRNT
        
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, -1.0f}}, // TOP LEFT BACK
        {{-0.5f,-0.5f,-0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f, -1.0f}}, // BTM LEFT BACK
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, -1.0f}}, // BTM RGHT BACK
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f, -1.0f}}, // TOP LEFT BACK
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f, -1.0f}}, // BTM RGHT BACK
        {{ 0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f, -1.0f}}, // TOP RGHT BACK

        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f,  1.0f}}, // TOP LEFT FRNT
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 0.f,  1.0f}}, // BTM LEFT FRNT
        {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f,  1.0f}}, // BTM RGHT FRNT
        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 0.f,  1.0f}}, // TOP LEFT FRNT
        {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 0.f,  1.0f}}, // BTM RGHT FRNT
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 0.f,  1.0f}}, // TOP RGHT FRNT

        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 1.f, 0.0f}}, // TOP LEFT BACK
        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {0.f, 1.f, 0.0f}}, // TOP LEFT FRNT
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 1.f, 0.0f}}, // TOP RGHT FRNT
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f, 0.f}, {0.f, 1.f, 0.0f}}, // TOP LEFT BACK
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {1.f, 1.f}, {0.f, 1.f, 0.0f}}, // TOP RGHT FRNT
        {{ 0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {0.f, 1.f, 0.0f}}, // TOP RGHT BACK

        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {1.f, 0.f, 0.0f}}, // TOP RGHT FRNT
        {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {1.f, 0.f, 0.0f}}, // BTM RGHT FRNT
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {1.f, 0.f, 0.0f}}, // BTM RGHT BACK
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {1.f, 0.f, 0.0f}}, // TOP RGHT FRNT
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {1.f, 0.f, 0.0f}}, // BTM RGHT BACK
        {{ 0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {1.f, 0.f, 0.0f}}, // TOP RGHT BACK

        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f, 0.f}, {-1.f, 0.f, 0.0f}}, // TOP LEFT FRNT
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {-1.f, 0.f, 0.0f}}, // BTM LEFT FRNT
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {-1.f, 0.f, 0.0f}}, // TOP LEFT BACK
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f, 1.f}, {-1.f, 0.f, 0.0f}}, // BTM LEFT FRNT
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f, 0.f}, {-1.f, 0.f, 0.0f}}, // TOP LEFT BACK
        {{-0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f, 1.f}, {-1.f, 0.f, 0.0f}}, // BTM LEFT BACK

    };

    vbo_->SetData(sizeof(ogl::Vertex)*30, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_->AddBuffer(*vbo_, vbl);   
}

SimpleCube::~SimpleCube()
{
    delete vao_;
    delete vbo_;
}

void SimpleCube::Update(float dtime)
{

}

void SimpleCube::Render(engine::GraphicsContext& gc)
{
    vbo_->Bind();
    vao_->Bind();
    //program_->Use();
    glDrawArrays(GL_TRIANGLES, 0, 30);
}