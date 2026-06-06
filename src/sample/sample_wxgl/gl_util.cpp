#include "gl_util.h"

#include "glad/glad.h"
// 窗口尺寸
const int WIDTH = 500;
const int HEIGHT = 500;


//这两段字符是写给显卡 GPU 执行的小程序，叫做 GLSL（OpenGL Shading Language）。


/*
    C++ 传顶点
       ↓
    顶点着色器：计算3D位置
       ↓
    光栅化：生成像素
       ↓
    片段着色器：给每个像素上色
       ↓
    显示到屏幕
*/

// 顶点着色器（Vertex Shader）
const char* vertexShaderSource = "#version 330 core\n"              //告诉显卡：我用的是 OpenGL 3.3 核心版本语法，必须写在第一行
"layout (location = 0) in vec3 aPos;\n"                             //输入：从 C++ 传过来的 顶点坐标（x,y,z）aPos
"uniform mat4 model;\n"                                             //模型矩阵：控制物体 平移 / 旋转 / 缩放
"uniform mat4 view;\n"                                              //观察矩阵：控制 相机位置、看哪里
"uniform mat4 projection;\n"                                        //投影矩阵：做 3D 透视效果（近大远小）
"void main() {\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"   //计算出这个顶点最终在屏幕上的位置
"}\0";
// 片段着色器（Fragment Shader）
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"                                             //输出：最终像素颜色
"void main() {\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n" // 橙色            //格式：(R, G, B, A) → 红、绿、蓝、透明度
"}\0";

// 绘制三角形
void Demo_DrawTriangles(unsigned int shaderProgram, unsigned int VAO)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}


GLHandler::GLHandler()
    :shaderProgram(0)
    ,VAO(0)
    ,VBO(0)
{
    // 模型、视图和投影矩阵
    model         = glm::mat4(1.0f);
    view          = glm::mat4(1.0f);
    projection    = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

}

void GLHandler::GLPrepare()
{
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置视口
    glViewport(0, 0, WIDTH, HEIGHT);

    // 创建着色器程序
    shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    // 立方体顶点
    float vertices[] = {
        // 所有的值是在[-1, 1]之间的
        -0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
        0.5f, -0.5f, 0.0f, 0.f, 0.f, 1.f,
        0.0f,  0.5f, 0.0f, 0.f, 0.f, 1.f
    };

    // 创建 VAO 和 VBO
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


    // 启用抗锯齿 (貌似没作用)
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    // 设置线宽
    glLineWidth(1);
}

void GLHandler::GLWhileDo()
{
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

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void GLHandler::GLEnd()
{
    // 删除 VAO, VBO 和着色器程序
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

// 创建着色器程序
unsigned int CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // 创建顶点着色器
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 检查顶点着色器编译错误
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 创建片段着色器
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 检查片段着色器编译错误
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 创建着色器程序
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 检查着色器程序链接错误
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 删除着色器对象
    // 只要着色器已经被正确地链接到着色器程序中，删除它们不会影响程序的正常运行。
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}