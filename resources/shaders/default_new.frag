#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material material;

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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse =  material.diffuse * diff;

    vec3 specular = material.specular * spec;  
    vec3 lighting = material.ambient + diffuse + specular;

    vec4 texColor = texture(ourTexture, TexCoord);

    FragColor = texColor * vertexColor * vec4(lighting,1);
}
