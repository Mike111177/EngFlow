#include <GL/glew.h>

#include <Shader.h>

#include <ui/primitives/SolidRectangle.h>

static const char* vertexSource = R"glsl(
    #version 330 core
    in vec2 position;
    void main(){
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

static const char* fragmentSource = R"glsl(
    #version 330 core
    uniform vec3 boxColor;
    out vec4 outColor;
    void main(){
        outColor = vec4(boxColor, 1.0);
    }
)glsl";

static const GLfloat vertices[8] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f,
    -1.0f,  1.0f
};
static const GLuint elements[6] = {
    0, 1, 2,
    2, 3, 0
};

static GLuint vbo = 0;
static GLuint ebo = 0;
static GLuint posAttrib;
static GLint uniColor;

static GLUtil::Shader shader;

static void compileShader() {
    shader.compile(vertexSource, fragmentSource);

    posAttrib = shader.getAttrib("position");
    uniColor = shader.getUniform("boxColor");
    //Create vertex and element buffers to draw rectangle
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void EngFlow::UI::Primitives::SolidRectangle::draw(){
    if (!shader.isCompiled()) compileShader();

    shader.select();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glUniform3f(uniColor, color.r, color.g, color.b);
    glViewport(x, y, w, h);
    glDrawElements(GL_TRIANGLES, (GLsizei)std::size(elements), GL_UNSIGNED_INT, 0);
}
