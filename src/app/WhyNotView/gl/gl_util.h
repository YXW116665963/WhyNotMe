#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "wx/glcanvas.h"

namespace why
{
	struct ModelInfo
	{
		// 外部输入
		GLuint uVao;
		GLuint uVbo;

		// 输出到外部
		GLuint uShaderProgramId;
	};

	// 着色器信息
	struct ShaderInfo
	{
		GLint			iShaderType;
		std::string		strShaderSource;
	};

	// 着色器程序信息，暂时不用
	struct ShaderProgramInfo
	{
		std::string					strProgramName;
		std::vector<ShaderInfo>		vecShaderInfo;
	};


	bool CompileLinkShader(const std::vector<ShaderInfo>& vecShaderInfo, GLuint& uShaderProgramId);
	bool CompileShader(GLint iShaderType, const std::string& strShaderSource,GLuint& uShaderId);
	bool LinkShader(const std::vector<GLuint>& vecShaderId,GLuint& uShaderProgramId);

	
	bool GetShaderInfo(const std::string& strShaderProgramName, std::vector<ShaderInfo>& vecShaderInfo);
}