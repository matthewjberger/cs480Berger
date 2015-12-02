#include "../core/GlutProgram.h"
#include "../core/Model.h"
#include "../core/Texture.h"
#include "../core/Menu.h"
#include "../core/PhysicsManager.h"
#include "../core/Camera.h"
#include "../core/Skybox.h"

class PauseState : public ProgramState
{
    public:
        void Initialize(GlutProgram* program);
        void Finalize();

        void Pause();
        void Resume();
        void PrintText(float x, float y, std::string text, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

        void Mouse(int button, int state, int xPos, int yPos);
        void MousePassive(int xPos, int yPos);
        void Keyboard(unsigned char key, int xPos, int yPos);
        void Reshape(int newWidth, int newHeight);
        void Render();
        void Update();

        static PauseState* GetInstance();

    private:
        PauseState();
        ~PauseState();

        static PauseState* instance;

        GlutProgram* mainProgram;

        /**************************/
        /* Place state items here */
        /**************************/
};
