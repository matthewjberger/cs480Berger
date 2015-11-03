#include "../core/Game.h"
#include "../core/ShaderProgram.h"
#include "../core/VAO.h"
#include "../core/VBO.h"
#include "../core/Camera.h"
#include "../core/Texture.h"
#include "../core/Model.h"
#include "../core/Skybox.h"
#include "../core/PhysicsManager.h"

enum
{
    TABLE    = 0,
    PADDLE_1 = 1,
    PADDLE_2 = 2,
    PUCK     = 3,
};

class IntroState : public GameState
{
    public:
        void Initialize();
        void Finalize();

        void Pause();
        void Resume();

        void Draw();
        void HandleEvents();
        void Update();

        static IntroState *GetInstance()
        {
            if (inst == 0) inst = new IntroState();
            return inst;
        }

    private:
        IntroState(){}
        ~IntroState(){}
        static IntroState *inst;

        /**************************/
        /* Place State Items here */
        /**************************/
        ShaderProgram shaderProgram;
        Camera* camera;
        Skybox *skybox;
        Model* models[3];
        Texture sunTexture;
        PhysicsManager* physicsManager;
};


