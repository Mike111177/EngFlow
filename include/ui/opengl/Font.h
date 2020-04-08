#pragma once
#include <memory>
#include <map>
#include <unordered_map>
// GL
#include <GL/glew.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ui/opengl/Shader.h>

#include <ui/FontDescriptor.h>

class Font {
public:
    struct Character;
    struct FontStore {
        std::map<GLchar, Character> Characters;
        FontStore(FontDescriptor fd);
    };
    static std::unordered_map<FontDescriptor, std::shared_ptr<FontStore>> loadedFonts;
    static std::shared_ptr<FontStore> getFont(FontDescriptor fd);
    std::shared_ptr<FontStore> font_store;
public:
    struct Character {
        GLuint     TextureID;  // ID handle of the glyph texture
        glm::ivec2 Size;       // Size of glyph
        glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
        GLuint     Advance;    // Offset to advance to next glyph
    };
    Font(FontDescriptor fd);
    Character operator[](char c);
};