#pragma once

#include "TestBase.h"

namespace test
{
	class TestCustomQuad_Uniform : public TestBase
	{
	private:
		float m_clearColor[4];
	public:
		TestCustomQuad_Uniform();
		~TestCustomQuad_Uniform() override;

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	};
}