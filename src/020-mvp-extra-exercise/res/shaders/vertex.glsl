#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord; // v_ for varying, the process of sending data between shaders

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

/**
 * Side note -- MVP
 * ----------------
 * - Model transformation:      Any transformation applied to the model object (e.g. rotation, translation, scale)
 * - View transformation:       Used to simulate a camera (i.e. transformation applied to every object in the scene)
 * - Projection transformation: Projects vertex positions (3D) into final device coordinate (2D application window)
 *       -> Projection types: Orthographic (2D) vs Perspective (3D, i.e. size depends on z and simulates depth)
 * 
 * Note: In column-major ordering (e.g. in OpenGL), the final transformation matrix is obtained
 *       by this multiplication order: P * V * M (as opposed to M * V * P in row-major ordering)
 */

void main()
{
	// normalized device coordinates (coordinates between -1, +1) is rasterized to our display window
	gl_Position = u_projection * u_view * u_model * position;
	v_texCoord = texCoord;
}
