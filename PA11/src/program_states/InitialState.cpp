#include "InitialState.h"
#include "PauseState.h"
#include <iostream>
#include <sstream>
using namespace std;
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

void InitialState::Reshape(int newWidth, int newHeight)
{
    // Prevent division by zero
    if(newHeight == 0) newHeight = 1;

    mainProgram->SetScreenWidth(newWidth);
    mainProgram->SetScreenHeight(newHeight);

    glViewport(0, 0, mainProgram->GetScreenWidth(), mainProgram->GetScreenHeight());

}

void PrintText(float x, float y, string text, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f))
{
    GlutProgram* mainProgram = GlutProgram::GetInstance();

    // Render text with fixed-function pipeline calls
    glUseProgram(0);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, mainProgram->GetScreenWidth() - 1.0, 0.0, mainProgram->GetScreenHeight() - 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(x,y);
    glColor3f(color.x, color.y, color.z);
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text.c_str());
    glEnable(GL_TEXTURE_2D);
}

void InitialState::Initialize(GlutProgram* program)
{
    // Grab the main program instance for use in the program state
    mainProgram = program;

    contextMenu = Menu::GetInstance();
    contextMenu->AddEntry("Start", MENU_START );
    contextMenu->AddEntry("Quit",  MENU_QUIT  );
    contextMenu->AddEntry("Pause", MENU_PAUSE );
    contextMenu->AddEntry("High View", MENU_RAISED_VIEW);
    contextMenu->AddEntry("Low View", MENU_PLAYER_VIEW);
    contextMenu->AttachToMouseRight();

    camera = new Camera();

    // Raised view
    camera->LookAt(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0));

    shaderProgram.CreateProgram();
    shaderProgram.AddShaderFromFile("Shaders/modelVert.glsl", GL_VERTEX_SHADER);
    shaderProgram.AddShaderFromFile("Shaders/modelFrag.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.LinkProgram();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    models[0] = new Model("Assets/labyrinth.obj",  "Assets/mazefloor.png", true);
    models[1] = new Model("Assets/sphere.obj",     "Assets/blue.jpg", true);

    physicsManager = new PhysicsManager();

    // Generate collision shapes
    btCollisionShape* sphereShape   = new btSphereShape(0.05f);

    // Add rigid bodies
    physicsManager->AddRigidBody(models[0]->GetCollisionShape());
    physicsManager->AddRigidBody(sphereShape,
            btVector3(0.0f, 2.0f, 0.0f),
            btScalar(1.0f),
            btScalar(0.3f));

    skybox = new Skybox("Assets/skybox/right.jpg",
            "Assets/skybox/left.jpg",
            "Assets/skybox/top.jpg",
            "Assets/skybox/bottom.jpg",
            "Assets/skybox/back.jpg",
            "Assets/skybox/front.jpg");

    start = std::clock();
}

void InitialState::Finalize()
{
    // Free Resources here
    shaderProgram.DeleteProgram();

    models[0]->Free();
    models[1]->Free();

    delete camera;
    delete skybox;
    delete physicsManager;

    camera = NULL;
    skybox = NULL;
    physicsManager = NULL;
}

void InitialState::Mouse(int button, int state, int xPos, int yPos){}
void InitialState::MousePassive(int xPos, int yPos){}

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
    // Update logic
    switch(contextMenu->GetSelectedOption())
    {
        case MENU_START:
            break;

        case MENU_QUIT:
            mainProgram->Quit();
            break;

        case MENU_PAUSE:
            GlutProgram::GetInstance()->PushState(PauseState::GetInstance());
            break;

        case MENU_RAISED_VIEW:
            // Raised view
            camera->LookAt(glm::vec3(2.00f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0));
            break;

        case MENU_PLAYER_VIEW:
            // Player view
            camera->LookAt(glm::vec3(2.0f, 1.0f, 0.01f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0));
            break;

    }

    camera->Update();
    physicsManager->Update();
    models[0]->SetModelMatrix(physicsManager->GetModelMatrixAtIndex(0));
    models[1]->SetModelMatrix(physicsManager->GetModelMatrixAtIndex(1));
    models[1]->Scale(glm::vec3(0.05,0.05,0.05));

    duration = (( std::clock() - start ) / (double) 360 );
    duration = duration/1000.00;
 }

void InitialState::Render()
{
    // Clear the screen
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox->Draw(camera->projectionMatrix, camera->viewMatrix);

    shaderProgram.UseProgram();

    // Draw table
    shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(models[0]->GetModelMatrix()));
    models[0]->Draw();

    // Draw puck
    shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(models[1]->GetModelMatrix()));
    models[1]->Draw();

    PrintText(0, 0,  "Current Score: 0");

    std::ostringstream ss;
    ss << "Elapsed Time: ";
    int d = duration;
    ss << d << " seconds";
    PrintText(320,400, ss.str());
}

