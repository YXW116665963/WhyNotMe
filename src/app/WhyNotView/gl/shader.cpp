
#include "shader.h"
#include "logger.h"
#include "xml_util.h"
#include "data_center_value_util.h"
#include "env_var_data_def.h"
#include "file_util.h"
#include <map>
#include <set>
namespace why
{
	// xml中字段对应的着色器类型
	std::map<std::string, GLint> g_mapShaderTypeName = {
		{"vertex"	,GL_VERTEX_SHADER},
		{"fragment"	,GL_FRAGMENT_SHADER}
	};
		;

	bool CreateShader(const std::string& strShaderProgramName, ShaderProgram*& shaderProgram)
	{
		std::string strGLShader_dirPath = GetStringValue(envVar::g_Domain, envVar::strGLShader_dirPath);
		std::string strGLShaderXml_filePath = GetStringValue(envVar::g_Domain, envVar::strGLShaderXml_filePath);
		
		std::string						strXMLFileName = UTF8ToLocal(strGLShaderXml_filePath);
		rapidxml::file<char>			fdoc(strXMLFileName.c_str());
		rapidxml::xml_document<char> doc;
		try
		{
			doc.parse<0>(fdoc.data());
		}
		catch (const rapidxml::parse_error& ex)
		{
			LOG_ERROR << "invalidate xml file:" << strXMLFileName << ",where:" << ex.where<char>();
			throw ex;
			return false;
		}
		catch (const std::exception& e)
		{
			LOG_ERROR << "invalidate xml file:" << strXMLFileName;
			throw e;
			return false;
		}
		CXmlNode* pRoot = doc.first_node();
		if (nullptr == pRoot)
		{
			LOG_INFO << "invalidate xml file:" << strXMLFileName;
			return false;
		}


		// 比对着色器程序名
		CXmlNode* pResProgramNode = nullptr;
		for (CXmlNode* pCur = pRoot->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
		{
			std::string lpName = pCur->name();
			std::string strXmlShaderProName;
			if (!GetAttributeText(pCur, "name", strXmlShaderProName))
			{
				continue;
			}

			if (strXmlShaderProName == strShaderProgramName)
			{
				pResProgramNode = pCur;
			}
		}

		if (nullptr == pResProgramNode)
		{
			LOG_ERROR << "not find node" << strShaderProgramName << " in shader.xml";
			return false;
		}

		// 获取着色器类型和程序
		std::vector<ShaderInfo> arrShaderSourceInfo;
		for (CXmlNode* pCur = pResProgramNode->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
		{
			std::string lpName = pCur->name();
			std::string strShaderFileName;
			std::string strShaderType;

			if (!GetAttributeText(pCur, "file_name", strShaderFileName))
			{
				LOG_ERROR << strShaderProgramName << "has no file_name node!";
				return false;
			}

			if (!GetAttributeText(pCur, "type", strShaderType))
			{
				LOG_ERROR << strShaderProgramName << "has no type node!";
				return false;
			}

			if (g_mapShaderTypeName.find(strShaderType) == g_mapShaderTypeName.end())
			{
				LOG_ERROR << strShaderProgramName << "has err type node!";
				return false;
			}

			PathAppender pathAppender;
			std::string strShaderFilePath
				= pathAppender.SetSourcePath(strGLShader_dirPath)
				.AppendChildPath(strShaderFileName)
				.GetPath();

			std::string strShaderSource;
			if (!LoadTextFile(strShaderFilePath, strShaderSource))
			{
				return false;
			}

			ShaderInfo shaderInfo;
			shaderInfo.iShaderType = g_mapShaderTypeName[strShaderType];
			shaderInfo.strShaderSource = strShaderSource;

			arrShaderSourceInfo.push_back(shaderInfo);
		}

		shaderProgram = new ShaderProgram(arrShaderSourceInfo);

		return true;
	}


	ShaderProgram::ShaderProgram(std::vector<ShaderInfo>& arrShaderSourceInfo)
		:	m_uShaderProgramId(0)
		,	m_arrShaderSourceInfo(arrShaderSourceInfo)
		,	m_bSuccess(false)
	{
		
	}

	

	bool ShaderProgram::Load()
	{
		if (!CompileShader())
		{
			m_bSuccess = false;
			return false;
		}

		if (!LinkShader())
		{
			m_bSuccess = false;
			return false;
		}

		m_bSuccess = true;

		return true;
	}

	bool ShaderProgram::CompileShader()
	{
		for (auto &shaderSourceInfo : m_arrShaderSourceInfo)
		{
			shaderSourceInfo.uShaderId = glCreateShader(shaderSourceInfo.iShaderType);
			const char* cstrShaderSource = shaderSourceInfo.strShaderSource.c_str();
			glShaderSource(shaderSourceInfo.uShaderId, 1, &cstrShaderSource, nullptr);
			glCompileShader(shaderSourceInfo.uShaderId);

			GLint success;
			char infoLog[512];
			glGetShaderiv(shaderSourceInfo.uShaderId, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderSourceInfo.uShaderId, 512, NULL, infoLog);
				LOG_ERROR << "SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
				LOG_ERROR << shaderSourceInfo.strShaderSource;
				return false;
			}
		}

		return true;
	}

	bool ShaderProgram::LinkShader()
	{		
		m_uShaderProgramId = glCreateProgram();

		for (auto &shaderSourceInfo : m_arrShaderSourceInfo)
		{
			glAttachShader(m_uShaderProgramId, shaderSourceInfo.uShaderId);
		}
		glLinkProgram(m_uShaderProgramId);
		for (auto &shaderSourceInfo : m_arrShaderSourceInfo)
		{
			glDeleteShader(shaderSourceInfo.uShaderId);
		}

		// 检查着色器程序链接错误
		GLint success;
		char infoLog[512];

		glGetProgramiv(m_uShaderProgramId, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(m_uShaderProgramId, 512, NULL, infoLog);
			LOG_ERROR << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			return false;
		}

		return true;
	}

	uint32_t ShaderProgram::GetShaderProgramId() 
	{ 
		return m_uShaderProgramId; 
	};

	void ShaderProgram::Use() 
	{ 
		glUseProgram(m_uShaderProgramId); 
	}

	void ShaderProgram::SetMat4f(const std::string& strName, const glm::mat4& matrix4)
	{
		GLuint uLoc = glGetUniformLocation(m_uShaderProgramId, strName.c_str());
		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(matrix4));
	}

	void ShaderProgram::SetBool(const std::string& strName, const bool& value)
	{
		GLuint uLoc = glGetUniformLocation(m_uShaderProgramId, strName.c_str());
		glUniform1i(uLoc, (int)value);
	}

	void ShaderProgram::SetInt(const std::string& strName, const int& value)
	{
		GLuint uLoc = glGetUniformLocation(m_uShaderProgramId, strName.c_str());
		glUniform1i(uLoc, value);
	}

	void ShaderProgram::SetFloat(const std::string& name, const float& value)
	{
		GLuint uLoc = glGetUniformLocation(m_uShaderProgramId, name.c_str());
		glUniform1f(uLoc, value);
	}


}