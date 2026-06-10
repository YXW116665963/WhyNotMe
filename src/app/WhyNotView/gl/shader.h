#pragma once
#include "glad/glad.h"
#include "util.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace why
{
    // 着色器信息
    struct ShaderInfo
    {
        GLint           iShaderType;
        std::string		strShaderSource;
        GLuint          uShaderId = 0;
    };

    class ShaderProgram
    {
    public:
        ShaderProgram(std::vector<ShaderInfo>& arrShaderSourceInfo);
        bool Load();
    private:
        // 编译GLSL
        bool CompileShader();
        // 链接GLSL
        bool LinkShader();
    public:
        uint32_t GetShaderProgramId();
        void Use();
    public:
        void SetMat4f(const std::string& strName, const glm::mat4& matrix4);
        void SetBool(const std::string& strName, const bool& value);
        void SetInt(const std::string&   strName, const int& value);
        void SetFloat(const std::string& strName, const float& value);
    private:
        GLuint                          m_uShaderProgramId;
        std::vector<ShaderInfo>         m_arrShaderSourceInfo;
        bool                            m_bSuccess;
    };

    bool CreateShader(const std::string& strShaderProgramName, ShaderProgram*& shader);
}