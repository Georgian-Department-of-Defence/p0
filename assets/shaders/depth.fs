#version 430

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D tex_depth;

uniform float z_near;
uniform float z_far;

out vec4 finalColor;

void main()
{
    float zNear = z_near;
    float zFar = z_far;
    float z = texture(tex_depth, fragTexCoord).x;

    // Linearize depth value
    float depth = (2.0 * zNear) / (zFar + zNear - z * (zFar - zNear));

    finalColor = vec4(depth, depth, depth, 1.0f);
}
