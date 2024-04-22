#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool wireframe = false;
bool released = true;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// print out the maximum number of vertex attributes supported by my hardware
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// Create Shader Objects
	Shader yellowShader("Shader.vert", "Orange.frag");
	Shader orangeShader("Shader.vert", "Yellow.frag");

	// Set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// triangle in Normalized Devize Coordinates

	float leftVerticies[] = {
		// left triangle
		-1.0,  -0.5,  0.0f, // left
		 0.0f, -0.5f, 0.0f, // right
		-0.5f,  0.5f, 0.0f  // top 
	};

	float rightVerticies[] = {
		// Positions        // Colors
		0.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // left
		1.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // right
		0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
	};

	/*
	// Rectangle using Element buffer object
	float verticies[] = {
		 0.5f,  0.5,  0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f  // top left
	};

	unsigned int indicies[] = {
		0, 1, 3, // first triangle 
		1, 2, 3  // second triangle 
	};
	*/


	unsigned int VBOLeft, VAOLeft, EBO, VBORight, VAORight;
	glGenVertexArrays(1, &VAOLeft);
	glGenVertexArrays(1, &VAORight);
	glGenBuffers(1, &VBOLeft);
	glGenBuffers(1, &VBORight);
	
	//glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attribure(s)
	// Start setup for left Triangle 
	glBindVertexArray(VAOLeft);

	// Bind VBO and send verticies to the graphics card 
	glBindBuffer(GL_ARRAY_BUFFER, VBOLeft);
	glBufferData(GL_ARRAY_BUFFER, sizeof(leftVerticies), leftVerticies, GL_STATIC_DRAW);

	// Bind EBO and send indicies to the graphics card
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	// tell opengl how to parse the data in the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind the VBO, because the call to glVertexAttripPointer registered VBO as the vertex attributes bound vertex buffer object so afterwords we can safely unbind it 
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Bind the VAO for the right triangle 
	glBindVertexArray(VAORight);

	glBindBuffer(GL_ARRAY_BUFFER, VBORight);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rightVerticies), rightVerticies, GL_STATIC_DRAW);
	
	// position attrubute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);



	// Draw loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		proccessInput(window);

		if (wireframe) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// render
		// ------

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// set the shader program
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		orangeShader.use();
		orangeShader.setFloat("ourColor", greenValue);
		orangeShader.setFloat("offset", .1);
		// bind and draw the left triangle 
		glBindVertexArray(VAOLeft);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		yellowShader.use();
		yellowShader.setFloat("offset", .1);
		//bind and draw the right triangle
		glBindVertexArray(VAORight);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		

		// glfw: swap buffers and poll IO events
		// -------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources once they've outlived their purpose:
	// --------------------------------------------------------------
	glDeleteVertexArrays(1, &VAOLeft);
	glDeleteVertexArrays(1, &VAORight);
	glDeleteBuffers(1, &VBOLeft);
	glDeleteBuffers(1, &VBORight);

	// glfw: terminate, clearing all previousely allocated GLFW resources
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void proccessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && released) {
		wireframe = !wireframe;
		released = false;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		released = true;
	}
}
