#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, // 0
		 0.5f, -0.5f, 1.0f, 0.0f,// 1
		 0.5f,  0.5f, 1.0f, 1.0f,// 2
		-0.5f,  0.5f, 0.0f, 1.0f// 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND)); //���û��
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//���û������

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));// �������󶨶��㻺����� (VBO)

		VertexBufferLayout layout;
		//�������Ե������
		layout.Push<float>(2);
		layout.Push<float>(2);//������������
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);// ������������������� (IBO)

		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);//������������
		
		Shader shader("res/shaders/Basic.shader"); //ָ�����·�����������ĿĿ¼
		shader.Bind(); //ͨ��ʹ��ʵ�ʵ���ɫ��id��shader��дglUseProgram������ɫ��
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f); //��ȡ���ʵ����ɫ����u_Color��λ��
		shader.SetUniformMat4f("u_MVP", proj);

		//������л������ ��OpenGL�н�����ϸ�Ҫ��ģ�����Ե������õĻ����Խ�󣬵�һ���������ֻ���˷�����
		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.01f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			//shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			Texture texture("res/textures/Spike.png");
			texture.Bind();
			shader.SetUniform1i("u_Texture", 0); 

			renderer.Draw(va, ib, shader);

			if (r > 1.0f)
				increment = -0.01f;
			else if (r < 0.0f)
				increment = 0.01f;

			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		// ����������Shader��������������Զ�����
	}

	glfwTerminate();
	return 0;
}