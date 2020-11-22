#include "TestMenu.h"

#include <imgui.h>

namespace test
{
	TestMenu::~TestMenu()
	{
		clearTest();
	}

	void TestMenu::onImGuiRender()
	{
		for (auto& [test_name, test_generator] : m_tests)
		{
			if (ImGui::Button(test_name.c_str()))
			{
				m_currentTest = test_generator();
				m_currentTestName = test_name;
			}
		}
	}

	void TestMenu::clearTest()
	{
		if (m_currentTest)
		{
			delete m_currentTest;
			m_currentTest = nullptr;
		}
		m_currentTestName = "";
	}

	TestBase* TestMenu::currentTest()
	{
		return m_currentTest;
	}

	const std::string& TestMenu::currentTestName() const
	{
		return m_currentTestName;
	}
}