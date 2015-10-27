attribute vec4 vertex;
varying vec2 TexCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * vec4(vertex.xy, 0.0f, 1.0f);
    TexCoords = vertex.zw;
}
