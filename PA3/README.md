# Moons
----
In this project, I extend the functionality of PA2 by adding a moon that orbits the initial planet, as well as text to indicate the direction the planet is rotating.

### Notes

I implemented the extra credit for this assignment. I used fixed-function pipeline raster functions for quick and dirty text rendering, but will create a proper text engine in the future. I had to use:

    #include <Gl/freeglut.h>

because glutBitmapString() is not available in GLUT alone. The code for rendering the extra credit text is shown below:

     // Render text with fixed-function pipeline calls
    glUseProgram(0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, mainProgram->GetScreenWidth() - 1.0, 0.0, mainProgram->GetScreenHeight() - 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(10,10);
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string text = (reversed) ? "Clockwise" : "Counter-Clockwise";
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text.c_str());

### The Moon

Adding the moon simply required a seperate model matrix, different transformations, and a second call to glDrawArrays(). I also used a second MVP matrix for convenience, but this is not necessary. The first MVP can simply be overwritten to achieve the same effect. The code for this is shown below:

        glm::mat4 moonMVP;
        glm::mat4 moonModelMatrix;

Then I transform these matrices in Update(). The moon is translated around the planet in a circle by using planetModelMatrix as the first parameter in glm::translate():

    planetModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0 * sin(translationAngle), 0.0, 4.0 * cos(translationAngle)));
    moonModelMatrix = glm::translate(planetModelMatrix, glm::vec3(3.0 * cos(4 * moonAngle), 0.0, 3.0 * sin(4 * moonAngle)));
    moonModelMatrix = glm::scale(moonModelMatrix, glm::vec3(0.2, 0.2, 0.2));
    planetModelMatrix = glm::rotate(planetModelMatrix, 4*rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

Then finally in Render():

    planetMVP = projection * view * planetModelMatrix;
    moonMVP = projection * view * moonModelMatrix;
    cubeVAO.Bind();
    shaderProgram.UseProgram();
    shaderProgram.SetUniform("mvpMatrix", &planetMVP);
    glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count
    shaderProgram.SetUniform("mvpMatrix", &moonMVP);
    glDrawArrays(GL_TRIANGLES, 0, 36);//mode, starting index, count
 
