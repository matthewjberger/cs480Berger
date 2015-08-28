A Simple Example of Matrix Use in OpenGL
===

Notes
-----
#### Shader Files

I extracted the shader code from the original main.cpp file and placed each set of shader code into their own files.

1. Shaders/vShader.glvs
2. Shaders/fShader.glfs

#### Shader Loading
For this program, I added two functions:

    GLuint LoadShaderFromFile(string path, GLuint shaderType);

This function will create and compile a shader from a plain-text file of any extension. A shader type must be specified (i.e. GL\_VERTEX\_SHADER, GL\_FRAGMENT\_SHADER, etc). This function returns the ID of the loaded shader for use in linking to the shader program.

    void PrintShaderLog(int shaderID);

This function will print information about the shader of a specified id if an error occurs during the loading process.

#### Rotation
    model = glm::rotate(model, 4.0 * angle, glm::vec3(0.0f, 1.0f, 0.0f));

Finally, this line of code makes the cube rotate about its y-axis as requested.

#### Additional notes
    glDelete(vertex_shader);
    glDelete(fragment_shader);
I delete the shaders after attaching to the shader program, as the loaded shaders are no longer needed in memory once attached to the shader program.

Building This Example
---------------------

*This example requires GLM*
*On ubuntu it can be installed with this command*

>$ sudo apt-get install libglm-dev

*On a Mac you can install GLM with this command(using homebrew)*
>$ brew install glm

To build this example just

>$ cd build
>$ make

*If you are using a Mac you will need to edit the makefile in the build directory*

The excutable will be put in bin

Additional Notes For OSX Users
------------------------------

Ensure that the latest version of the Developer Tools is installed.
