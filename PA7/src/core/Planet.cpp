#include "Planet.h"

Planet::Planet(Model *model, float r_la, float r_lo, float oSpeed, float rSpeed, float fSize, int numMoons)
{
    modelData = model;

    lat_radius = r_la;
    long_radius= r_lo;
    orbitSpeed = oSpeed;
    rotationSpeed = rSpeed;
    size = fSize;

    angleO = (random() % 360) * M_PI / 180;
    angleR = (random() % 360) * M_PI / 180;

    modelMatrix = glm::mat4(1.0f);

    for(int i = 0; i < numMoons; i++)
    {
        moonModelMatrices.push_back(glm::mat4(1.0f));
    }
}

void Planet::Draw(ShaderProgram &shaderProgram, Camera *camera, Texture moonTexture)
{
    // modelMatrix has to be set
    // moonModelMatrices have to be set
    shaderProgram.UseProgram();
    shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(modelMatrix));
    modelData->Draw();

    for(GLint i = 0; i < moonModelMatrices.size(); i++)
    {
        moonTexture.Bind(0);
        shaderProgram.SetUniform("mvpMatrix", camera->GetMVP(moonModelMatrices[i]));
        modelData->Draw();
    }

    shaderProgram.DisUseProgram();
}

void Planet::Update()
{
    angleO += orbitSpeed;
    angleR += rotationSpeed;

    if(angleO > 2*M_PI)
        angleO = 0;

    if(angleR > 2*M_PI)
        angleR = 0;

    // orbit
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(lat_radius*cos(angleO), 0.0f, long_radius*sin(angleO)));

   // scale
    modelMatrix = glm::scale( modelMatrix, glm::vec3(size) );

    // spin
    modelMatrix = glm::rotate( modelMatrix, angleR, glm::vec3(0.0f,1.0f,0.0f) );

}

void Planet::SetModelMatrix(glm::mat4 matrix)
{
    modelMatrix = matrix;
}

glm::mat4 Planet::GetModelMatrix()
{
    return modelMatrix;
}

