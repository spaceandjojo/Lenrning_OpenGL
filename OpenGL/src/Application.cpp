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

		GLCall(glEnable(GL_BLEND)); //启用混合
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置混合属性

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));// 创建并绑定顶点缓冲对象 (VBO)

		VertexBufferLayout layout;
		//顶点属性的组件数
		layout.Push<float>(2);
		layout.Push<float>(2);//增加纹理坐标
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);// 创建并绑定索引缓冲对象 (IBO)

		glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);//创建正交矩阵
		
		Shader shader("res/shaders/Basic.shader"); //指定相对路径，相对于项目目录
		shader.Bind(); //通过使用实际的着色器id即shader编写glUseProgram来绑定着色器
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f); //获取这个实际颜色变量u_Color的位置
		shader.SetUniformMat4f("u_MVP", proj);

		//解绑定所有缓冲对象 在OpenGL中解绑不是严格要求的，如果对调试有用的话可以解绑，当一般情况下这只是浪费性能
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

		// 在作用域外Shader类的析构函数会自动调用
	}

	glfwTerminate();
	return 0;
}