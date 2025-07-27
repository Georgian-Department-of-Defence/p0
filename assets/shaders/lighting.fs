#version 430

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

struct Light
{
    vec3 position;
    vec3 color;
    
    float ambient;
    float diffuse;
    float specular;
    float specularExponent;
    
    float radius;
    float cutoff;
    vec3 direction;
};

vec3 phong(vec3 P, vec3 N, vec3 cameraPosition, Light light)
{
    // AB = B - A
    // L = vector FROM fragment TO light
    // V = vector FROM fragment TO camera
    // R = vector FROM light TO fragment reflected about the normal
    vec3 L = normalize(light.position - P);
    vec3 V = normalize(cameraPosition - P);
    vec3 R = reflect(-L, N);
    
    // How similar our light direction is to our normal
    float dotNL = max(dot(N, L), 0.0);
    
    // How similar our reflected light direction is to our camera
    float dotVR = max(dot(V, R), 0.0);
    
    vec3 ambient = light.color;
    vec3 diffuse = light.color * dotNL;
    vec3 specular = light.color * pow(dotVR, light.specularExponent);
    
    vec3 lighting = vec3(0.0);
    lighting += ambient * light.ambient;
    lighting += diffuse * light.diffuse;
    lighting += specular * light.specular;
    
    return lighting;
}

vec3 pointLight(vec3 P, vec3 N, vec3 cameraPosition, Light light)
{
    vec3 lighting = phong(P, N, cameraPosition, light);
    float dist = distance(light.position, P);
    float t = smoothstep(light.radius, 0.0, dist);
    return lighting * t;
}

vec3 directionLight(vec3 P, vec3 N, vec3 cameraPosition, Light light)
{
    light.position = -light.direction;
    vec3 lighting = phong(vec3(0.0), N, cameraPosition, light);
    return lighting;
}

vec3 spotLight(vec3 P, vec3 N, vec3 cameraPosition, Light light)
{
    vec3 lighting = vec3(0.0);
    vec3 L = normalize(light.position - P);
    float angle = dot(L, -light.direction);
    if (light.cutoff < angle)
    {
        lighting = pointLight(P, N, cameraPosition, light);
    }
    return lighting;
}

#define MAX_LIGHTS 1
uniform Light lights[MAX_LIGHTS];

uniform sampler2D texture0; // Base color (1x1 white by default)
uniform sampler2D texture1; // Shadow-map

uniform vec3 viewPos;
uniform vec4 colDiffuse;
uniform mat4 lightViewProj;

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 lighting = vec3(0.0);
    for (int i = 0; i < 1; i++)
    {
        lighting += directionLight(fragPosition, N, viewPos, lights[i]);
    }

    vec4 texelColor = texture(texture0, fragTexCoord);
    vec4 tint = colDiffuse * fragColor;
    finalColor = texelColor * tint * vec4(lighting, 1.0);

    float bias = 0.003; // <-- 0.0025 is too small, 0.004 is too large
    bias = max(dot(N, -lights[0].direction) * bias * 2.0, bias);

    vec4 lightSpace = lightViewProj * vec4(fragPosition, 1.0);
    lightSpace.xyz /= lightSpace.w;
    lightSpace.xyz = (lightSpace.xyz + 1.0) * 0.5;

    float shadowDepth = texture(texture1, lightSpace.xy).r;
    float inLight = (lightSpace.z - bias) < shadowDepth ? 1.0 : 0.0;
    vec4 shadowColor = vec4(vec3(1.0) * max(inLight, 0.4), 1.0);

    finalColor = finalColor * shadowColor;
    //finalColor = pow(finalColor, vec4(1.0/2.2));
}
