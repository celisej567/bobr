#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; 
in vec3 cameraPos;

uniform sampler2D ourTexture;
uniform vec3 lightPos; 

float specularStrength = 0.5;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    float diffuse = max(dot(normal, lightDir), 0.0);

    // wont be so dark
    float ambient = 0.15;

    float specular = specularStrength * spec;  
    float lighting = ambient + diffuse + specular;

    vec4 texColor = texture(ourTexture, TexCoord);

    FragColor = texColor * vertexColor * lighting;
}
