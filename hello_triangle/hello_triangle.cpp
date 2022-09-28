#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 将顶点着色器的源代码硬编码在代码文件顶部
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// 将片段着色器的源代码硬编码在代码文件顶部
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {
	glfwInit(); // 初始化GLFW
	// 使用glfwWindowHint配置GLFW
	// 选项与值：https://www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 将主版本号和次版本号设为3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "08019221 Yuanze Xu", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// 告诉GLFW希望每当窗口调整大小的时候调用framebuffer_size_callback函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	// 在调用任何OpenGL的函数前需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	// 编译顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // 创建着色器对象
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 将着色器源码附着在着色器对象上
	glCompileShader(vertexShader); // 编译

	// check for shader compile errors
	// 检测编译错误
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	// 编译片段着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // 与顶点着色器类似
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	// 检测编译错误
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	// 将两个着色器对象链接到用来渲染的着色器程序中
	unsigned int shaderProgram = glCreateProgram(); // 创建程序对象
	// 将之前编译的着色器附加到程序对象上
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram); // 链接着色器

	// check for linking errors
	// 检测链接着色器程序是否失败
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// 删除着色器对象（不再需要了）
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	// 以标准化设备坐标形式定义float数组，储存三角形顶点坐标。
	// 因为是2D三角形，所以z（深度）被设置为0。
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO); // 生成顶点数组对象VAO
	glGenBuffers(1, &VBO); // 生成顶点缓冲对象VBO，用于管理内存

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	// good step1 绑定VAO
	glBindVertexArray(VAO);
	// bad step1 复制顶点数组到缓冲中供OpenGL使用
	// good step2 复制顶点数组到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把之前定义的顶点数据复制到缓冲的内存中

	// bad step2 设置顶点属性指针
	// good step3 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 告诉OpenGL如何解析顶点数据
	glEnableVertexAttribArray(0); // 以顶点属性位置值作为参数，启用顶点属性

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	// 希望程序在我们主动关闭它之前不断绘制图像并能够接受用户输入，因此需要在程序中添加while循环。
	// 在每次循环开始前检查一次GLFW是否被要求退出
	while (!glfwWindowShouldClose(window)) {
		// input
		// -----
		processInput(window); // 检查输入

		// render
		// ------
		// 渲染指令
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 设置清空屏幕所用颜色
		glClear(GL_COLOR_BUFFER_BIT); // 清空屏幕颜色缓冲

		// draw our first triangle
		// bad step3 当渲染一个物体时要使用着色器程序
		// good step4 绘制物体
		glUseProgram(shaderProgram); // 用创建的程序对象作为它的参数，以激活程序对象。每个着色器调用和渲染调用都会使用这个程序对象。
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3); // 使用当前激活的着色器、之前定义的顶点属性配置、VBO的顶点数据（通过VAO间接绑定）来绘制图元
		// glBindVertexArray(0); // no need to unbind it every time 

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window); // 交换颜色缓冲
		glfwPollEvents(); // 检查有没有触发什么事件
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate(); // 正确释放/删除之前的分配的所有资源
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// 输入函数。在每个循环中调用。
void processInput(GLFWwindow* window) {
	// 检查Esc键是否被按下
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true); // 设置WindowShouldClose属性为true
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
// 当用户改变窗口的大小时调整视口。函数需要注册。
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height); // 设置窗口维度。也可以将视口的维度设置为比GLFW的维度小。
}