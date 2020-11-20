#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord; // v_ for varying, the process of sending data between shaders

uniform mat4 u_projection; // projection matrix of the model/view/projection matrices

void main()
{
	gl_Position = u_projection * position;
	v_texCoord = texCoord;
}
