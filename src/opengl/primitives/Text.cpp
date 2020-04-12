#include <iostream>
#include <algorithm>  
// GL
#include <GL/glew.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// FT
#include <ft2build.h>
#include FT_FREETYPE_H  

#include <Shader.h>
#include <Font.h>

#include <ui\primitives\Text.h>

#ifdef _MSC_VER
    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glu32.lib")
#endif

static const char* vertexSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
    out vec2 TexCoords;

    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }  
)glsl";

static const char* fragmentSource = R"glsl(
    #version 330 core
    in vec2 TexCoords;
    out vec4 color;

    uniform sampler2D text;
    uniform vec3 textColor;

    void main() {    
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(textColor, 1.0) * sampled;
    }  
)glsl";

const GLuint WIDTH = 640, HEIGHT = 480;

static GLUtil::Shader shader;

EngFlow::UI::Primitives::Text::Text(std::string text, UIColor color) : text(text), color(color) {}

void EngFlow::UI::Primitives::Text::initBuffers() {
    if (!buffersInitialized) {
        // Configure VAO/VBO for texture quads
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        buffersInitialized = true;
    }
}

void EngFlow::UI::Primitives::Text::draw(UIRect rect){
    GLfloat x = 0;
    GLfloat y = 0;
    if (!shader.isCompiled()) {
        shader.compile(vertexSource, fragmentSource);
    } 
    shader.select();
    initBuffers();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(rect.x, rect.y, rect.w, rect.h);

    // Activate corresponding render state
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(rect.w), 0.0f, static_cast<GLfloat>(rect.h));
    glUniformMatrix4fv(shader.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(shader.getUniform("textColor"), color.r, color.g, color.b);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    Font fontData({ font, fontSize });
    // Iterate through all characters
    for (auto const& c: text) {
        auto ch = fontData[c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

EngFlow::UI::UISize EngFlow::UI::Primitives::Text::textSize() const {
    GLfloat xpos = 0;
    int ypos = 0;
    Font fontData({ font,fontSize });
    for (auto const& c : text) {
        auto ch = fontData[c];
        xpos += (ch.Advance >> 6);
        ypos = std::max(ypos, ch.Size.y);
    }
    xpos *= scale;
    ypos *= scale;
    return { (int)ceil(xpos), ypos};
}
