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
    btRigidBody::btRigidBodyConstructionInfo constructionInfo(mass, motionState, shape, inertia);
    btRigidBody* body = new btRigidBody(constructionInfo);
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    body->setActivationState(DISABLE_DEACTIVATION);
    dynamicsWorld->addRigidBody(body, COLLIDE_GROUND, COLLIDE_OBJECT);

    //btCollisionShape* box = models[1]->GetCollisionShape();
    btCollisionShape* box = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
    btTransform boxTransform;
    boxTransform.setIdentity();
    boxTransform.setOrigin(btVector3(0.0f, 4.0f, 0.0f));
    btDefaultMotionState* boxMotionState = new btDefaultMotionState(boxTransform);
    btScalar boxMass(1.0f);
    btVector3 boxInertia(0.0f, 0.0f, 0.0f);
    box->calculateLocalInertia(boxMass, boxInertia);
    btRigidBody::btRigidBodyConstructionInfo boxConstructionInfo(boxMass, boxMotionState, box, boxInertia);
    btRigidBody* boxBody = new btRigidBody(boxConstructionInfo);
    dynamicsWorld->addRigidBody(boxBody, COLLIDE_OBJECT, COLLIDE_GROUND);

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
        models[i]->SetModelMatrix(glm::make_mat4(m));
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

