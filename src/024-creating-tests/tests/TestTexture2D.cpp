#include "TestTexture2D.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace test
{
	TestTexture2D::TestTexture2D()
		: m_va(std::make_unique<VertexArray>())
		, m_renderer(std::make_unique<Renderer>())
	{
		constexpr std::array positions{
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f, // 1
			 50.0f,  50.0f, 1.0f, 1.0f, // 2
			-50.0f,  50.0f, 0.0f, 1.0f  // 3
		};

		auto vb = std::make_unique<VertexBuffer>(positions.data(), positions.size() * sizeof(float));
		VertexBufferLayout layout;
		layout.push<float>(2); // attribute index/location 0
		layout.push<float>(2); // attribute index/location 1
		
		m_va->addObject(std::move(vb), std::move(layout));

		constexpr std::array indices{
			0u, 1u, 2u,
			2u, 3u, 0u
		};
		m_ib = std::make_unique<IndexBuffer>(indices.data(), indices.size());

		m_translationA = glm::vec3(200.0f, 200.0f, 0.0f);
		m_translationB = glm::vec3(400.0f, 200.0f, 0.0f);

		glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		
		m_vp = projection * view;

		// You need to run the binary from lesson directory '024-creating-tests' for these relative paths to work
		m_shader = std::make_unique<Shader>("res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
		m_renderer->draw(*m_va, *m_ib, *m_shader);

		m_texture = std::make_unique<Texture>("../../res/images/cherno.png");
		m_texture->bind();
	    m_shader->setUniform1i("u_texture", 0); // should match the bound texture slot

	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::onUpdate(float deltaTime)
	{
	}
	
	void TestTexture2D::onRender()
	{
		m_shader->setUniformMat4f("u_mvp", m_vp * glm::translate(glm::mat4(1.0f), m_translationA));
        m_renderer->draw(*m_va, *m_ib, *m_shader);
        m_shader->setUniformMat4f("u_mvp", m_vp * glm::translate(glm::mat4(1.0f), m_translationB));
        m_renderer->draw(*m_va, *m_ib, *m_shader);
	}
	
	void TestTexture2D::onImGuiRender()
	{
		ImGui::SliderFloat("xA", &m_translationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat("yA", &m_translationA.y, 0.0f, 540.0f);
		ImGui::SliderFloat("xB", &m_translationB.x, 0.0f, 960.0f);
		ImGui::SliderFloat("yB", &m_translationB.y, 0.0f, 540.0f);
	}
}