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
    models[2] = new Model("Assets/pyramid.obj",  "Assets/blue.jpg");
    models[3] = new Model("Assets/cylinder.obj", "Assets/green.jpg");

    shaderProgram.CreateProgram();
    shaderProgram.AddShaderFromFile("Shaders/modelVert.glsl", GL_VERTEX_SHADER);
    shaderProgram.AddShaderFromFile("Shaders/modelFrag.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.LinkProgram();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    camera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f), 0.7f, M_PI, -1 * M_PI );


    skybox = new Skybox("Assets/hw_blue/blue_rt.tga",
                        "Assets/hw_blue/blue_lf.tga",
                        "Assets/hw_blue/blue_up.tga",
                        "Assets/hw_blue/blue_dn.tga",
                        "Assets/hw_blue/blue_bk.tga",
                        "Assets/hw_blue/blue_ft.tga");

    physicsManager = new PhysicsManager();

    // Generate collision shapes
    btCollisionShape* floorShape = new btBoxShape(btVector3(3.0f, 0.1f, 3.0f));

    btCollisionShape* boxShape   = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));

    // Add floor
    physicsManager->AddRigidBody(floorShape,
                                 btVector3(0.0f, -6.0f, 0.0f),
                                 btScalar(0.0f));

    physicsManager->AddRigidBody(boxShape,
                                 btVector3(0.0f, -2.0f, 0.0f),
                                 btScalar(1.0f));
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

void IntroState::HandleEvents(){}

void IntroState::Update()
{
    // Update logic
    camera->Update();
    physicsManager->Update();
    models[0]->SetModelMatrix(physicsManager->GetModelMatrixAtIndex(0));
    models[1]->SetModelMatrix(physicsManager->GetModelMatrixAtIndex(1));
}

void IntroState::Draw()
{
    // Render logic
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox->Draw(camera->projectionMatrix, camera->viewMatrix);
    shaderProgram.UseProgram();
    for(size_t i = 0; i < SIZE(models) - 2; i++)
    {
        shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(models[i]->GetModelMatrix()));
        models[i]->Draw();
    }
}

