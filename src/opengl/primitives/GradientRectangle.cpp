#include <GL/glew.h>

#include <Shader.h>

#include <ui/primitives/GradientRectangle.h>

static const char* vertexSource = R"glsl(
    #version 330 core
    uniform uint xy;
    uniform vec3 c1;
    uniform vec3 c2;
    in vec2 position;
    varying highp vec4 colorGradient;
    void main(){
        colorGradient = vec4(mix(c2,c1,(position[xy]+1)/2), 1.0f); 
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

static const char* fragmentSource = R"glsl(
    #version 330 core
    varying highp vec4 colorGradient;
    out vec4 outColor;
    void main(){
        outColor = colorGradient;
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
static GLint c1, c2, xy;
static bool compiled;

static GLUtil::Shader shader;

static void compileShader() {
    shader.compile(vertexSource, fragmentSource);

    posAttrib = shader.getAttrib("position");
    c1 = shader.getUniform("c1");
    c2 = shader.getUniform("c2");
    xy = shader.getUniform("xy");
    //Create vertex and element buffers to draw rectangle
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void EngFlow::UI::Primitives::GradientRectangle::draw(UIRect rect){
    if (!shader.isCompiled()) compileShader();

    shader.select();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glUniform3f(c1, color1.r, color1.g, color1.b);
    glUniform3f(c2, color2.r, color2.g, color2.b);
    glUniform1ui(xy, vert);
    glViewport(rect.x, rect.y, rect.w, rect.h);
    glDrawElements(GL_TRIANGLES, (GLsizei)std::size(elements), GL_UNSIGNED_INT, 0);
}
