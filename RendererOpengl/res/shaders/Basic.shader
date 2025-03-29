#shader vertex

#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inColor;

out vec3 fragColor;

uniform float u_Time;
void main()
{
    mat3 rotX = mat3(
    1.0,       0.0,        0.0,
    0.0,  cos(u_Time), -sin(u_Time),
    0.0,  sin(u_Time),  cos(u_Time)
    );

    mat3 rotY = mat3(
    cos(u_Time),       0.0,        -sin(u_Time),
    0.0,               1.0,                 0.0,
    sin(u_Time),       0.0,        cos(u_Time)
    );

    float offest = sin(u_Time) * 0.5f;
    gl_Position = vec4(vec3(position.x /*+ offest*/, position.y, position.z) * rotX * rotY, 1.0);
    fragColor = inColor;
}
#shader fragment
#version 460 core

in vec3 fragColor;
out vec4 color;

void main()
{
    color = vec4(fragColor, 1.0);
}