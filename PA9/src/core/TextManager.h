#ifndef __TEXTMANAGER_H
#define __TEXTMANAGER_H

#include "GlobalIncludes.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "VBO.h"
#include "VAO.h"

struct Character
{
    Texture texture;   // Texture for the glyph
    glm::ivec2 size;    // Size of the glyph
    glm::ivec2 bearing; // Offset from baseline to the top-left of the glyph
    GLuint    advance;  // Offset to advance to the next glyph from this glyph
};

class TextManager
{
    public:
        TextManager(std::string defaultFont = "Fonts/FreeSans.ttf", int defaultFontSize = 18);
        ~TextManager();

        void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    private:
        FT_Library fontEngine;
        FT_Face    currentFont;
        bool isInitialized;
        bool fontLoaded;
        ShaderProgram shaderProgram;
        VAO textVAO;
        VBO textVBO;

        std::map<GLchar, Character> characters;

};

#endif
