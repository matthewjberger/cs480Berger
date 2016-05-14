# Blender and Model Loading

In this project, I had to create a board with four walls in Blender, export it to a wavefront .OBJ file, and then create a simple loader to load and display that file.

### Notes

This code is contained in *src/core/Model.h* and *src/core/Model.cpp.*

After being exported, the model for the object looked like this:

    # Blender v2.71 (sub 0) OBJ File: ''
    # www.blender.org
    o Cube_Cube.001
    v -0.166711 0.981912 2.899482
    v -0.166711 0.981912 -0.666013
    v 1.423027 0.981912 -0.666013
    v 1.423027 0.981912 2.899482
    v -0.166711 1.304674 2.899482
    v -0.166711 1.304674 -0.666013
    v 1.423027 1.304674 -0.666013
    v 1.423027 1.304674 2.899482
    vn -1.000000 0.000000 0.000000
    vn 0.000000 0.000000 -1.000000
    vn 1.000000 0.000000 0.000000
    vn 0.000000 0.000000 1.000000
    vn 0.000000 -1.000000 0.000000
    s off
    f 6//1 2//1 1//1
    f 7//2 3//2 2//2
    f 8//3 4//3 3//3
    f 5//4 1//4 4//4
    f 2//5 3//5 4//5
    f 5//1 6//1 1//1
    f 6//2 7//2 2//2
    f 7//3 8//3 3//3
    f 8//4 5//4 4//4
    f 1//5 2//5 4//5

In my model loader, I simply had a class with a LoadModel() function.

I began by creating temporary vectors to hold vertex, texture coordinate, and normal data. There was also a vector to hold indices for when the faces are parsed:
```cpp
    vector<unsigned int> vIndices, uvIndices, nIndices;
    vector<glm::vec3> tempV;
    vector<glm::vec2> tempUV;
    vector<glm::vec3> tempN;
```
I iterate over the file line by line after opening it and check the first character of each line and storing data appropriately:
```cpp
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
```
The last thing to parse is the faces, which have three sets of three numbers delimited by forward slashes. To  parse this, I create a string stream that will hold the three sets concatenated with forward slashes, and then use getline() to iterate over each token in that using '/' as the delimiter:
```cpp
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
```

Next I display all the loaded data for debugging purposes:
```cpp
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
```

And finally I load the data into the proper member vectors in the right order based on the order that the face lines described:
```cpp
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
```
