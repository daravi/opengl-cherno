/**
 * Fragment shader code: (also known as pixel shader)
 *  - This shader is responsible for determining the color of each pixel
 *  - It is called for each individual pixel so be cautious
 */
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color; // u_ for uniform

void main()
{
	color = u_color;
}
