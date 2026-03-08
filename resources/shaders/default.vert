#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform vec3 OffsetShit;

out vec4 vertexColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x + OffsetShit.x, aPos.y + OffsetShit.y, aPos.z + OffsetShit.z, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}
