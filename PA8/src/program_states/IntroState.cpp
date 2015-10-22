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

    camera = new Camera(glm::vec3(0.0f, 8.0f, -16.0f));


    skybox = new Skybox("Assets/hw_blue/blue_rt.tga",
                        "Assets/hw_blue/blue_lf.tga",
                        "Assets/hw_blue/blue_up.tga",
                        "Assets/hw_blue/blue_dn.tga",
                        "Assets/hw_blue/blue_bk.tga",
                        "Assets/hw_blue/blue_ft.tga");

    // Transformations
    // Table
    models[0]->Scale(glm::vec3(10.0f, 6.0f, 10.0f));

    // Cube
    models[1]->Translate(glm::vec3(-8.0f, 2.0f, 10.0f));
    models[1]->Scale(glm::vec3(3.0f, 3.0f, 3.0f));

    // Pyramid
    models[2]->Translate(glm::vec3(0.0f, 4.0f, 10.0f));

    // Cylinder
    models[3]->Translate(glm::vec3(8.0f, 4.0f, 10.0f));
    models[3]->Scale(glm::vec3(3.0f, 3.0f, 3.0f));

    // Initialize Bullet
    broadphase             = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher             = new btCollisionDispatcher(collisionConfiguration);
    solver                 = new btSequentialImpulseConstraintSolver();
    dynamicsWorld          = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

    // Add gravity
    dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    // Add a static rigid body for the ground
    btCollisionShape* shape = models[0]->GetCollisionShape();

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));
    btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
    btScalar mass(0.0f);
    btVector3 inertia(0.0f, 0.0f, 0.0f);
    //shape->calculateLocalInertia(mass, localInertia);
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape, inertia);
    btRigidBody* body = new btRigidBody(constructionInfo);
    dynamicsWorld->addRigidBody(body);

    //btCollisionShape* box = models[1]->GetCollisionShape();
    //btTransform boxTransform;
    //boxTransform.setIdentity();
    //boxTransform.setOrigin(btVector3())

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
    camera = NULL;
    delete skybox;
    skybox = NULL;

    // Free Bullet
    delete broadphase;
    delete collisionConfiguration;
    delete dispatcher;
    delete solver;
    delete dynamicsWorld;

    broadphase             = NULL;
    collisionConfiguration = NULL;
    dispatcher             = NULL;
    solver                 = NULL;
    dynamicsWorld          = NULL;
}

void IntroState::HandleEvents()
{
}

void IntroState::Update()
{
    // Update logic
    camera->Update();
    dynamicsWorld->stepSimulation(Game::GetInstance()->GetTimeDelta(), 10);
    btTransform trans;
    btScalar m[16];
    for(size_t i = 0; i < dynamicsWorld->getNumCollisionObjects(); i++)
    {
        btRigidBody* object = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[i]);
        object->getMotionState()->getWorldTransform(trans);
        trans.getOpenGLMatrix(m);
        switch(i)
        {
            case 0:
                models[0]->SetModelMatrix(glm::make_mat4(m));
                break;
        }
    }
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

