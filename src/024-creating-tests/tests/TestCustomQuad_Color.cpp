#include "TestCustomQuad_Color.h"

#include <GL/glew.h>
#include <imgui.h>

namespace test
{
	TestCustomQuad_Color::TestCustomQuad_Color()
		: m_clearColor{ 0.2f, 0.3f, 0.8f, 1.0f } // blue
	{
	}

	TestCustomQuad_Color::~TestCustomQuad_Color()
	{
	}

	void TestCustomQuad_Color::onUpdate(float deltaTime)
	{
	}
	
	void TestCustomQuad_Color::onRender()
	{
		glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	void TestCustomQuad_Color::onImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_clearColor);
	}
}