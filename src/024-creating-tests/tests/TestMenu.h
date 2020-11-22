#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "TestBase.h"

namespace test
{
	class TestMenu : public TestBase
	{
	private:
		TestBase* m_currentTest{ nullptr };
		std::string m_currentTestName{ "" };
		std::vector<std::pair<std::string, std::function<TestBase*()>>> m_tests;

	public:
		TestMenu() = default;
		~TestMenu();

		void onImGuiRender() override;

		void clearTest();
		TestBase* currentTest();
		const std::string& currentTestName() const;

		template<typename T>
		void registerTest(std::string name)
		{
			m_tests.push_back(std::make_pair(std::move(name), [](){ return new T(); }));
		}
	};
}
