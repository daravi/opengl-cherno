#pragma once

#include "TestBase.h"

namespace test
{
	class TestCustomQuad_Color : public TestBase
	{
	private:
		float m_clearColor[4];
	public:
		TestCustomQuad_Color();
		~TestCustomQuad_Color() override;

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	};
}