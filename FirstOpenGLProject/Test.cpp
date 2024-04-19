#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void proccessInput(GLFWwindow* window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool wireframe = false;
bool released = true;

const char* vertexShaderSourse = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* orangeFragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

const char* yellowFragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.9f, 0.0f, 1.0f);\n"
	"}\n\0";


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

	// Build and compiile the shaders
	// ------------------------------
	// Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourse, NULL);
	glCompileShader(vertexShader);
	// Check if the vertex shader compiled successfully
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shaders
	// ----------------
	// Orange fragment shader
	unsigned int orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSource, NULL);
	glCompileShader(orangeFragmentShader);
	// Check if the orange fragment shader compiled successfully
	glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(orangeFragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::ORANGE_FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Yellow fragment shaders
	unsigned int yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
	glCompileShader(yellowFragmentShader);
	// Check if the yellow fragment shader compiled successfully
	glGetShaderiv(yellowFragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(yellowFragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::YELLOW_FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	// ------------
	// orange shader
	unsigned int orangeShaderProgram = glCreateProgram();
	glAttachShader(orangeShaderProgram, vertexShader);
	glAttachShader(orangeShaderProgram, orangeFragmentShader);
	glLinkProgram(orangeShaderProgram);
	// Check if the shader program linked succesfully
	glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(orangeShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::ORANGE_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// yellow shader
	unsigned int yellowShaderProgram = glCreateProgram();
	glAttachShader(yellowShaderProgram, vertexShader);
	glAttachShader(yellowShaderProgram, yellowFragmentShader);
	glLinkProgram(yellowShaderProgram);
	// Check if the shader program linked succesfully
	glGetProgramiv(yellowShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(yellowShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::YELLOW_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// delete the shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(orangeFragmentShader);
	glDeleteShader(yellowFragmentShader);

	// Set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// triangle in Normalized Devize Coordinates

	float leftVerticies[] = {
		// left triangle
		-1.0, -0.5, 0.0f,  // left
		0.0f, -0.5f, 0.0f, // right
		-0.5f, 0.5f, 0.0f, // top 
	};

	float rightVerticies[] = {
		// right triangle
		0.0f, -0.5f, 0.0f,  // left
		1.0f, -.5f, 0.0f, // right
		0.5f, 0.5f, 0.0f // top
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
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
		glUseProgram(yellowShaderProgram);

		// bind and draw the left triangle 
		glBindVertexArray(VAOLeft);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(orangeShaderProgram);

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
	glDeleteProgram(orangeShaderProgram);
	glDeleteProgram(yellowShaderProgram);

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
