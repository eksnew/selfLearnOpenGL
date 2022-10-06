#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

//#define _ORIGIN_
//#define _TWO_TEXTURES_
#define _TEST_


int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader zprogram
    // ------------------------------------
#ifdef _ORIGIN_
    Shader ourShader("texture.vertex_shader", "texture.fragment_shader");
#endif // _ORIGIN_

#ifdef _TWO_TEXTURES_
    Shader ourShader("texture.vertex_shader", "texture_with_face.fragment_shader");
#endif // _TWO_TEXTURES_

#ifdef _TEST_
    Shader ourShader("texture.vertex_shader", "texture.fragment_shader");
#endif // _TEST_



    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // ����
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // ���ز���������
    unsigned int texture1, texture2;
    // ����1
    // ����Ҳ��ID���õ�
    // glGenTextures�������������������Ȼ������Ǵ����ڵڶ���������������
    glGenTextures(1, &texture1);
    // ��������֮���κε�����ָ��������õ�ǰ�󶨵�����
    // ���м������е�GL_TEXTURE_2D���������ڶ��������������������
    glBindTexture(GL_TEXTURE_2D, texture1);
    // Ϊ��ǰ�󶨵�����������û��Ʒ�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ������������ΪGL_REPEAT��Ĭ�ϻ��Ʒ�ʽ����S��
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T��
    // Ϊ��ǰ�󶨵�����������ù��˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //����ͼ�񣬴�����������mipmaps
    // ���ز���������
    int width, height, nrChannels;
    // ����stb_image.h�����ص�������y���Ϸ�ת��
    stbi_set_flip_vertically_on_load(true);
    // stbi_load()����һ��ͼ���ļ���λ����Ϊ���룬Ȼ����ͼ��Ŀ�ȡ��߶Ⱥ���ɫͨ���ĸ������ڶ��������͵��ĸ�����
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
#ifdef _TEST_
    data = stbi_load("Shima_Rin_512x512.png", &width, &height, &nrChannels, 0);
#endif // _TEST_

    std::cout << nrChannels << std::endl;
    //unsigned char* data = stbi_load("IMG_3365.PNG", &width, &height, &nrChannels, 0);
    if (data) {
        // ͨ��glTexImage2D��������ʹ��ǰ�󶨵�������󸽼�������ͼ��
        // param1: ָ������Ŀ�ꡣGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ�����
        // param2: Ϊ����ָ���༶��Զ����ļ���0������������
        // param3: ����OpenGL����ϣ����������Ϊ���ָ�ʽ����ͼ��ֻ��RGBֵ���������Ҳ��������ΪRGBֵ��
        // param4, 5: �������յ�����Ŀ�Ⱥ͸߶ȡ�ʹ�ö�Ӧ�ı������ɡ�
        // param6: Ӧ���ܱ���Ϊ0������ʷ�������⣩
        // param7, 8: ������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
        // param9: ������ͼ�����ݡ�
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
#ifdef _TEST_
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
#endif // _TEST_
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    // �����������Ӧ�Ķ༶��Զ������ͷ�ͼ����ڴ棨��ϰ�ߣ�
    stbi_image_free(data);

#ifdef _TWO_TEXTURES_
    // ����2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Ϊ��ǰ�󶨵�����������û��Ʒ�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Ϊ��ǰ�󶨵�����������ù��˷�ʽ
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    data = stbi_load("Shima_Rin_300x300.png", &width, &height, &nrChannels, 0);
    //data = stbi_load("Shima_Rin_512x512.png", &width, &height, &nrChannels, 0); // ʹ��RGBA
    //data = stbi_load("Shima_Rin_653x653.png", &width, &height, &nrChannels, 0);
    std::cout << nrChannels << std::endl;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // ����openglÿ�������������ĸ�����Ԫ��ֻ��Ҫ��һ�Σ���
    ourShader.use(); // ��Ҫ����������uniform����֮ǰ������ɫ������
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0); // �ֶ�����
    ourShader.setInt("texture2", 1); // ����ʹ����ɫ��������
#endif // _TWO_TEXTURES_

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ������
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

#ifdef _TWO_TEXTURES_
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
#endif // _TWO_TEXTURES_

        // render container
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}