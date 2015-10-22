#include "../core/Game.h"
#include "../core/ShaderProgram.h"
#include "../core/VAO.h"
#include "../core/VBO.h"
#include "../core/Camera.h"
#include "../core/Texture.h"
#include "../core/Model.h"
#include "../core/Skybox.h"

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

        Model* models[4];

        Texture sunTexture;

        /* Bullet */
        // Tells bullet how to check for collisions between objects
        // Helps to eliminate object pairs that should not collide
        btBroadphaseInterface* broadphase;

        // Create a collision configuration
        // This is a collision algorithm
        // It can be used to register a callback that filters overlapping
        // broadphase proxies so that the collisions are not processed by the rest of the system.
        btDefaultCollisionConfiguration* collisionConfiguration;

        // Create a dispatcher
        // Takes in the collisionConfiguration pointer as a parameter
        // used in conjunction with collisionConfiguration for the collision algorithm
        // Sends events to the objects
        btCollisionDispatcher *dispatcher;

        // Create a solver
        // This causes the objects to interact properly
        btSequentialImpulseConstraintSolver* solver;

        // Create a physics world
        // Takes in all the parameters listed above
        btDiscreteDynamicsWorld* dynamicsWorld;
};


