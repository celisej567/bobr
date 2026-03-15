#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;
uniform float mixAmount;

void main()
{
    //FragColor = vec4(0.f, 0.f, 0.f, 1.0f);
    //FragColor = vertexColor;

    FragColor = mix(texture(ourTexture, TexCoord), texture(texture2, TexCoord), mixAmount) * vertexColor;
}
