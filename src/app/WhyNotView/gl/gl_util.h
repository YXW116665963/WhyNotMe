#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "wx/glcanvas.h"

namespace whygl
{
	struct ShaderInfo
	{
		GLint			iShaderType;
		std::string		strShaderSource;
	};

	void InitializeOpenGLFunctions();


	bool CompileLinkShader(const std::vector<ShaderInfo>& strShaderSource, GLuint& uShaderProgramId);
	bool CompileShader(GLint iShaderType, const std::string& strShaderSource,GLuint& uShaderId);
	bool LinkShader(const std::vector<GLuint>& vecShaderId,GLuint& uShaderProgramId);
}