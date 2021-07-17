#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	//Remember check out your OPGL`s edition!!!
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLEW_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Open GLFW Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OPGL Game", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "window creat failed!";
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	//Init GLEW
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW Init failed!";
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glViewport(0, 0, 800, 600);

	//==========================================
	//creat VAO & VBO & EBO & VertexShader & FragmentShader

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Creat VAO & VBO
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 // 0.5f, -0.5f, 0.0f,
	 //-0.5f,  0.5f, 0.0f,//just need by VAO.
	  0.5f,  0.5f, 0.0f
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Creat EBO
	unsigned int indices[] = {
		3,2,1,
		0,1,2
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//VertexShader coding here.
	const char* vertexShaderSource =
		"#version 460 core                                      \n "
		"layout (location = 0) in vec3 position;                \n "
		"                                                       \n "
		"out vec4 vertexColor;                                  \n "
		"                                                       \n "
		"void main()                                            \n "
		"{                                                      \n "
		"    gl_Position = vec4(position, 1.0);                 \n "
		"    vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);        \n "
		"}                                                      \n ";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	//How coding ends?
	int success_vertexShader;
	char infoLog_vertexShader[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_vertexShader);
	if (!success_vertexShader)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog_vertexShader);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog_vertexShader << std::endl;
	}
	else
	{
		std::cout << "vertexShader built success!\n" << std::endl;
	}

	//FragmentShader coding here.
	const char* fragmentShaderSource =
		"#version 460 core                               \n "
		"in vec4 vertexColor;                            \n "
		"                                                \n "
		"out vec4 FragColor;                             \n "
		"                                                \n "
		"void main()                                     \n "
		"{                                               \n "
		"    FragColor = vertexColor;                    \n "
		"}                                               \n ";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	//How coding ends?
	int success_fragmentShader;
	char infoLog_fragmentShader[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_fragmentShader);
	if (!success_fragmentShader)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog_fragmentShader);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog_fragmentShader << std::endl;
	}
	else
	{
		std::cout << "fragmentShader built success!\n" << std::endl;
	}

	//Program both shader.
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//How programing ends?
	int success_programing;
	char infoLog_programing[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_programing);
	if (!success_programing)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog_programing);
		std::cout << "ERROR::SHADER::VERTEX&FRAGMENT::PROGRAM_02_FAILED\n" << infoLog_programing << std::endl;
	}
	else
	{
		std::cout << "Shader Program_02 success!\n" << std::endl;
	}

	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//==========================================

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		//Clear the window
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//R,G,B,alpha
		glClear(GL_COLOR_BUFFER_BIT);

		//==========================================

		//uniform color check

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glUseProgram(shaderProgram);
		//glDrawArrays(GL_TRIANGLES, 0, 6);//Work without VAO.
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)4);

		//==========================================

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}