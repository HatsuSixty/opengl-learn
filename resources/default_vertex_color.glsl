#shader vertex
#version 330 core

// We are using vec4 here because OpenGL knows it's
// actually a vec2 and will convert it for us.
// The location is the index we setup earlier.
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

// Output to fragment shader
out vec4 vertexColor;

void main()
{
   gl_Position = position;
   vertexColor = color;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

// Input from vertex shader
in vec4 vertexColor;

void main()
{
   color = vertexColor;
}