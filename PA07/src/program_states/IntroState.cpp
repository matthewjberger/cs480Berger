#include "IntroState.h"
#include "../core/GlobalIncludes.h"
void IntroState::Pause(){}
void IntroState::Resume(){}
IntroState *IntroState::inst = 0;

void IntroState::Initialize()
{
    // Initialize resources
    model = new Model("Assets/planets/sphere.obj");
    sun = new Model("Assets/planets/sphere.obj");
    sun->LoadTexture("Assets/planets/sun.png", true);
    asteroid = new Model("Assets/asteroid/asteroid.obj");
    asteroid->LoadTexture("Assets/asteroid/asteroid.jpg", false);

    shaderProgram.CreateProgram();
    shaderProgram.AddShaderFromFile("Shaders/modelVert.glsl", GL_VERTEX_SHADER);
    shaderProgram.AddShaderFromFile("Shaders/modelFrag.glsl", GL_FRAGMENT_SHADER);
    shaderProgram.LinkProgram();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    camera = new Camera(glm::vec3(3000.0, 10.0, 3000.0), 200.0f);


    skybox = new Skybox("Assets/ame_starfield/starfield_rt.tga",
                        "Assets/ame_starfield/starfield_lf.tga",
                        "Assets/ame_starfield/starfield_rt.tga",
                        "Assets/ame_starfield/starfield_dn.tga",
                        "Assets/ame_starfield/starfield_bk.tga",
                        "Assets/ame_starfield/starfield_ft.tga"
            );

    GLuint amount = 600;
    modelMatrices = new glm::mat4[amount];
    srand(SDL_GetTicks()); // initialize random seed
    GLfloat radius = 100.0f;
    GLfloat offset = 2.5f;
    for(GLuint i = 0; i < amount; i++)
    {
        glm::mat4 model;
        // 1. Translation: displace along circle with 'radius' in range [-offset, offset]
        GLfloat angle = (GLfloat)i / (GLfloat)amount * 360.0f;
        GLfloat displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat x = sin(angle) * radius + displacement;
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat y = displacement * 0.4f; // y value has smaller displacement
        displacement = (rand() % (GLint)(2 * offset * 100)) / 100.0f - offset;
        GLfloat z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));
        // 2. Scale: Scale between 0.05 and 0.25f
        GLfloat scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));
        // 3. Rotation: add random rotation around a (semi)randomly picked rotation axis vector
        GLfloat rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));
        // 4. Now add to list of matrices
        modelMatrices[i] = model;
    }

    ifstream fin("planet.data");
    fin.clear();

    string line = "";
    string buffer = "";
    int i = 0;

    while (getline(fin,line,'\n'))
    {
        istringstream stream(line);

        float lat_radius    = 0.0f;
        float long_radius   = 0.0f;
        float orbitSpeed    = 0.0f;
        float rotationSpeed = 0.0f;
        float size          = 0.0f;
        int numMoons        = 0;

        // Simulation speed and scaling modifiers
        float simulationSpeed = 0.005f;
        float orbitScale      = 600.0f;
        float scale           = 50.0f;

        stream >> buffer;   lat_radius    = orbitScale * stof(buffer);
        stream >> buffer;   long_radius   = orbitScale * stof(buffer);
        stream >> buffer;   orbitSpeed    = simulationSpeed * stof(buffer);
        stream >> buffer;   rotationSpeed = simulationSpeed * stof(buffer);
        stream >> buffer;   size          = scale           * stof(buffer);
        stream >> buffer;   numMoons      = stof(buffer);

        planets[i] = new Planet(model, lat_radius, long_radius, orbitSpeed, rotationSpeed, size, numMoons);
        i++;
    }

    planetTextures[0].Load("Assets/planets/mercurymap.jpg");
    planetTextures[1].Load("Assets/planets/venusmap.jpg");
    planetTextures[2].Load("Assets/planets/earthmap1k.jpg");
    planetTextures[3].Load("Assets/planets/marsmap1k.jpg");
    planetTextures[4].Load("Assets/planets/jupitermap.jpg");
    planetTextures[5].Load("Assets/planets/saturnmap.jpg");
    planetTextures[6].Load("Assets/planets/uranusmap.jpg");
    planetTextures[7].Load("Assets/planets/neptunemap.jpg");
    planetTextures[8].Load("Assets/planets/plutomap1k.jpg");
    planetTextures[9].Load("Assets/planets/moonmap2k.jpg");

}

void IntroState::Finalize()
{
    // Free resources
    model->Free();
    asteroid->Free();
    shaderProgram.DeleteProgram();
    delete camera;
    camera = NULL;
    delete skybox;
    skybox = NULL;

    for(int i = 0; i < 9; i++)
    {
        planetTextures[i].Free();
        delete planets[i];
        planets[i] = NULL;
    }
}

void IntroState::HandleEvents()
{
}

void IntroState::Update()
{
    // Update logic
    camera->Update();

    for(int i = 0; i < 9; i++)
    {
        planets[i]->Update();
    }

    shaderProgram.SetUniform("sampler2D", 0);
}

void IntroState::Draw()
{
    // Render logic
    glClearColor(0.0, 0.0, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox->Draw(camera->projectionMatrix, camera->viewMatrix);

    shaderProgram.UseProgram();
    float sunScale = 600.0f;
    glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(sunScale, sunScale, sunScale));

    shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(modelMatrix));
    sun->Draw();

    for (GLint i = 0; i < 9; i++)
    {
        planetTextures[i].Bind(0);
        planets[i]->Draw(shaderProgram, camera, sunTexture);//planetTextures[9] );
    }

    for(GLuint i = 0; i < 600; i++)
    {
        shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(modelMatrices[i]));
        asteroid->Draw();
    }

}

