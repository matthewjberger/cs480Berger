#include "Model.h"
#include <sstream>
#include <cstdlib>
#include <iostream>

using namespace std;

Model::Model()
{
}

Model::~Model(){}

void Model::LoadModel(std::string path)
{
    ifstream file(path.c_str());
    string line = "";
    file.clear();

    vector<unsigned int> vIndices, uvIndices, nIndices;
    vector<glm::vec3> tempV;
    vector<glm::vec2> tempUV;
    vector<glm::vec3> tempN;

    string textureName = "";

    // Iterate over the file line by line
    while(getline(file, line, '\n'))
    {
        // Get the first token from the line
        string firstToken;
        istringstream ss(line);
        if(!(ss >> firstToken)) { break; }

        // Vertex
        if(firstToken == "v")
        {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            tempV.push_back(vertex);
        }
        // Texture coordinate
        else if(firstToken == "vt")
        {
            glm::vec2 uv;
            ss >> uv.x >> uv.y;
            tempUV.push_back(uv);
        }
        // Normal
        else if(firstToken == "vn")
        {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempN.push_back(normal);
        }
        // Face
        else if(firstToken == "f")
        {
            string first, second, third = "";
            ss >> first >> second >> third;
            cout << first << " " << second << " " << third << endl;

            string fullString = first + "/" + second + "/" + third;
            vector<string> items;
            istringstream tokenizer(fullString);
            for(string each; getline(tokenizer, each, '/'); items.push_back(each));

            vIndices.push_back(stoi(items[0]));
            vIndices.push_back(stoi(items[3]));
            vIndices.push_back(stoi(items[6]));

            //uvIndices.push_back(stoi(items[1]));
            //uvIndices.push_back(stoi(items[4]));
            //uvIndices.push_back(stoi(items[7]));

            //nIndices.push_back(stoi(items[2]));
            //nIndices.push_back(stoi(items[5]));
            //nIndices.push_back(stoi(items[8]));
        }
    }

    cout << "Loaded Vertices: " << endl;
    for(unsigned int i = 0; i < tempV.size(); i++)
    {
        cout << tempV[i].x << " " << tempV[i].y << " " << tempV[i].z << endl;
    }
    cout << endl;

    cout << "Loaded Tex-Coords: " << endl;
    for(unsigned int i = 0; i < tempUV.size(); i++)
    {
        cout << tempUV[i].x << " " << tempUV[i].y << endl;
    }
    cout << endl;

    cout << "Loaded Normals: " << endl;
    for(unsigned int i = 0; i < tempN.size(); i++)
    {
        cout << tempN[i].x << " " << tempN[i].y << " " << tempN[i].z << endl;
    }
    cout << endl;

    for(unsigned int i = 0; i < vIndices.size(); i++)
    {
        unsigned int vertexIndex = vIndices[i];
        glm::vec3 vertex = tempV[vertexIndex - 1];
        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < uvIndices.size(); i++)
    {
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uvCoords = tempUV[uvIndex - 1];
        texCoords.push_back(uvCoords);
    }

    for(unsigned int i = 0; i < nIndices.size(); i++)
    {
        unsigned int normalIndex = nIndices[i];
        glm::vec3 normal = tempN[normalIndex - 1];
        vertices.push_back(normal);
    }

    cout << "Finished loading model successfully!" << endl;
}

std::vector<glm::vec2> Model::GetTexCoords()
{
    return texCoords;
}


std::vector<glm::vec3> Model::GetNormals()
{
    return normals;
}


std::vector<glm::vec3> Model::GetVertices()
{
    return vertices;
}

void Model::Draw()
{
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() );
}

