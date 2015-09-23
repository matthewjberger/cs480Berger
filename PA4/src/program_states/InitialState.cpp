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
    // Grab the main program instance for use in the program state
    mainProgram = program;

    model.LoadModel("model.obj");

    objVAO.Create();
    objVAO.Bind();
    objVBO.Create();
    objVBO.Bind();
    objVBO.AddData(&model.GetVertices().front(), sizeof(glm::vec3) * model.GetVertices().size());
    objVBO.UploadData(GL_STATIC_DRAW);

    shaderProgram.CreateProgram();
    shaderProgram.AddShaderFromFile("Shaders/vShader.glsl", GL_VERTEX_SHADER);
    shaderProgram.AddShaderFromFile("Shaders/fShader.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.LinkProgram();
    viewMatrix =  glm::lookAt( glm::vec3(0.0, 8.0, -16.0), // Eye Position
                         glm::vec3(0.0, 0.0, 0.0),   // Focus point
                         glm::vec3(0.0, 1.0, 0.0));  // Positive Y is up

    projectionMatrix = glm::perspective( 45.0f,                         // The FoV typically 90 degrees is good which is what this is set to
                                   mainProgram->GetAspectRatio(), // Aspect Ratio, so Circles stay Circular
                                   0.01f,                         // Distance to the near plane, normally a small value like this
                                   100.0f);                       // Distance to the far plane,
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    objVAO.EnableAttribute(shaderProgram.GetAttributeLocation("v_position"));
    objVAO.EnableAttribute(shaderProgram.GetAttributeLocation("v_color"));
    objVBO.Bind();
    objVAO.ConfigureAttribute( shaderProgram.GetAttributeLocation("v_position"), // Location of attribute
                                3,             // Number of elements
                                GL_FLOAT,      // Type
                                GL_FALSE,      // Normalized?
                                sizeof(glm::vec3),// Stride
                                0);            // Offset

    objVAO.ConfigureAttribute( shaderProgram.GetAttributeLocation("v_color"),
                                3,
                                GL_FLOAT,
                                GL_FALSE,
                                sizeof(glm::vec3),
                                0);
}

void InitialState::Finalize()
{
    // Free Resources here
    shaderProgram.DeleteProgram();
    objVAO.Free();
    objVBO.Free();
}

void InitialState::Mouse(int button, int state, int xPos, int yPos)
{
    // Handle Mouse input
}

void InitialState::Keyboard(unsigned char key, int xPos, int yPos)
{
    // Handle keyboard input
    if(key == 27) // ESC
    {
        mainProgram->Quit();
    }
}

void InitialState::Update()
{
    static float angle = 0.0f;

    angle += mainProgram->GetTimeDelta() * M_PI/2;

    // Update logic
    modelMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 1.0f, 1.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 3.0f, 1.0f));
}

void InitialState::Render()
{
    // Clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mvpMatrix = projectionMatrix * viewMatrix * modelMatrix;

    objVAO.Bind();
    shaderProgram.UseProgram();
    shaderProgram.SetUniform("mvpMatrix", &mvpMatrix);
    model.Draw();
 }

void InitialState::Reshape(int newWidth, int newHeight)
{
    // Prevent division by zero
    if(newHeight == 0) newHeight = 1;

    mainProgram->SetScreenWidth(newWidth);
    mainProgram->SetScreenHeight(newHeight);

    glViewport(0, 0, mainProgram->GetScreenWidth(), mainProgram->GetScreenHeight());

    // NOTE: Update the projection matrix here if present
    // Something similar to the code below
    // glm::perspective(45.0f, mainProgram->GetAspectRatio(), 0.01f, 100.0f);
}
