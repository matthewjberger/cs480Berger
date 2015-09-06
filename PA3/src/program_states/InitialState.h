#include "../core/GlutProgram.h"

class InitialState : public ProgramState
{
    public:
        void Initialize(GlutProgram* program);
        void Finalize();

        void Pause();
        void Resume();

        void Mouse(int button, int state, int xPos, int yPos);
        void Keyboard(unsigned char key, int xPos, int yPos);
        void Reshape(int newWidth, int newHeight);
        void Render();
        void Update();

        static InitialState* GetInstance();

        void SetSelectedMenuOption(int option);

    private:
        InitialState();
        ~InitialState();

        static InitialState* instance;

        GlutProgram* mainProgram;

        /**************************/
        /* Place state items here */
        /**************************/
        struct Vertex
        {
            GLfloat position[3];
            GLfloat color[3];
        };

        ShaderProgram shaderProgram;
        VAO cubeVAO;
        VBO cubeVBO;

        glm::mat4 moonMVP;
        glm::mat4 moonModelMatrix;
        glm::mat4 planetModelMatrix;
        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 planetMVP;

        std::chrono::time_point<std::chrono::high_resolution_clock> t1,t2;

        bool stopped;
        bool reversed;
        int selectedMenuOption;

        enum MenuOptions
        {
            START_SPINNING = 0,
            PAUSE_SPINNING = 1,
            EXIT_PROGRAM   = 2
        };
};
