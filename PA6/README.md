# PA6 - Assimp Texture Loading

## Authors
Matthew Berger & Matt Fredrickson

##Dependencies
This project uses SOIL (Simple OpenGL Image Loader) to load images. To install this library on Ubuntu, run the command:

    sudo apt-get install libsoil-dev
## Goal
This project will load a textured model of a house using Assimp.

## Notes
For this project, my partner and I made a Model and Mesh class. The model class allows the user to load a model using assimp and instantiates a Mesh object for each mesh in the scene using a function called ProcessMesh() which sets up the vertices and texture coordinates for the mesh:

    Mesh Model::ProcessMesh(aiMesh* mesh)
    {
        vector<Vertex> vertices;
        vector<GLuint> indices;
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            const aiFace& face = mesh->mFaces[i];
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }
        for(unsigned int i = 0; i < indices.size(); i++)
        {
            const aiVector3D* temp = &(mesh->mVertices[indices[i]]);
            Vertex v;
            v.Position.x = temp->x;
            v.Position.y = temp->y;
            v.Position.z = temp->z;

            // Texture Coordinates
            if(mesh->HasTextureCoords(0))
            {
                v.TexCoords = glm::vec2(mesh->mTextureCoords[0][indices[i]].x, mesh->mTextureCoords[0][indices[i]].y);
            }
            else
            {
                v.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(v);
        }
        return Mesh(vertices);
    }

This Mesh then sets up a VAO and VBO. It also allows for a texture to be loaded and binds that texture when drawing.

    void Mesh::SetupMesh()
    {
        meshVAO.Create();
        meshVBO.Create();
        meshVAO.Bind();
        meshVBO.Bind();
        meshVBO.AddData(&vertices.front(), sizeof(Vertex) * vertices.size());
        meshVBO.UploadData();

        // Vertex Positions
        meshVAO.EnableAttribute(0);
        meshVAO.ConfigureAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

        // Vertex Texture Coordinates
        meshVAO.EnableAttribute(1);
        meshVAO.ConfigureAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
        meshVAO.Unbind();
    }

Finally, in order to load textures, we made a dedicated Texture class that wraps the functionality for loading and storing OpenGL textures. Here is the load function for example:

    void Texture::Load(string path, bool genMipMaps)
    {
    // The texture id
    mTextureID = 0;

    // Load the image
    unsigned char* textureImage = SOIL_load_image(path.c_str(), &mWidth, &mHeight, 0, SOIL_LOAD_RGB);

    // Check for errors
    if (textureImage == NULL)
    {
        printf("Couldn't load image %s./n", path.c_str());
    }

    // Generate the texture and bind it
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    // Set pixel mode
    int pixelMode = GL_RGB;

    // Send data to gpu
    glTexImage2D(GL_TEXTURE_2D, 0, pixelMode, mWidth, mHeight, 0, pixelMode, GL_UNSIGNED_BYTE, textureImage);

    // Generate mipmaps if requested
    if (genMipMaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        // Set mipmap generation flag
        mMipMapsGenerated = true;
    }

    // Generate samplers
    glGenSamplers(1, &mSampler);

    // Set filtering
    SetFiltering(GL_LINEAR, GL_LINEAR);

    // Set path
    mPath = path;

    // Parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Get rid of the temporary surface
    SOIL_free_image_data(textureImage);
    }

