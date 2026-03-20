#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform vec3 vecColor;

void main()
{
    FragColor = vertexColor * vec4(vecColor,1);
}
