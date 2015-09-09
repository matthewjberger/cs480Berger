Interaction: Menus, Keyboard, and Mouse
===
This program renders a spinning cube moving in a circular path around the origin. The cube can have its spinning paused, restarted, and reversed through different keyboard, mouse, and menu input.

Note
-----

The main code for this project is contained within the *src/program_states/InitialState.cpp* file. I have created a 3D graphics engine that utilizes glut commands to make these projects easier and more manageable as they scale in difficulty and complexity throughout the semester. Details on this framework are declared at the end of this README.

Main Features
-------------

####Keyboard Input
Pressing the 'A' or 'a' key will cause the cube to reverse rotation.

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

####Mouse Input
Clicking the screen will causes the mouse to reverse rotation as well.

    void InitialState::Mouse(int button, int state, int xPos, int yPos)
    {
        // Handle mouse input
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){reversed = !reversed;}
    }

####Context Menu
Right clicking the screen will cause a context menu to appear with three options as stated in the enum below:

    enum MenuOptions
    {
        START_SPINNING = 0,
        PAUSE_SPINNING = 1,
        EXIT_PROGRAM   = 2
    };

I wrote a wrapper for context menues and the resulting function calls were as follows:

    // Menu creation
    Menu *contextMenu = Menu::GetInstance();
    contextMenu->AddEntry("Start Spinning", START_SPINNING);
    contextMenu->AddEntry("Pause Spinning", PAUSE_SPINNING);
    contextMenu->AddEntry("Exit Program",   EXIT_PROGRAM);
    contextMenu->AttachToMouseRight();

Finally I check the selected option in the InitialState's Update() function:

    switch(Menu::GetInstance()->GetSelectedOption())
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


## Gluttony Engine

For the sake of ease in this project and future projects, I have wrapped many of the glut functions and concepts presented thus far into useful classes. I have also created a state machine for better organization. I've titled my framework 'Gluttony'. The structure of Gluttony is represented below:

![alt text](https://github.com/matthewjberger/cs480Berger/blob/master/PA2/file_structure.png "Gluttony Engine File Structure")

#### Makefile

With this new file structure, I had to modify the makefile to search through subdirectories, create object files, and link those object files properly.

#### GlutProgram.h

This is the main file. It is a singleton containing a vector of pointers to objects that inherit from an abstract base class called 'ProgramState'.

       // Program States
       std::vector<ProgramState *> ProgramStates;

What this allows me to do is have a single GlutProgram object instantiated in main.cpp, and simply have it use this stack of "ProgramStates" to determine what resources to use, when to free them, and in what order.

#### Callbacks
I wrapped all the glut callbacks necessary for this in the ProgramState abstract class as per below:

        // GLUT callbacks
        virtual void Mouse(int button, int state, int xPos, int yPos) = 0;
        virtual void Keyboard(unsigned char key, int xPos, int yPos) = 0;
        virtual void Reshape(int newWidth, int newHeight) = 0;
        virtual void Render() = 0;
        virtual void Update() = 0;

I declare a singleton class called 'InitialState' that inherits from the ProgramState abstract base class and has these calls defined. These are then called from the main GlutProgram object like so:

    // GLUT Callback handlers
    void RenderCallback()
    {
        GlutProgram::GetInstance()->Render();
    }

    void ReshapeCallback(int width, int height)
    {
        GlutProgram::GetInstance()->Reshape(width, height);
    }

    void UpdateCallback()
    {
        GlutProgram::GetInstance()->Update();
    }

    void MouseCallback(int button, int state, int xPos, int yPos)
    {
        GlutProgram::GetInstance()->Mouse(button, state, xPos, yPos);
    }

    void KeyboardCallback(unsigned char key, int xPos, int yPos)
    {
        GlutProgram::GetInstance()->Keyboard(key, xPos, yPos);
    }

In the GlutProgram::Initialize() function the callbacks are set:

    // Setup GLUT callbacks
    glutDisplayFunc(RenderCallback);    // Called when its time to display
    glutReshapeFunc(ReshapeCallback);   // Called if the window is resized
    glutIdleFunc(UpdateCallback);       // Called if there is nothing else to do
    glutKeyboardFunc(KeyboardCallback); // Called if there is keyboard input
    glutMouseFunc(MouseCallback);       // Called if there is mouse input

This structure allows me to setup the glut callbacks dynamically without being constrained to a single program state due to the global availability of the singleton GlutProgram class allowing external functions to call public commands.

####
Wrappers

I have wrapped Vertex Array Objects, Vertex Buffer Objects, Shaders, Shader Programs, and Menus. I have also moved out a lot of the common headers to a 'GlobalIncludes.h' file.

#### Main.cpp

Here is main.cpp in its entirety:

    #include "core/GlutProgram.h"
    #include "program_states/InitialState.h"

    int main(int argc, char **argv)
    {
        // Create the program
        GlutProgram* mainProgram = GlutProgram::GetInstance();

        if(mainProgram->Initialize(argc, argv) == false)
        {
            printf("ERROR: Failed to initialize program!");
        }

        if(mainProgram->LoadContent(InitialState::GetInstance()) == false)
        {
            printf("ERROR: Failed to load content!");
        }

        mainProgram->Run();

        return 0;
    }

##Final notes
The repository for my projects now has a script called "buildall" that contains one line of code:

    #!/bin/bash
    for d in ./*/build ; do (cd "$d" && make); done

This code will go into all of the build directories for my projects and run the makefile for that project in a subshell.

I also added the Gluttony engine as a git submodule to this repository for convenience in future programming assignments. It exists as a private repository of mine.
