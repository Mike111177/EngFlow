#include <iostream>

#include <Font.h>

#include <ft2build.h>
#include FT_FREETYPE_H  

#ifdef _MSC_VER
	#pragma comment(lib, "opengl32.lib")
	#pragma comment(lib, "glu32.lib")
	#pragma comment(lib, "freetype.lib")
#endif

std::unordered_map<FontDescriptor, std::shared_ptr<Font::FontStore>> Font::loadedFonts;

Font::FontStore::FontStore(FontDescriptor fd) {
    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    // Load font as face
    FT_Face face;
    std::string path("C:/Windows/Fonts/" + fd.fontname + ".ttf");
    std::cout << "Loading Font: " << path << std::endl;
    if (FT_New_Face(ft, path.c_str(), 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, fd.rSize);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++) {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

std::shared_ptr<Font::FontStore> Font::getFont(FontDescriptor fd) {
    std::unordered_map<FontDescriptor, std::shared_ptr<FontStore>>::iterator it;
    if ((it = loadedFonts.find(fd)) == loadedFonts.end()) {
        auto font = std::make_shared<FontStore>(fd);
        loadedFonts.insert({ fd, font });
        return font;
    }
    else {
        return std::get<1>(*it);
    }
}

Font::Font(FontDescriptor fd) : font_store(getFont(fd)) {}
Font::Character Font::operator[](char c) {
    return font_store->Characters[c];
}
