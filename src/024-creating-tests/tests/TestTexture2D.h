#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "TestBase.h"

class IndexBuffer;
class Renderer;
class Shader;
class Texture;
class VertexArray;

namespace test
{
	class TestTexture2D : public TestBase
	{
	public:
		TestTexture2D();
		~TestTexture2D() override;

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		std::unique_ptr<VertexArray> m_va{ nullptr };
		std::unique_ptr<IndexBuffer> m_ib{ nullptr };
		std::unique_ptr<Shader> m_shader{ nullptr };
		std::unique_ptr<Renderer> m_renderer{ nullptr };
		std::unique_ptr<Texture> m_texture{ nullptr };
		glm::mat4 m_vp;
		glm::vec3 m_translationA;
		glm::vec3 m_translationB;
	};
}