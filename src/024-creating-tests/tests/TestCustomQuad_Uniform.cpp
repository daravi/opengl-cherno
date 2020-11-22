#include "TestCustomQuad_Uniform.h"

#include <GL/glew.h>
#include <imgui.h>

namespace test
{
	TestCustomQuad_Uniform::TestCustomQuad_Uniform()
		: m_clearColor{ 0.2f, 0.3f, 0.8f, 1.0f } // blue
	{
	}

	TestCustomQuad_Uniform::~TestCustomQuad_Uniform()
	{
	}

	void TestCustomQuad_Uniform::onUpdate(float deltaTime)
	{
	}
	
	void TestCustomQuad_Uniform::onRender()
	{
		glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	void TestCustomQuad_Uniform::onImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_clearColor);
	}
}