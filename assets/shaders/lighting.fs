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
    
    // High gradient = sharper edges = more occlusion
    //float edgeFactor = smoothstep(0.0, 0.05, length(fwidth(N)));
    //float ao = 1.0 - edgeFactor * (1.0 - dotNL);
    //return lighting * ao;
    // My "poor man's AO" is too harsh...
    // It adds detail to the mechs, but also adds unwanted seams to building bases.
    // Shadows + low-lighting areas conceal most of the AO artifacts, but I'm still hesitant to apply this AO.

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

#define SHADOW_MAP_RES 4096
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

    // Less bias needed when surfaces point towards the light as depth is most accurate
    float idotNL = 1.0 - max(dot(N, -lights[0].direction), 0.0);
    float bias = max(0.002 * idotNL, 0.001);
    
    vec4 lightSpace = lightViewProj * vec4(fragPosition + N * 0.01, 1.0);
    lightSpace.xyz /= lightSpace.w;
    lightSpace.xyz = (lightSpace.xyz + 1.0) * 0.5;
    float currentDepth = lightSpace.z - bias;

    int shadowCount = 0;
    vec2 texelSize = vec2(1.0 / float(SHADOW_MAP_RES));
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float shadowDepth = texture(texture1, lightSpace.xy + texelSize * vec2(x, y)).r;
            if (currentDepth > shadowDepth)
            {
                shadowCount++;
            }
        }
    }

    float shadowAlpha = float(shadowCount) / 9.0;
    vec4 shadowColor = vec4(finalColor.xyz * 0.4, 1.0);
    finalColor = mix(finalColor, shadowColor, shadowAlpha);

    // Without PCF (hard shadows look worse due to aliasing)
    //float shadowDepth = texture(texture1, lightSpace.xy).r;
    //float shadowAlpha = currentDepth > shadowDepth ? 1.0 : 0.0;

    // Normals rate-of-change heatmap:
    //finalColor = vec4(vec3(1.0) * length(fwidth(N)), 1.0);

    // Gamma-correction:
    //finalColor = pow(finalColor, vec4(1.0/2.2));

    // (Fake) ambient occlusion visualization
    //vec3 L = normalize(-lights[0].direction - fragPosition);
    //float dotNL = max(0.0, dot(N, L));
    //float edgeFactor = smoothstep(0.0, 0.05, length(fwidth(N)));
    //float ao = 1.0 - edgeFactor * (1.0 - dotNL);
    //finalColor = vec4(vec3(1.0) * ao, 1.0);
}
