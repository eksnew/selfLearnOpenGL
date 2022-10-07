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

// 全局
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;

int main() {
    // 初始化GLFW
    glfwInit();
    // 使用glfwWindowHint配置GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 将主版本号和次版本号设为3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "08019221 Yuanze Xu", NULL, NULL);
    if (window == NULL) {
        std::cout << "创建窗口对象失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 每当窗口调整大小的时候调用framebuffer_size_callback函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "初始化GLAD失败" << std::endl;
        return -1;
    }

    // 编译顶点着色器和片段着色器
    Shader tempShader("tri_rotation.vertex_shader", "tri_rotation.fragment_shader");

    // 把颜色数据加进顶点数据中。把颜色数据添加为3个float值至vertices数组
    float vertices[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // 生成顶点数组对象VAO
    glGenBuffers(1, &VBO); // 生成顶点缓冲对象VBO，用于管理内存

    // 绑定VAO
    glBindVertexArray(VAO);
    // 复制顶点数组到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 把之前定义的顶点数据复制到缓冲的内存中

    // 位置属性。对于每个顶点来说，位置顶点属性在前，所以它的偏移量是0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色属性。颜色属性紧随位置数据之后，所以偏移量就是3 * sizeof(float)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float lastX = 0;
    float tempX = 0;
    float tempY = 0;
    // 在每次循环开始前检查一次GLFW是否被要求退出
    while (!glfwWindowShouldClose(window)) {
        // 检查输入
        processInput(window);
        // 渲染指令
        glClearColor(0.85f, 0.43f, 0.83f, 1.0f); // 设置清空屏幕所用颜色
        glClear(GL_COLOR_BUFFER_BIT); // 清空屏幕颜色缓冲

        // 以时间为主要变换参数
        float timeValue = glfwGetTime();
        tempX = sin(timeValue);
        if (tempX - lastX < 0) {
            tempY = sqrt(1 - tempX * tempX);
        }
        else {
            tempY = -1 * sqrt(1 - tempX * tempX);
        }
        lastX = tempX;

        // 颜色变换
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        // 坐标变换
        glm::mat4 transform = glm::mat4(1.0f); // 确保矩阵初始化
        //transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f)); // 使三角形变位
        transform = glm::translate(transform, glm::vec3(tempY * 0.6, tempX * 0.6, 0.0f)); // 使三角形变位
        transform = glm::rotate(transform, timeValue, glm::vec3(0.0f, 0.0f, 1.0f)); // 使三角形旋转

        // 更新一个uniform前必须先调用glUseProgram，因为它是在当前激活的着色器程序中设置uniform的
        tempShader.use();
        // 更新uniform颜色
        int vertexColorLoc = glGetUniformLocation(tempShader.ID, "colorDelta");
        glUniform1f(vertexColorLoc, tempX); // 设置uniform值
        // 更新uniform变换矩阵
        unsigned int transformLoc = glGetUniformLocation(tempShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // 绘制三角形
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // 使用当前激活的着色器、之前定义的顶点属性配置、VBO的顶点数据（通过VAO间接绑定）来绘制图元

        // 交换缓冲并查询IO事件
        glfwSwapBuffers(window); // 交换颜色缓冲
        glfwPollEvents(); // 检查有没有触发什么事件
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // 正确释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

// 输入函数。在每个循环中调用。
void processInput(GLFWwindow* window) {
    // 检查Esc键是否被按下
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // 设置WindowShouldClose属性为true
        glfwSetWindowShouldClose(window, true);
}

// 当用户改变窗口的大小时调整视口。函数需要注册。
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // 设置窗口维度。也可以将视口的维度设置为比GLFW的维度小。
}