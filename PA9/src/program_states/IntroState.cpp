#include "IntroState.h"
#include "../core/GlobalIncludes.h"
void IntroState::Pause(){}
void IntroState::Resume(){}
IntroState *IntroState::inst = 0;

void IntroState::Initialize()
{
    // Initialize resources
    models[0] = new Model("Assets/hockey/Table.obj", "Assets/hockey/wood.jpg");
    models[1] = new Model("Assets/hockey/Paddle.obj","Assets/hockey/whiteplastic.jpg");
    models[2] = new Model("Assets/hockey/puck.obj",  "Assets/hockey/puck.jpg");

    shaderProgram.CreateProgram();
    shaderProgram.AddShaderFromFile("Shaders/modelVert.glsl", GL_VERTEX_SHADER);
    shaderProgram.AddShaderFromFile("Shaders/modelFrag.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.LinkProgram();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    camera = new Camera(glm::vec3(0.0f, 12.0f, 8.0f), 0.7f, M_PI, -1);
    camera->UseInput(true);


    skybox = new Skybox("Assets/hw_blue/blue_rt.tga",
            "Assets/hw_blue/blue_lf.tga",
            "Assets/hw_blue/blue_up.tga",
            "Assets/hw_blue/blue_dn.tga",
            "Assets/hw_blue/blue_bk.tga",
            "Assets/hw_blue/blue_ft.tga");

    physicsManager = new PhysicsManager();

    // Generate collision shapes
    btCollisionShape* paddle_1 = new btCylinderShape(btVector3(1, 0.5, 5)); // TODO: These need to be modified to match actual dimensions of paddles and puck
    btCollisionShape* paddle_2 = new btCylinderShape(btVector3(1, 0.5, 5));
    btCollisionShape* puck     = new btCylinderShape(btVector3(1, 0.5, 5));

    // Add floor
    physicsManager->AddRigidBody(models[0]->GetCollisionShape());

    physicsManager->AddRigidBody(paddle_1,
            btVector3(0.0f, 4.0f, 0.0f), // origin
            btScalar(1.0f));

    physicsManager->AddRigidBody(paddle_2,
            btVector3(-1.0f, 4.0f, 0.0f),
            btScalar(1.0f),
            btScalar(0.3f));

    physicsManager->AddRigidBody(puck,
            btVector3(1.0f, 4.0f, 0.0f),
            btScalar(2.0f),
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
}

void IntroState::Draw()
{
    // Render logic
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox->Draw(camera->projectionMatrix, camera->viewMatrix);

    shaderProgram.UseProgram();

    // Draw table
    shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(physicsManager->GetModelMatrixAtIndex(TABLE)));
    models[0]->Draw();

    // Draw paddles
    shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(physicsManager->GetModelMatrixAtIndex(PADDLE_1)));
    models[1]->Draw();

    shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(physicsManager->GetModelMatrixAtIndex(PADDLE_2)));
    models[1]->Draw();

    // Draw puck
    shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(physicsManager->GetModelMatrixAtIndex(PUCK)));
    models[2]->Draw();

}

