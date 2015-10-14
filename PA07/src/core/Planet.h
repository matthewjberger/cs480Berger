#ifndef __PLANET_H
#define __PLANET_H

#include "GlobalIncludes.h"
#include "Model.h"
#include "Camera.h"
#include "Game.h"

class Planet
{
    public:
        Planet(Model *model, float r_la, float r_lo, float oSpeed, float rSpeed, float fSize, int numMoons);
        ~Planet(){};

        void Draw(ShaderProgram &shaderProgram, Camera *camera, Texture moonTexture);
        void Update();

        void SetModelMatrix(glm::mat4 matrix);
        glm::mat4 GetModelMatrix();

    private:
        vector<glm::mat4> moonModelMatrices;

        glm::mat4 modelMatrix;
        Model* modelData;

        float lat_radius;
        float long_radius;
        float orbitSpeed;
        float rotationSpeed;
        float size;
        float angleR;
        float angleO;
};

#endif
