#include "demo_util.h"
#ifdef _WIN32
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>
#endif
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

// 在 glfw 窗口标题显示中文
void SetWindowTitleUTF8(GLFWwindow* window, const char* title)
{
    int wlen = MultiByteToWideChar(CP_ACP, 0, title, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[wlen];
    MultiByteToWideChar(CP_ACP, 0, title, -1, wstr, wlen);

    int utf8len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* utf8str = new char[utf8len];
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8str, utf8len, NULL, NULL);

    glfwSetWindowTitle(window, utf8str);

    delete[] wstr;
    delete[] utf8str;
}

// 初始化 GLFW 和 OpenGL
bool InitOpenGL(const int WIDTH, const int HEIGHT)
{
    // 初始化 GLFW
    if (!glfwInit())
    {
        return false;
    }

    // 设置 GLFW 版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL_demo", NULL, NULL);
	HWND hwnd = glfwGetWin32Window(window);

    if (!window)
    {
        glfwTerminate();
        return false;
    }
    SetWindowTitleUTF8(window, "OpenGL_demo");

    // 设置当前窗口为活动窗口
    glfwMakeContextCurrent(window);

    // 初始化 GLAD
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // 设置视口
    glViewport(0, 0, WIDTH, HEIGHT);

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    return true;
}

bool ShouldRedraw(int fps)
{
    // 默认帧率
    if (fps <= 0)
    {
		fps = 60; 
    }

	static double lastTime = 0.0;           // 上次绘制时间

    double currentTime = glfwGetTime();     // 获取当前时间
	double interval = 1.0 / fps;            // 计算每帧的时间间隔

    // 计算时间差
    if (currentTime - lastTime >= interval) 
    {
        lastTime = currentTime;             // 更新最后绘制时间
        return true;                        // 返回需要重绘
    }

    return false;                           // 不需要重绘
}


/*
    // 帧率控制
    double timePrevious = glfwGetTime();
    double timeLast = 0.0;
    int fps = 0;
    double standard_frame_rate = 60; // 目标帧率
    double frameTimeLimit = 1.0 / standard_frame_rate;  // 初始跳帧时间
    float fltFactor = 0.0f;
    double timeFramePrevious = glfwGetTime();
    double timeFrameLast = 0.0;
*/


// 帧率控制函数
// fps: 当前帧率
// frameTimeLimit： 当前跳帧时间
double AdjustFrameRate(int& fps, double& frameTimeLimit, double standard_frame_rate, double& timeFrameLast, double& timeFramePrevious) {
    // 计算时间间隔
    timeFramePrevious = glfwGetTime();
    if ((timeFramePrevious - timeFrameLast) >= 1.0f) {
        timeFrameLast = timeFramePrevious;

        // 根据当前帧率调整跳帧时间，让帧率趋近 standard_frame_rate
        float fltFactor = 0.0f;
        if (fps > standard_frame_rate) {
            if (fps > (standard_frame_rate * 2)) {
                fltFactor = 1;
            }
            else if (fps > (standard_frame_rate * 1.5)) {
                fltFactor = 0.5;
            }
            else if (fps > (standard_frame_rate * 1.1)) {
                fltFactor = 0.1;
            }
            else {
                fltFactor = 0.01;
            }
            frameTimeLimit = frameTimeLimit + (frameTimeLimit * fltFactor);
        }
        else if (fps < standard_frame_rate) {
            if (fps < (standard_frame_rate / 1.1)) {
                fltFactor = 0.1;
            }
            else if (fps < (standard_frame_rate / 1.5)) {
                fltFactor = 0.5;
            }
            else if (fps < (standard_frame_rate / 2)) {
                fltFactor = 1;
            }
            frameTimeLimit = frameTimeLimit - (frameTimeLimit * fltFactor);
        }
        fps = 0;

        // 防止跳帧时间过小
        if (frameTimeLimit < 0.000001) {
            frameTimeLimit = 0.1;
        }
    }
    return frameTimeLimit;
}