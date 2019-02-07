#include "SimpleCube.h"

#include "ogl/Shader.h"

static constexpr char* FRAGMENT_SHADER_SRC = R"(
    #version 330 core
    out vec4 color;

    in vec3 v3normal;
    in vec3 v3fragPos;

    uniform vec3 u_lightPos;
    uniform vec3 u_viewPos;
    uniform vec3 u_objectColor;
    uniform vec3 u_lightColor;

    void main()
    {
        // ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * u_lightColor;

        // diffuse
        vec3 norm = normalize(v3normal);
        vec3 lightDir = normalize(u_lightPos - v3fragPos);
        float diff = max(dot(v3normal,lightDir), 0.0);
        vec3 diffuse = diff * u_lightColor;

        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(u_viewPos - v3fragPos);
        vec3 reflectDir = reflect(-u_lightPos, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * u_lightColor;

        vec3 result = (ambient + diffuse + specular) * u_objectColor;
        color = vec4(result, 1.0);
    }
)";

static constexpr char* VERTEX_SHADER_SRC = R"(
    #version 330 core
    // vertex position
    layout (location=0) in vec3 position;
    // 4 component RGBA normalized color
    layout (location=1) in vec4 color;
    // tex coords
    layout (location=2) in vec2 texCoords;
    // normals for lighting
    layout (location=3) in vec3 normal;

    out vec3 v3normal;
    out vec3 v3fragPos;

    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_projection;

    void main()
    {
        gl_Position = u_projection * u_view * u_model * vec4(position, 1.0);
        v3fragPos = vec3(u_model * vec4(position, 1.0));
        v3normal = mat3(transpose(inverse(u_model))) * normal;
    } 
)";

SimpleCube::SimpleCube()
{
    vao_ = new ogl::VertexArray();
    vbo_ = new ogl::VertexBuffer();

    ogl::Vertex vertices[12] = {
        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f,0.f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f,1.f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f,-0.5f, 0.5f}, {255,255,255,255}, {1.f,1.f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {255,255,255,255}, {0.f,0.f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,-0.5f, 0.5f}, {255,255,255,255}, {0.f,1.f}, {0.0f, 0.0f, 1.0f}},
        {{ 0.5f, 0.5f, 0.5f}, {255,255,255,255}, {1.f,0.f}, {0.0f, 0.0f, 1.0f}},

        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f,0.f}, {0.0f, 0.0f,-1.0f}},
        {{-0.5f,-0.5f,-0.5f}, {255,255,255,255}, {0.f,1.f}, {0.0f, 0.0f,-1.0f}},
        {{ 0.5f,-0.5f,-0.5f}, {255,255,255,255}, {1.f,1.f}, {0.0f, 0.0f,-1.0f}},
        {{-0.5f, 0.5f,-0.5f}, {255,255,255,255}, {0.f,0.f}, {0.0f, 0.0f,-1.0f}},
        {{-0.5f,-0.5f,-0.5f}, {255,255,255,255}, {0.f,1.f}, {0.0f, 0.0f,-1.0f}},
        {{ 0.5f, 0.5f,-0.5f}, {255,255,255,255}, {1.f,0.f}, {0.0f, 0.0f,-1.0f}},

    };

    vbo_->SetData(sizeof(ogl::Vertex)*12, vertices, GL_STATIC_DRAW);
    ogl::VertexBufferLayout vbl;
    ogl::Vertex::PushLayout(vbl);
    vao_->AddBuffer(*vbo_, vbl);
    program_ = new ogl::Program();
    ogl::Shader* vertexShader = new ogl::Shader(GL_VERTEX_SHADER, VERTEX_SHADER_SRC);
    ogl::Shader* fragmentShader = new ogl::Shader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_SRC);
    program_->CompileShaders(*vertexShader, *fragmentShader);
    delete vertexShader;
    delete fragmentShader;
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
    glDrawArrays(GL_TRIANGLES, 0, 12);
}