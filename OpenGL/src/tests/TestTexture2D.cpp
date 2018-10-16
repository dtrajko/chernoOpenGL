#include "TestTexture2D.h"

namespace test {

	TestTexture2D::TestTexture2D() :
		WIDTH(960),
		HEIGHT(540),
		m_translationA(WIDTH / 3 * 1, HEIGHT / 2, 0),
		m_translationB(WIDTH / 3 * 2, HEIGHT / 2, 0),
		m_Proj(glm::ortho(0.0f, WIDTH, 0.0f, HEIGHT, -1.0f, 1.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
	{
		float positions[] = {
			-50.5f, -50.0f, 0.0f, 0.0f, // 0
			 50.5f, -50.0f, 1.0f, 0.0f, // 1
			 50.5f,  50.0f, 1.0f, 1.0f, // 2
			-50.5f,  50.0f, 0.0f, 1.0f, // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_VAO = std::make_unique<VertexArray>();

		// 4 vertices x 4 floats per vertex:
		// 2 floats for position, 2 floats for texture coordinates (texCoord in vertex shader)
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		m_VAO->AddBuffer(* m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D() {

	}

	void TestTexture2D::OnUpdate(float deltaTime) {

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
			renderer.Draw(* m_VAO, * m_IndexBuffer, * m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
			renderer.Draw(* m_VAO, * m_IndexBuffer, * m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat("TranslationA X", &m_translationA.x, 0.0f, WIDTH);
		ImGui::SliderFloat("TranslationA Y", &m_translationA.y, 0.0f, HEIGHT);
		ImGui::SliderFloat("TranslationB X", &m_translationB.x, 0.0f, WIDTH);
		ImGui::SliderFloat("TranslationB Y", &m_translationB.y, 0.0f, HEIGHT);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
