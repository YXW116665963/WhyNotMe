#include "xml_util.h"
#include "text_util.h"
#include "util.h"
#include <cstring>  // 或 #include <strings.h> 在某些系统上
namespace why
{
	CXmlNode* QueryXMLNode(const CXmlDocument& doc, const std::string& path)
	{
		std::vector<std::string>		items;
		size_t							uCount;
		CXmlNode* pNode = nullptr;

		StringSplit(path, '/', items);
		uCount = items.size();
		for (size_t i = 0; i < uCount; i++)
		{
			if (nullptr == pNode)
			{
				pNode = doc.first_node(items[i].c_str(), 0, false);
				if (nullptr == pNode)
					break;
			}
			else
			{
				pNode = pNode->first_node(items[i].c_str(), 0, false);
				if (nullptr == pNode)
					break;
			}
		}

		return pNode;
	}

	std::string QueryXMLValue(const CXmlDocument& doc, const std::string& path)
	{
		std::vector<std::string>		items;
		std::string						strValue;
		size_t							uCount;
		CXmlNode* pNode = nullptr;

		StringSplit(path, '/', items);
		uCount = items.size();
		if (0 == uCount)
			return strValue;

		for (size_t i = 0; i < (uCount - 1); i++)
		{
			if (nullptr == pNode)
			{
				pNode = doc.first_node();
				if (nullptr == pNode)
					break;

				if (false == strcasecmp(items[i].c_str(), pNode->name()))
				{
					pNode = nullptr;
					break;
				}
			}
			else
			{
				pNode = pNode->first_node(items[i].c_str(), 0, false);
				if (nullptr == pNode)
					break;
			}
		}

		if (pNode)
		{
			auto	pResult = pNode->first_node(items[uCount - 1].c_str(), 0, false);


			if (pResult)
			{
				strValue = pResult->value();
			}
			else
			{
				auto	pAttr = pNode->first_attribute(items[uCount - 1].c_str(), 0, false);

				if (pAttr)
					strValue = pAttr->value();
			}
		}

		return strValue;
	}

	bool AttributeAsBool(CXmlNode* pNode, const char* pName, bool bDefault)
	{
		CXmlAttribute* pAttribute;

		assert(pNode);
		assert(pName);

		pAttribute = pNode->first_attribute(pName);
		if (pAttribute)
		{
			if (strcasecmp(pAttribute->value(), "true"))
				return true;

			if (strcasecmp(pAttribute->value(), "false"))
				return false;

			//LOG_ERROR << "vlue of attribute[" << pName << "] is not valid:" << pAttribute->value();
		}

		return bDefault;
	}

	int32_t AttributeAsInt(CXmlNode* pNode, const char* pName, int32_t nDefault)
	{
		CXmlAttribute* pAttribute;

		assert(pNode);
		assert(pName);

		pAttribute = pNode->first_attribute(pName, 0, false);
		if (pAttribute)
			return (int32_t)atoi(pAttribute->value());
		//else
		//	LOG_WARNING << "attribute[" << pName << "] is not found";

		return nDefault;
	}

	uint32_t AttributeAsUInt(CXmlNode* pNode, const char* pName, uint32_t uDefault)
	{
		CXmlAttribute* pAttribute;

		assert(pNode);
		assert(pName);

		pAttribute = pNode->first_attribute(pName, 0, false);
		if (pAttribute)
			return (uint32_t)atoi(pAttribute->value());
		//else
		//	LOG_WARNING << "attribute[" << pName << "] is not found";

		return uDefault;
	}

	float32_t AttributeAsFloat(CXmlNode* pNode, const char* pName, float fDefault)
	{
		CXmlAttribute* pAttribute;

		assert(pNode);
		assert(pName);

		pAttribute = pNode->first_attribute(pName, 0, false);
		if (pAttribute)
			return (float32_t)atof(pAttribute->value());
		//else
		//	LOG_WARNING << "attribute[" << pName << "] is not found";

		return fDefault;
	}

	bool GetAttributeText(CXmlNode* pNode, const char* pName, std::string& strValue)
	{
		CXmlAttribute* pAttribute;

		assert(pNode);
		assert(pName);

		pAttribute = pNode->first_attribute(pName, 0, false);
		if (nullptr == pAttribute)
		{
			//LOG_WARNING << "attribute[" << pName << "] is not found";
			strValue.clear();
			return false;
		}

		strValue = pAttribute->value();
		return true;
	}
}
