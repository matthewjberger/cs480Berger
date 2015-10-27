#include "TextManager.h"
#include "Game.h"

using namespace std;

TextManager::TextManager(std::string defaultFont, int defaultFontSize)
{
    isInitialized = true;
    fontLoaded = true;

    // Initialize freetype
    if(FT_Init_FreeType(&fontEngine))
    {
        std::cerr << "Error! Could not initialize FreeType Library." << std::endl;
        isInitialized = false;
        fontLoaded = false;
    }

    // Attempt to load default font
    if(FT_New_Face(fontEngine, defaultFont.c_str(), 0, &currentFont))
    {
        std::cerr << "Error! Could not load default font: '" << defaultFont << "' for TextManager." << std::endl;
        fontLoaded = false;
        isInitialized = false;
        return;
    }
    else
    {
        // Set default font size
        FT_Set_Pixel_Sizes(currentFont, 0, defaultFontSize);
    }

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(GLubyte c = 0; c < 128; c++)
    {
        // Load the glyph
        if(FT_Load_Char(currentFont, c, FT_LOAD_RENDER))
        {
            std::cerr << "Error! Failed to load glyph." << std::endl;
            continue;
        }

        // Extract data from glyph
        int width    = currentFont->glyph->bitmap.width;
        int height   = currentFont->glyph->bitmap.rows;
        int left     = currentFont->glyph->bitmap_left;
        int top      = currentFont->glyph->bitmap_top;
        int advance  = currentFont->glyph->advance.x;
        GLbyte* data = (GLbyte*)currentFont->glyph->bitmap.buffer;


        // Generate texture from extracted data
        Texture texture;
        texture.CreateFromData(data, width, height, GL_RED);
        texture.SetWrap();

        // Create glyph data struct
        Character character =
        {
            texture,
            glm::ivec2(width, height),
            glm::ivec2(left, top),
            advance
        };

        // Map character to glyph data struct
        characters.insert(std::pair<GLchar, Character>(c, character));

    }

    // Setup shader
    shaderProgram.CreateProgram();
    shaderProgram.AddShaderFromFile("Shaders/textVert.glsl", GL_VERTEX_SHADER);
    shaderProgram.AddShaderFromFile("Shaders/textFrag.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.LinkProgram();

    textVAO.Create();
    textVBO.Create(sizeof(GLfloat) * 24);

    textVAO.Bind();
    textVBO.Bind();

    textVBO.UploadData(GL_DYNAMIC_DRAW);

    textVAO.EnableAttribute(0);
    textVAO.ConfigureAttribute(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    textVAO.Unbind();

    glm::mat4 projectionMatrix = glm::ortho(0.0f, float(Game::GetInstance()->GetScreenWidth()), 0.0f, float(Game::GetInstance()->GetScreenHeight()));
    glm::mat4 viewMatrix = glm::mat4(1.0f);

    shaderProgram.UseProgram();
    shaderProgram.SetUniform("projectionMatrix", projectionMatrix);
    shaderProgram.SetUniform("viewMatrix", viewMatrix);
    shaderProgram.DisUseProgram();

}

TextManager::~TextManager()
{
    FT_Done_Face(currentFont);
    FT_Done_FreeType(fontEngine);

    shaderProgram.DeleteProgram();
    textVAO.Free();
    textVBO.Free();

    delete currentFont;
    delete fontEngine;

    currentFont = NULL;
    fontEngine  = NULL;
}

void TextManager::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shaderProgram.UseProgram();
    shaderProgram.SetUniform("textColor", glm::vec3(color.x, color.y, color.z));
    textVAO.Bind();

    std::string::const_iterator c;
    for(c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        GLfloat xPos   = x + ch.bearing.x * scale;
        GLfloat yPos   = y - (ch.size.y - ch.bearing.y) * scale;

        GLfloat width  = ch.size.x * scale;
        GLfloat height = ch.size.y * scale;

        GLfloat vertices[6][4] =
        {
            {xPos,         yPos + height, 0.0, 0.0},
            {xPos,         yPos,          0.0, 1.0},
            {xPos + width, yPos,          1.0, 0.0},

            {xPos,         yPos + height, 0.0, 0.0},
            {xPos + width, yPos,          1.0, 1.0},
            {xPos + width, yPos + height, 1.0, 0.0}
        };

        ch.texture.Bind();
        textVBO.UpdateData(0, sizeof(vertices), vertices);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        x += (ch.advance >> 6) * scale; // 2^6 = 64 pixels
    }
    textVAO.Unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
    shaderProgram.DisUseProgram();
    glDisable(GL_BLEND);
}

