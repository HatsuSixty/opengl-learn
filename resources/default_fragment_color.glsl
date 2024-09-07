#shader vertex
#version 330 core

// We are using vec4 here because OpenGL knows it's
// actually a vec2 and will convert it for us.
// The location is the index we setup earlier.
layout(location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

// Color parameter set in the main program.
uniform vec4 u_Color;

void main()
{
    color = u_Color;
}