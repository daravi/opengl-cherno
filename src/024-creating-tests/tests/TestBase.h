#pragma once

namespace test
{
	class TestBase
	{
	public:
		TestBase() = default;
		virtual ~TestBase() = default;

		virtual void onUpdate(float deltaTime) {}
		virtual void onRender() {}
		virtual void onImGuiRender() {}
	};
}