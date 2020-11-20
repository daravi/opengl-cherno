/**
 * Fragment shader code: (also known as pixel shader)
 *  - This shader is responsible for determining the color of each pixel
 *  - It is called for each individual pixel so be cautious
 */
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.2, 0.3, 0.8, 1.0);
}
