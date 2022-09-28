#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ��������ɫ����Դ����Ӳ�����ڴ����ļ�����
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n" // λ�ñ���������λ��ֵΪ0
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//// ��Ƭ����ɫ����Դ����Ӳ�����ڴ����ļ�����
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//"}\n\0";

// [advance 1] ������ɫ����Ƭ����ɫ���ĸĶ�����Ϊ����ɫ��
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n" // λ�ñ���������λ��ֵΪ0
//"out vec4 vertexColor;\n" // ΪƬ����ɫ��ָ��һ����ɫ���
//"void main()\n"
//"{\n"
//"gl_Position = vec4(aPos, 1.0);\n" // ע��������ΰ�һ��vec3��Ϊvec4�Ĺ������Ĳ���
//"vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // �������������Ϊ����ɫ
//"}\0";

//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//// ����������ͬ��������ͬ��Ƭ����ɫ���е�vertexColor�ͺͶ�����ɫ���е�vertexColor������
//"in vec4 vertexColor;\n" // �Ӷ�����ɫ�����������������������ͬ��������ͬ��
//"void main()\n"
//"{\n"
//"   FragColor = vertexColor;\n"
//"}\n\0";
// end of [advance 1]

// [advance 2] ��ʱ���ɫ
//const char* vertexShaderSource = "#version 330 core\n"
//"layout (location = 0) in vec3 aPos;\n" // λ�ñ���������λ��ֵΪ0
//"out vec4 vertexColor;\n" // ΪƬ����ɫ��ָ��һ����ɫ���
//"void main()\n"
//"{\n"
//"gl_Position = vec4(aPos, 1.0);\n" // ע��������ΰ�һ��vec3��Ϊvec4�Ĺ������Ĳ���
//"vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // �������������Ϊ����ɫ
//"}\0";
//
//const char* fragmentShaderSource = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"uniform vec4 ourColor;\n" // ��OpenGL����������趨�������
//"void main()\n"
//"{\n"
//"   FragColor = ourColor;\n"
//"}\n\0";
// end of [advance 2]

// [advance 3] ����������ɫ����ʹ���ܹ�������ɫֵ��Ϊһ��������������
// ע��Ҫ��layout��ʶ������aColor���Ե�λ��ֵ����Ϊ1
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // λ�ñ���������λ��ֵΪ0
"layout (location = 1) in vec3 aColor;\n" // ��ɫ����������λ��ֵΪ1
"out vec3 ourColor;\n" // ��Ƭ����ɫ�����һ����ɫ
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n" // ע��������ΰ�һ��vec3��Ϊvec4�Ĺ������Ĳ���
"ourColor = aColor;\n" // ��ourColor����Ϊ���ǴӶ�����������õ���������ɫ
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n" // ����ʹ��uniform������Ƭ�ε���ɫ������ʹ��ourColor�������
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";
// end of [advance 3]


int main() {
    glfwInit(); // ��ʼ��GLFW
    // ʹ��glfwWindowHint����GLFW
    // ѡ����ֵ��https://www.glfw.org/docs/latest/window.html#window_hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // �����汾�źʹΰ汾����Ϊ3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ʹ�ú���ģʽ

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // �������ڶ���
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "08019221 Yuanze Xu", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // ����GLFWϣ��ÿ�����ڵ�����С��ʱ�����framebuffer_size_callback����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    // �ڵ����κ�OpenGL�ĺ���ǰ��Ҫ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    // ���붥����ɫ��
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // ������ɫ������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // ����ɫ��Դ�븽������ɫ��������
    glCompileShader(vertexShader); // ����

    // check for shader compile errors
    // ���������
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    // ����Ƭ����ɫ��
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // �붥����ɫ������
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    // ���������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    // ��������ɫ���������ӵ�������Ⱦ����ɫ��������
    unsigned int shaderProgram = glCreateProgram(); // �����������
    // ��֮ǰ�������ɫ�����ӵ����������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // ������ɫ��

    // check for linking errors
    // ���������ɫ�������Ƿ�ʧ��
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // ɾ����ɫ�����󣨲�����Ҫ�ˣ�
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // �Ա�׼���豸������ʽ����float���飬���������ζ������ꡣ
    // ��Ϊ��2D�����Σ�����z����ȣ�������Ϊ0��
    //float vertices[] = {
    //	-0.5f, -0.5f, 0.0f, // left  
    //	 0.5f, -0.5f, 0.0f, // right 
    //	 0.0f,  0.5f, 0.0f  // top   
    //};
    // [advance 3] ����ɫ���ݼӽ����������С�����ɫ�������Ϊ3��floatֵ��vertices����
    float vertices[] = {
        // λ��              // ��ɫ
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
    };
    // end of [advance 3]


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // ���ɶ����������VAO
    glGenBuffers(1, &VBO); // ���ɶ��㻺�����VBO�����ڹ����ڴ�

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    // good step1 ��VAO
    glBindVertexArray(VAO);
    // bad step1 ���ƶ������鵽�����й�OpenGLʹ��
    // good step2 ���ƶ������鵽�����й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // ���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���

    // bad step2 ���ö�������ָ��
    // good step3 ���ö�������ָ��
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // ����OpenGL��ν�����������
    //glEnableVertexAttribArray(0); // �Զ�������λ��ֵ��Ϊ���������ö�������


    // [advance 3] ʹ��glVertexAttribPointer�������¶����ʽ
    // λ������
    // ����ÿ��������˵��λ�ö���������ǰ����������ƫ������0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ��ɫ����
    // ��ɫ���Խ���λ������֮������ƫ��������3 * sizeof(float)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // end of [advance 3]

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    // ϣ�����������������ر���֮ǰ���ϻ���ͼ���ܹ������û����룬�����Ҫ�ڳ��������whileѭ����
    // ��ÿ��ѭ����ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window); // �������

        // render
        // ------
        // ��Ⱦָ��
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // ���������Ļ������ɫ
        glClear(GL_COLOR_BUFFER_BIT); // �����Ļ��ɫ����

        // draw our first triangle
        // bad step3 ����Ⱦһ������ʱҪʹ����ɫ������
        // good step4 ��������
        // ������ɫ��
        glUseProgram(shaderProgram); // �ô����ĳ��������Ϊ���Ĳ������Լ���������ÿ����ɫ�����ú���Ⱦ���ö���ʹ������������

        // [advance 2] ����uniform��ɫ
        // ��ѯuniform��ַ��Ҫ����֮ǰʹ�ù���ɫ�����򣬵��Ǹ���һ��uniform֮ǰ�������ʹ�ó��򣨵���glUseProgram),
        // ��Ϊ�����ڵ�ǰ�������ɫ������������uniform��
        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // ��ѯuniform ourColor��λ��ֵ
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // ����uniformֵ
        // end of [advance 2]

        // ����������
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3); // ʹ�õ�ǰ�������ɫ����֮ǰ����Ķ����������á�VBO�Ķ������ݣ�ͨ��VAO��Ӱ󶨣�������ͼԪ
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        // �������岢��ѯIO�¼�
        glfwSwapBuffers(window); // ������ɫ����
        glfwPollEvents(); // �����û�д���ʲô�¼�
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate(); // ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
// ���뺯������ÿ��ѭ���е��á�
void processInput(GLFWwindow* window) {
    // ���Esc���Ƿ񱻰���
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); // ����WindowShouldClose����Ϊtrue
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
// ���û��ı䴰�ڵĴ�Сʱ�����ӿڡ�������Ҫע�ᡣ
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height); // ���ô���ά�ȡ�Ҳ���Խ��ӿڵ�ά������Ϊ��GLFW��ά��С��
}