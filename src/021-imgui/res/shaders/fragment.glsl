/**
 * Fragment shader code: (also known as pixel shader)
 *  - This shader is responsible for determining the color of each pixel
 *  - It is called for each individual pixel so be cautious
 */
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform sampler2D u_texture; // active texture slot number

void main()
{
	// Telling fragment shader to set the color of the pixel by sampling the texture at a specifc coordinate
	vec4 texColor = texture(u_texture, v_texCoord);
	color = texColor;
}
