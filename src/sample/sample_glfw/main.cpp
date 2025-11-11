#include "demo_util.h"
#include "triangle.h"

// 每使用 #define GLAD_GL_IMPLEMENTATION，都定义 glad 的实现，
// 多以使用则会导致多次定义相同的符号。
// 为了避免这种情况，应该确保 #define GLAD_GL_IMPLEMENTATION 只出现在一个源文件中。
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
// 窗口尺寸
const int WIDTH = 500;
const int HEIGHT = 500;

// 着色器代码
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"}\0";
// 立方体的颜色
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 橙色
"}\0";


// 绘制正方体
void DrawCube(unsigned int shaderProgram, unsigned int VAO)
{
    glBindVertexArray(VAO);

    glDrawArrays(GL_LINE_LOOP, 0, 4); // 前面
    glDrawArrays(GL_LINE_LOOP, 4, 4); // 后面
    glDrawArrays(GL_LINE_LOOP, 8, 4); // 左侧
    glDrawArrays(GL_LINE_LOOP, 12, 4); // 右侧
    glDrawArrays(GL_LINE_LOOP, 16, 4); // 顶部
    glDrawArrays(GL_LINE_LOOP, 20, 4); // 底部

    glBindVertexArray(0);
}

int main()
{
    // 初始化 OpenGL
    if (!InitOpenGL(WIDTH,HEIGHT))
    {
        return 1;
    }

    // 创建着色器程序
    unsigned int shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    // 立方体顶点
    float vertices[] = {
        // 所有的值是在[-1, 1]之间的
        -0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
        0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
        0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f
    };

    // 创建 VAO 和 VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 绑定 VAO 和 VBO
    // 将当前的 VAO 绑定到 OpenGL 上下文中，这意味着接下来的顶点属性配置（如顶点数据如何读取）将被保存在这个 VAO 中。
    glBindVertexArray(VAO);
    // 将当前的 VBO 绑定到 OpenGL 上下文中，表示接下来的顶点数据将被存储到这个 VBO 中。
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 将顶点数据复制到 VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //#glVertexAttribPointer
    // 第一个参数为，索引开始位置
    // 第二个参数为，每个顶点属性的组件数量
    // 第五个参数为，步长（stride），即每个顶点属性之间的字节偏移量。有时顶点数组中可能包含法线数据

    // 设置顶点属性指针
    // 实际是在设置VAO，也就是设置如何使用VBO的数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 设置法线属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 模型、视图和投影矩阵
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    // 获取控制台窗口句柄
    HWND hConsole = GetConsoleWindow();
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    // 启用抗锯齿 (貌似没作用)
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);
    // 设置线宽
    glLineWidth(1);


    // 主循环
    while (!glfwWindowShouldClose(glfwGetCurrentContext()))
    {        
        if (!ShouldRedraw(60))
        {
            continue;
        }

        // 清除颜色缓冲区
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 使用着色器程序
        glUseProgram(shaderProgram);

        // 设置模型、视图和投影矩阵        
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


        // 绘制物体
        Demo_DrawTriangles(shaderProgram, VAO);

        // 交换缓冲区
        glfwSwapBuffers(glfwGetCurrentContext());
        // 处理事件
        glfwPollEvents();
    }

    // 删除 VAO, VBO 和着色器程序
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // 退出 GLFW
    glfwTerminate();

    return 0;
}