#include "PauseState.h"
#include "InitialState.h"
#include <iostream>
using namespace std;
PauseState* PauseState::instance = 0;
PauseState::PauseState(){}
PauseState::~PauseState(){}
void PauseState::Pause(){}
void PauseState::Resume(){}
PauseState* PauseState::GetInstance()
{
    if(instance == 0) instance = new PauseState();
    return instance;
}

void PauseState::Reshape(int newWidth, int newHeight)
{
    // Prevent division by zero
    if(newHeight == 0) newHeight = 1;

    mainProgram->SetScreenWidth(newWidth);
    mainProgram->SetScreenHeight(newHeight);

    glViewport(0, 0, mainProgram->GetScreenWidth(), mainProgram->GetScreenHeight());

}

void PauseState::PrintText(float x, float y, string text, glm::vec3 color)
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

void PauseState::Initialize(GlutProgram* program)
{
}

void PauseState::Finalize()
{
}

void PauseState::Mouse(int button, int state, int xPos, int yPos){}
void PauseState::MousePassive(int xPos, int yPos){}

void PauseState::Keyboard(unsigned char key, int xPos, int yPos)
{
    // Handle keyboard input
    if(key == 27) // ESC
    {
        mainProgram->Quit();
    }
}

void PauseState::Update()
{
    // Update logic
    switch(Menu::GetInstance()->GetSelectedOption())
    {
        case MENU_START:
            GlutProgram::GetInstance()->PopState();
            break;

        case MENU_STOP:
            break;

        case MENU_QUIT:
            mainProgram->Quit();
            break;
        case MENU_PAUSE:
            break;

        case MENU_RAISED_VIEW:
            break;

        case MENU_PLAYER_VIEW:
            break;

    }

 }

void PauseState::Render()
{
    glClear(GL_DEPTH_BUFFER_BIT);

    // Clear the screen
    PrintText(320, 240, "PAUSED!");
}

