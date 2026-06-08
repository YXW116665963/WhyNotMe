#include "glad/glad.h"//#include glad.h必须放在所有头文件之前，不管在哪里，不管在.h还是.cpp;
#include "gl_util.h"
#include "file_util.h"
#include "util.h"
#include "logger.h"
#include "data_center_value_util.h"
#include "env_var_data_def.h"
#include "xml_util.h"


#include <set>
namespace why
{
	std::set<GLint> g_setShaderType = {
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,		
	};

	std::map<std::string, GLint> g_mapShaderTypeName = {
		{"vertex",GL_VERTEX_SHADER},
		{"fragment",GL_FRAGMENT_SHADER}
	};

	bool GetShaderInfo(const std::string& strShaderProgramName, std::vector<ShaderInfo>& vecShaderInfo)
	{
		std::string strGLShader_dirPath = GetStringValue(envVar::g_Domain, envVar::strGLShader_dirPath);
		std::string strGLShaderXml_filePath = GetStringValue(envVar::g_Domain, envVar::strGLShaderXml_filePath);

		std::string						strXMLFileName = UTF8ToLocal(strGLShaderXml_filePath);
		rapidxml::file<char>			fdoc(strXMLFileName.c_str());
		rapidxml::xml_document<char>	doc;
		CXmlNode* pRoot = nullptr;

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
		pRoot = doc.first_node();
		if (nullptr == pRoot)
		{
			LOG_INFO << "invalidate xml file:" << strXMLFileName;
			return false;
		}


		CXmlNode* pResProgramNode;
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

			vecShaderInfo.push_back(ShaderInfo(g_mapShaderTypeName[strShaderType], strShaderSource));			
		}
		return true;
	}


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

		uShaderId = glCreateShader(iShaderType);
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