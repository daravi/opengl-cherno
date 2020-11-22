#pragma once

namespace test
{
	class TestBase
	{
	public:
		TestBase() = default;
		virtual ~TestBase() = default;

		virtual void onUpdate(float deltaTime) = 0;
		virtual void onRender() = 0;
		virtual void onImGuiRender() = 0;
	};
}