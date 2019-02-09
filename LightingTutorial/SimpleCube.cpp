#include "SimpleCube.h"

#include <fstream>
#include <string>

#include "engine/GameEngine.h"
#include "ogl/Shader.h"

SimpleCube::SimpleCube()
{
    vao_ = new ogl::VertexArray();
    vbo_ = new ogl::VertexBuffer();

    ogl::Vertex vertices[24] = {
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
    };

    vbo_->SetData(sizeof(ogl::Vertex)*24, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_->AddBuffer(*vbo_, vbl);
    program_ = new ogl::Program();
    // ogl::Shader* vertexShader = new ogl::Shader(GL_VERTEX_SHADER, VERTEX_SHADER_SRC);
    // ogl::Shader* fragmentShader = new ogl::Shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
    std::ifstream vsFile, fsFile;
    std::string str, vsSrc, fsSrc;
    vsFile.open("res/shaders/lighting.vs");
    fsFile.open("res/shaders/lighting.frag");
    if(!vsFile.is_open() || !fsFile.is_open())
    {
        engine::GameEngine::Get().GetLogger().Logf(engine::Logger::Severity::WARNING, 
                "%s: Failed to open shader files for lighting.");
    }
    while(std::getline(vsFile,str))
        vsSrc += str + "\n";
    while(std::getline(fsFile, str))
        fsSrc += str + "\n";
    vsFile.close();
    fsFile.close();
    ogl::Shader vertexShader(GL_VERTEX_SHADER, vsSrc.c_str());
    ogl::Shader fragmentShader(GL_FRAGMENT_SHADER, fsSrc.c_str());

    program_->CompileShaders(vertexShader, fragmentShader);
    
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
    program_->Use();
    glDrawArrays(GL_TRIANGLES, 0, 24);
}