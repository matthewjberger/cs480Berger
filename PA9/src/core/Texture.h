#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "GlobalIncludes.h"
#include <string>

class Texture
{
    public:

        Texture();
        ~Texture();

        void CreateFromData(GLvoid* data, int width, int height, GLenum format, bool genMipMaps = false);

        void Load(std::string path, bool genMipMaps = false);
        void Bind(int textureUnit = 0);

        void SetTextureParameter(GLenum parameter, GLenum value);
        void SetSamplerParameter(GLenum parameter, GLenum value);
        void SetFiltering(int magnification, int minification);

        void SetWrap();

        int GetMinificationFilter();
        int GetMagnificationFilter();

        void Free();

        std::string GetType();
        void SetType(std::string _type);

        std::string GetPath();
        void SetPath(std::string path);

    private:

        std::string type;
        int mWidth, mHeight, mBPP;
        GLuint mTextureID;
        GLuint mSampler;

        bool mMipMapsGenerated;

        int mMinification, mMagnification;

        std::string mPath;
};
#endif
