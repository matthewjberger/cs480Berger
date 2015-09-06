#include "InitialState.h"

InitialState* InitialState::instance = 0;
InitialState::InitialState(){}
InitialState::~InitialState(){}
void InitialState::Pause(){}
void InitialState::Resume(){}
InitialState* InitialState::GetInstance()
{
    if(instance == 0) instance = new InitialState();
    return instance;
}

void InitialState::Initialize(GlutProgram* program)
{
    mainProgram = program;

    contextMenu = Menu::GetInstance();
    contextMenu->AddEntry("Start Spinning", START_SPINNING);
    contextMenu->AddEntry("Pause Spinning", PAUSE_SPINNING);
    contextMenu->AddEntry("Exit Program",   EXIT_PROGRAM);
    contextMenu->AttachToMouseRight();

    // Initialize variables
    reversed = true;
    stopped  = false;

    Vertex geometry[] =
    {
        {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
        {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
        {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},

        {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
        {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
        {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},

        {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},
        {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
        {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},

        {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
        {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
        {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},

        {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},
        {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
        {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},

        {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},
        {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
        {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.0}},

        {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
        {{-1.0, -1.0, 1.0}, {0.0, 0.0, 1.0}},
        {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},

        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
        {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
        {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},

        {{1.0, -1.0, -1.0}, {1.0, 0.0, 0.0}},
        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
        {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}},

        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
        {{1.0, 1.0, -1.0}, {1.0, 1.0, 0.0}},
        {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},

        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
        {{-1.0, 1.0, -1.0}, {0.0, 1.0, 0.0}},
        {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},

        {{1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}},
        {{-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}},
        {{1.0, -1.0, 1.0}, {1.0, 0.0, 1.0}}
    };

    cubeVAO.Create();
    cubeVAO.Bind();
    cubeVBO.Create();
    cubeVBO.Bind();
    cubeVBO.AddData(geometry, sizeof(geometry));
    cubeVBO.UploadData(GL_STATIC_DRAW);
    shaderProgram.CreateProgram();
    shaderProgram.AddShaderFromFile( "Shaders/vShader.glvs", GL_VERTEX_SHADER );
    shaderProgram.AddShaderFromFile( "Shaders/fShader.glfs", GL_FRAGMENT_SHADER );
    shaderProgram.LinkProgram();
    view =  glm::lookAt( glm::vec3(0.0, 8.0, -16.0), // Eye Position
                         glm::vec3(0.0, 0.0, 0.0),   // Focus point
                         glm::vec3(0.0, 1.0, 0.0));  // Positive Y is up

    projection = glm::perspective( 45.0f,                         // The FoV typically 90 degrees is good which is what this is set to
                                   mainProgram->GetAspectRatio(), // Aspect Ratio, so Circles stay Circular
                                   0.01f,                         // Distance to the near plane, normally a small value like this
                                   100.0f);                       // Distance to the far plane,
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    cubeVAO.EnableAttribute(shaderProgram.GetAttributeLocation("v_position"));
    cubeVAO.EnableAttribute(shaderProgram.GetAttributeLocation("v_color"));
    cubeVBO.Bind();
    cubeVAO.ConfigureAttribute( shaderProgram.GetAttributeLocation("v_position"), // Location of attribute
                                3,             // Number of elements
                                GL_FLOAT,      // Type
                                GL_FALSE,      // Normalized?
                                sizeof(Vertex),// Stride
                                0);            // Offset

    cubeVAO.ConfigureAttribute( shaderProgram.GetAttributeLocation("v_color"),
                                3,
                                GL_FLOAT,
                                GL_FALSE,
                                sizeof(Vertex),
                                (void*)offsetof(Vertex,color));
}

void InitialState::Finalize()
{
    // Free Resources here
    shaderProgram.DeleteProgram();
    cubeVAO.Free();
    cubeVBO.Free();
}

void InitialState::Mouse(int button, int state, int xPos, int yPos)
{
    // Handle mouse input
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){reversed = !reversed;}
}

void InitialState::Keyboard(unsigned char key, int xPos, int yPos)
{
    // Handle keyboard input
    if(key == 27) // ESC
    {
        mainProgram->Quit();
    }
    else if(key == 'A' || key == 'a')
    {
        reversed = !reversed;
    }
}

void InitialState::Update()
{
    switch(contextMenu->GetSelectedOption())
    {
        case START_SPINNING:
            stopped = false;
            break;
        case PAUSE_SPINNING:
            stopped = true;
            break;
        case EXIT_PROGRAM:
            mainProgram->Quit();
            break;
    }

    static float translationAngle = 0.0;
    static float rotationAngle = 0.0;
    static float moonAngle = 0.0;

    float deltaTimeIncrement = mainProgram->GetTimeDelta() * M_PI/2;
    translationAngle += deltaTimeIncrement;
    moonAngle -= deltaTimeIncrement;

    if(!stopped)
    {
        if(reversed)
            rotationAngle -= deltaTimeIncrement;
        else
            rotationAngle += deltaTimeIncrement;
    }

    planetModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0 * sin(translationAngle), 0.0, 4.0 * cos(translationAngle)));
    moonModelMatrix = glm::translate(planetModelMatrix, glm::vec3(3.0 * cos(4 * moonAngle), 0.0, 3.0 * sin(4 * moonAngle)));
    moonModelMatrix = glm::scale(moonModelMatrix, glm::vec3(0.2, 0.2, 0.2));
    planetModelMatrix = glm::rotate(planetModelMatrix, 4*rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void InitialState::Render()
{
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    planetMVP = projection * view * planetModelMatrix;
    moonMVP = projection * view * moonModelMatrix;
    cubeVAO.Bind();
    shaderProgram.UseProgram();
    shaderProgram.SetUniform("mvpMatrix", &planetMVP);
    glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count
    shaderProgram.SetUniform("mvpMatrix", &moonMVP);
    glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count
}

void InitialState::Reshape(int newWidth, int newHeight)
{
    if(newHeight == 0) newHeight = 1;
    mainProgram->SetScreenWidth(newWidth);
    mainProgram->SetScreenHeight(newHeight);
    glViewport(0, 0, mainProgram->GetScreenWidth(), mainProgram->GetScreenHeight());
    glm::perspective(45.0f, mainProgram->GetAspectRatio(), 0.01f, 100.0f);
}

