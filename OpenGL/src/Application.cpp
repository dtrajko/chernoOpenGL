#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{ // create a scope

		float positions[] = {
			100.0f, 100.0f, 0.0f, 0.0f, // 0
			200.0f, 100.0f, 1.0f, 0.0f, // 1
			200.0f, 200.0f, 1.0f, 1.0f, // 2
			100.0f, 200.0f, 0.0f, 1.0f, // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		// 4 vertices x 4 floats per vertex:
		// 2 floats for position, 2 floats for texture coordinates (texCoord in vertex shader)
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100, 0, 0));
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(50, 300, 0));
		model *= glm::rotate(glm::mat4(1.0f), -45.0f, glm::vec3(0, 0, 1));
		model *= glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 0));

		glm::mat4 mvp = proj * view * model;

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture texture("res/textures/ChernoLogo.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

		float camera_x = 480;
		float camera_x_offset = 1.0f;
		float camera_y = 270;
		float camera_y_offset = -1.0f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();
			shader.Bind();
			renderer.Draw(va, ib, shader);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	} // end the scope

	glfwTerminate();
	return 0;
}
