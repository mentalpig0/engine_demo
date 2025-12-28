#version 330 core
layout (location = 0) in vec3 aPos;   // position attribute
layout (location = 1) in vec3 aColor; // color attribute

out vec3 ourColor; // output color to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor; // pass color to fragment shader
}
