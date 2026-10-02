#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    bool useSpecular; // HACKY. i dont like that.
    sampler2D specular;
    float shininess;
};

uniform Material material;

in vec4 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec3 cameraPos;

uniform vec3 lightPos;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(cameraPos - FragPos);

    vec4 texColor = texture(material.diffuse, TexCoord);
    vec4 texColorSpec = vec4(1,1,1,1);

    vec4 albedo = texColor * vertexColor;

    if(material.useSpecular)
        texColorSpec = texture(material.specular, TexCoord);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = material.ambient * albedo.xyz;
    vec3 specular = spec * texColorSpec.xyz;
    vec4 diffuse  = albedo * diff;
    //vec3 diffuse = vec3(1,1,1) * diff;

    //vec3 specular = texColorSpec.xyz * spec;
    //vec3 lighting = material.ambient + diffuse + specular;

    FragColor = diffuse + vec4(ambient + specular, 0);
}
