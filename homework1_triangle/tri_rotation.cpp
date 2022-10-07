#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ȫ��
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

int main() {
    // ��ʼ��GLFW
    glfwInit();
    // ʹ��glfwWindowHint����GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // �����汾�źʹΰ汾����Ϊ3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // ʹ�ú���ģʽ

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // �������ڶ���
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "08019221 Yuanze Xu", NULL, NULL);
    if (window == NULL) {
        std::cout << "�������ڶ���ʧ��" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // ÿ�����ڵ�����С��ʱ�����framebuffer_size_callback����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ��ʼ��GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "��ʼ��GLADʧ��" << std::endl;
        return -1;
    }

    // ���붥����ɫ����Ƭ����ɫ��
    Shader tempShader("tri_rotation.vertex_shader", "tri_rotation.fragment_shader");

    // ����ɫ���ݼӽ����������С�����ɫ�������Ϊ3��floatֵ��vertices����
    float vertices[] = {
        // λ��              // ��ɫ
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
    };


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // ���ɶ����������VAO
    glGenBuffers(1, &VBO); // ���ɶ��㻺�����VBO�����ڹ����ڴ�

    // ��VAO
    glBindVertexArray(VAO);
    // ���ƶ������鵽�����й�OpenGLʹ��
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // ���´����Ļ���󶨵�GL_ARRAY_BUFFERĿ����
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ��֮ǰ����Ķ������ݸ��Ƶ�������ڴ���

    // λ�����ԡ�����ÿ��������˵��λ�ö���������ǰ����������ƫ������0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ��ɫ���ԡ���ɫ���Խ���λ������֮������ƫ��������3 * sizeof(float)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float lastX = 0;
    float tempX = 0;
    float tempY = 0;
    // ��ÿ��ѭ����ʼǰ���һ��GLFW�Ƿ�Ҫ���˳�
    while (!glfwWindowShouldClose(window)) {
        // �������
        processInput(window);
        // ��Ⱦָ��
        glClearColor(0.85f, 0.43f, 0.83f, 1.0f); // ���������Ļ������ɫ
        glClear(GL_COLOR_BUFFER_BIT); // �����Ļ��ɫ����

        // ��ʱ��Ϊ��Ҫ�任����
        float timeValue = glfwGetTime();
        tempX = sin(timeValue);
        if (tempX - lastX < 0) {
            tempY = sqrt(1 - tempX * tempX);
        }
        else {
            tempY = -1 * sqrt(1 - tempX * tempX);
        }
        lastX = tempX;

        // ��ɫ�任
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        // ����任
        glm::mat4 transform = glm::mat4(1.0f); // ȷ�������ʼ��
        //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f)); // ʹ�����α�λ
        transform = glm::translate(transform, glm::vec3(tempY * 0.6, tempX * 0.6, 0.0f)); // ʹ�����α�λ
        transform = glm::rotate(transform, timeValue, glm::vec3(0.0f, 0.0f, 1.0f)); // ʹ��������ת

        // ����һ��uniformǰ�����ȵ���glUseProgram����Ϊ�����ڵ�ǰ�������ɫ������������uniform��
        tempShader.use();
        // ����uniform��ɫ
        int vertexColorLoc = glGetUniformLocation(tempShader.ID, "colorDelta");
        glUniform1f(vertexColorLoc, tempX); // ����uniformֵ
        // ����uniform�任����
        unsigned int transformLoc = glGetUniformLocation(tempShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // ����������
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // ʹ�õ�ǰ�������ɫ����֮ǰ����Ķ����������á�VBO�Ķ������ݣ�ͨ��VAO��Ӱ󶨣�������ͼԪ

        // �������岢��ѯIO�¼�
        glfwSwapBuffers(window); // ������ɫ����
        glfwPollEvents(); // �����û�д���ʲô�¼�
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // ��ȷ�ͷ�/ɾ��֮ǰ�ķ����������Դ
    glfwTerminate();
    return 0;
}

// ���뺯������ÿ��ѭ���е��á�
void processInput(GLFWwindow* window) {
    // ���Esc���Ƿ񱻰���
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // ����WindowShouldClose����Ϊtrue
        glfwSetWindowShouldClose(window, true);
}

// ���û��ı䴰�ڵĴ�Сʱ�����ӿڡ�������Ҫע�ᡣ
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // ���ô���ά�ȡ�Ҳ���Խ��ӿڵ�ά������Ϊ��GLFW��ά��С��
}