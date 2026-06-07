#include "glad/glad.h"//#include glad.h必须放在所有头文件之前，不管在哪里，不管在.h还是.cpp;
#include "gl_util.h"
#include "logger.h"
#include <set>
namespace whygl
{
	std::set<GLint> g_setShaderType = {
		GL_FRAGMENT_SHADER,
		GL_VERTEX_SHADER,
		
	};

	bool CompileLinkShader(const std::vector<ShaderInfo>& vecShaderInfo, GLuint& uShaderProgramId)
	{
		std::vector<GLuint> vecShaderId;

		for (auto shaderInfo : vecShaderInfo)
		{
			GLuint uShaderId;

			if (!CompileShader(shaderInfo.iShaderType, shaderInfo.strShaderSource, uShaderId))
			{
				return false;
			}

			vecShaderId.push_back(uShaderId);
		}

		if (!LinkShader(vecShaderId, uShaderProgramId))
		{
			return false;
		}

		return true;
	}

	bool CompileShader(GLint iShaderType, const std::string& strShaderSource, GLuint& uShaderId)
	{
		if (!g_setShaderType.count(iShaderType))
		{
			LOG_ERROR << iShaderType << "is not exist in g_setShaderType";
			return false;
		}

		GLuint uShaderId = glCreateShader(iShaderType);
		const char* cpShaderSource = strShaderSource.c_str();
		glShaderSource(uShaderId, 1, &cpShaderSource, nullptr);
		glCompileShader(uShaderId);

		int success;
		char infoLog[512];
		glGetShaderiv(uShaderId, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(uShaderId, 512, NULL, infoLog);
			LOG_ERROR << "SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
			LOG_ERROR << strShaderSource;
			return false;
		}

		return true;
	}

	bool LinkShader(const std::vector<GLuint>& vecShaderId, GLuint& uShaderProgramId)
	{
		// 链接GPU程序
		uShaderProgramId = glCreateProgram();

		for (auto uShaderId : vecShaderId)
		{
			glAttachShader(uShaderProgramId, uShaderId);
		}
		glLinkProgram(uShaderProgramId);

		// 检查着色器程序链接错误
		GLint success;
		char infoLog[512];

		glGetProgramiv(uShaderProgramId, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(uShaderProgramId, 512, NULL, infoLog);
			LOG_ERROR << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			return false;
		}

		for (auto uShaderId : vecShaderId)
		{
			glDeleteShader(uShaderId);
		}

		return true;
	}

}