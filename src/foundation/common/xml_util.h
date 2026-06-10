#pragma once
#include "rapidxml.hpp"  
#include "rapidxml_utils.hpp"  
#include "rapidxml_print.hpp" 
#include "rapidxml_ext.hpp"  //只多了这一行
#include "standard.h"
#include "util.h"
#include "text_util.h"
typedef rapidxml::xml_attribute<char> CXmlAttribute;
typedef rapidxml::xml_node<char> CXmlNode;
typedef rapidxml::xml_document<char> CXmlDocument;

namespace why
{
	/**
	* @author why
	* @brief 依据路径加载xml的根节点
	* @param strFilePath xml文件路径
	* @param doc		 pRoot依赖于doc存在，外部创建一个doc即可
	* @param pRoot 根节点
	* @return
	* - true 读取成功
	* - false
	*/
	bool GetXmlRootNode(const std::string& strFilePath, rapidxml::xml_document<char>& doc,CXmlNode*& pRoot);

	/**
	* @brief 依据路径查找xml节点
	* @param doc 保存加载解析好得xml文件得内容
	* @param path xml节点得路径(节点所有父节点含自身得名称以'/'链接而成)
	* @return
	* - nullptr 指定得节点不存在
	* - 其他 返回对应得xml节点得对象
	*/
	CXmlNode* QueryXMLNode(const CXmlDocument& doc, const std::string& path);

	/**
	* @brief 依据路径获取xml节点/属性得值
	* @param doc 保存加载解析好得xml文件得内容
	* @param path xml节点/属性得路径(节点所有父节点含自身得名称以'/'链接而成)
	* @return
	* - 空字符，对应得节点或属性未找到
	* - 对应节点或属性得值(文本类型)
	* @waring
	* - 有且只有路径得最后一个分量可以为属性名称。
	* - 对于最后一个分量，总是先尝试找节点，只有当节点不存在得情况下，在尝试查找属性
	*/
	std::string QueryXMLValue(const CXmlDocument& doc, const std::string& path);

	/**
	* @brief 获取对应得节点得属性值(布尔型)
	* @param pNode 节点对象
	* @param pName 属性名称
	* @param bDefault 属性不存在时得默认值
	* @return 属性得值
	*/
	bool AttributeAsBool(CXmlNode* pNode, const char* pName, bool bDefault);

	/**
	* @brief 获取对应得节点得属性值(有符号整数)
	* @param pNode 节点对象
	* @param pName 属性名称
	* @param nDefault 属性不存在时得默认值
	* @return 属性得值
	*/
	int32_t AttributeAsInt(CXmlNode* pNode, const char* pName, int32_t nDefault);

	/**
	* @brief 获取对应得节点得属性值(无符号整数)
	* @param pNode 节点对象
	* @param pName 属性名称
	* @param uDefault 属性不存在时得默认值
	* @return 属性得值
	*/
	uint32_t AttributeAsUInt(CXmlNode* pNode, const char* pName, uint32_t uDefault);

	/**
	* @brief 获取对应得节点得属性值(浮点数)
	* @param pNode 节点对象
	* @param pName 属性名称
	* @param fDefault 属性不存在时得默认值
	* @return 属性得值
	*/
	float32_t AttributeAsFloat(CXmlNode* pNode, const char* pName, float fDefault);

	/**
	* @brief 获取对应得节点得属性值(文本型)
	* @param pNode 节点对象
	* @param pName 属性名称
	* @param strValue 用于保存获取得属性值
	* @return
	* - true 此时strValue中保存得就是属性值
	* - flase 此时属性未找到
	*/
	bool GetAttributeText(CXmlNode* pNode, const char* pName, std::string& strValue);

	/**
	* @brief 指定的属性是否存在
	* @param pNode 节点对象
	* @param pName 属性名称
	* @return
	* - true 存在
	* - flase 不存在
	*/
	bool HasAttribute(CXmlNode* pNode, const char* pName);

	/**
	* @author	why
	* @brief	以指定类型获取XML数据
	* @param	pNode 节点对象
	* @param	pName 节点中的属性名称
	* @return
	* -	true	存在
	* -	flase	不存在
	*/
	template <typename T>
	inline bool GetAttributeData(CXmlNode* pNode, const char* pName, std::vector<T>& res)
	{
		CXmlAttribute* pAttribute = nullptr;
		const char* lpValue = nullptr;

		assert(pNode); assert(pName);
		

		pAttribute = pNode->first_attribute(pName);
		if (nullptr == pAttribute)
			return false;

		lpValue = pAttribute->value();
		if ((nullptr != lpValue) && (0 != strlen(lpValue)))
		{
			std::vector<std::string>			stringArray;
			int32_t								nSize;

			StringSplit(std::string(lpValue), ',', stringArray);
			nSize = (int32_t)stringArray.size();
			
			for (auto str : stringArray)
			{
				T value;
				/*
				* 模板实参推导（Template Argument Deduction）
					简单说：
					函数参数能确定 T → 就不用写 <T>
				*/
				if (!StringTo<T>(str, value))
				{
					return;
				}
				
				res.push_back(value);
			}
		}
		return true;
	}
}

