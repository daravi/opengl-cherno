#pragma once

#include "TestBase.h"

namespace test
{
	class TestClearColor : public TestBase
	{
	private:
		float m_clearColor[4];
	public:
		TestClearColor();
		~TestClearColor() override;

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	};
}