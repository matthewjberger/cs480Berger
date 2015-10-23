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


    skybox = new Skybox("Assets/hw_blue/blue_rt.tga",
            "Assets/hw_blue/blue_lf.tga",
            "Assets/hw_blue/blue_up.tga",
            "Assets/hw_blue/blue_dn.tga",
            "Assets/hw_blue/blue_bk.tga",
            "Assets/hw_blue/blue_ft.tga");

    physicsManager = new PhysicsManager();

    // Generate collision shapes
    btCollisionShape* boxShape      = new btBoxShape(btVector3(1,1,1));
    btCollisionShape* sphereShape   = new btSphereShape(1.2f);
    btCollisionShape* cylinderShape = new btCylinderShape(btVector3(1, 0.5, 10));

    // Add floor
    physicsManager->AddRigidBody(models[0]->GetCollisionShape());

    physicsManager->AddRigidBody(boxShape,
            btVector3(0.0f, 1.0f, 0.0f), // origin
            btScalar(0.0f),              // mass
            btScalar(1.0f),              // restitution
            btVector3(0.0f, 0.0f, 0.0f), // inertia
            true);                       // kinematic

    physicsManager->AddRigidBody(sphereShape,
            btVector3(1.0f, 14.0f, 0.0f),
            btScalar(2.0f),
            btScalar(0.3f));

    physicsManager->AddRigidBody(sphereShape,
            btVector3(-1.0f, 13.0f, 0.0f),
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

