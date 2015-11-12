#include "IntroState.h"
#include "../core/GlobalIncludes.h"
void IntroState::Pause(){}
void IntroState::Resume(){}
IntroState *IntroState::inst = 0;

void IntroState::Initialize()
{
    // Initialize resources
    models[0] = new Model("Assets/pill.obj",     "Assets/wood.jpg");
    models[1] = new Model("Assets/cube.obj",     "Assets/crate.jpg");
    models[2] = new Model("Assets/sphere.obj",   "Assets/blue.jpg");
    models[3] = new Model("Assets/cylinder.obj", "Assets/green.jpg");

    shaderProgram.CreateProgram();
    shaderProgram.AddShaderFromFile("Shaders/modelVert.glsl", GL_VERTEX_SHADER);
    shaderProgram.AddShaderFromFile("Shaders/modelFrag.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.LinkProgram();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    camera = new Camera(glm::vec3(0.0f, 12.0f, 8.0f), 0.7f, M_PI, -1);


    skybox = new Skybox("Assets/skybox/right.jpg",
            "Assets/skybox/left.jpg",
            "Assets/skybox/top.jpg",
            "Assets/skybox/bottom.jpg",
            "Assets/skybox/back.jpg",
            "Assets/skybox/front.jpg");

    physicsManager = new PhysicsManager();

    // Generate collision shapes
    btCollisionShape* boxShape      = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    btCollisionShape* sphereShape   = new btSphereShape(1.2f);
    btCollisionShape* cylinderShape = new btCylinderShapeZ(btVector3(1, 0.5, 5));

    // Add floor
    physicsManager->AddRigidBody(models[0]->GetCollisionShape());

    physicsManager->AddRigidBody(boxShape,
            btVector3(0.0f, 1.0f, 0.0f), // origin
            btScalar(0.0f),              // mass
            btScalar(1.0f),              // restitution
            btVector3(0.0f, 0.0f, 0.0f), // inertia
            true);                       // kinematic

    physicsManager->AddRigidBody(sphereShape,
            btVector3(-1.0f, 7.0f, 0.0f),
            btScalar(2.0f),
            btScalar(0.3f));

    physicsManager->AddRigidBody(sphereShape,
            btVector3(1.0f, 5.0f, 0.0f),
            btScalar(1.0f),
            btScalar(0.3f));
}

void IntroState::Finalize()
{
    // Free resources
    for(size_t i = 0; i < SIZE(models); i++)
    {
        models[i]->Free();
    }
    shaderProgram.DeleteProgram();
    delete camera;
    delete skybox;
    delete physicsManager;

    camera         = NULL;
    skybox         = NULL;
    physicsManager = NULL;
}

void IntroState::HandleEvents()
{
    SDL_Event event = Game::GetInstance()->GetEvent();
    if(event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_SPACE:
                physicsManager->ApplyForceAtIndex(btVector3(0, 6, 0), 2);
                physicsManager->ApplyForceAtIndex(btVector3(0, 6, 0), 3);
                break;

            case SDLK_w:
                physicsManager->ApplyForceAtIndex(btVector3(0, 0, -10), 2);
                break;

            case SDLK_a:
                physicsManager->ApplyForceAtIndex(btVector3(-10, 0, 0), 2);
                break;

            case SDLK_s:
                physicsManager->ApplyForceAtIndex(btVector3(0, 0, 10), 2);
                break;

            case SDLK_d:
                physicsManager->ApplyForceAtIndex(btVector3(10, 0, 0), 2);
                break;

            case SDLK_UP:
                physicsManager->ApplyForceAtIndex(btVector3(0, 0, -10), 3);
                break;

            case SDLK_DOWN:
                physicsManager->ApplyForceAtIndex(btVector3(0, 0, 10), 3);
                break;

            case SDLK_LEFT:
                physicsManager->ApplyForceAtIndex(btVector3(-10, 0, 0), 3);
                break;

            case SDLK_RIGHT:
                physicsManager->ApplyForceAtIndex(btVector3(10, 0, 0), 3);
                break;
        }
    }
}

void IntroState::Update()
{
    // Update logic
    camera->Update();
    physicsManager->Update();
    models[0]->SetModelMatrix(physicsManager->GetModelMatrixAtIndex(0));
    models[1]->SetModelMatrix(physicsManager->GetModelMatrixAtIndex(1));
    models[2]->SetModelMatrix(physicsManager->GetModelMatrixAtIndex(2));
    models[3]->SetModelMatrix(physicsManager->GetModelMatrixAtIndex(3));
}

void IntroState::Draw()
{
    // Render logic
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox->Draw(camera->projectionMatrix, camera->viewMatrix);
    shaderProgram.UseProgram();
    for(size_t i = 0; i < SIZE(models); i++)
    {
        shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(models[i]->GetModelMatrix()));
        models[i]->Draw();
    }
}

