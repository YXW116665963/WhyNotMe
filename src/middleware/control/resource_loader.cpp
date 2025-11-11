#include "resource_loader.h"
#include "text_util.h"
#include "file_util.h"
#include "background.h"
#include "resource_manager.h"
#include "singleton.h"
#include "logger.h"
#include "rapidxml.hpp"
#include <wx/filename.h>

namespace why
{
	IDataCenter* g_pMiddlewareDataCenter{ nullptr };
	
	namespace
	{
		typedef	std::map<std::string, PicturePtr> PicturePool;
		typedef PicturePool::iterator	PicturePoolIt;

		std::vector<std::string>		g_searchPath;
		std::string						g_strDeviceModel;
		std::string						g_strLanguageSuffix;
		wxString						g_strPicturePath;
		wxString						g_strResourceTargetPath;
		PicturePool						g_picturePool;
		auto							g_backgroundDeleter = [](IBackground *pPicture) { if (pPicture) pPicture->Destroy(); };
		
		PicturePtr CreatePictureFromFile(const char *pFileName)
		{
			const char				*pFind = nullptr;
			std::string				strKey = pFileName;
			std::string				strFullPathFileName;
			PicturePoolIt			itFind = g_picturePool.end();

			pFind = strchr(pFileName, ':');
			if (nullptr == pFind)
			{
				if (!SearchResourceFullPath(g_strPicturePath.ToStdString(), pFileName, strFullPathFileName))
				{
					LOG_ERROR << "SearchResourceFullPath not find:" << g_strPicturePath << "strFileName:" << pFileName;
					return NULL;
				}
			}
			else
			{
				strFullPathFileName = pFileName;
			}

			itFind = g_picturePool.find(strKey);
			if (itFind == g_picturePool.end())
			{
				PicturePtr		picturePtr(new Picture);

				if (picturePtr->CreatePicture(strFullPathFileName.c_str()))
					g_picturePool.insert(std::make_pair(strKey, picturePtr));
				else
					picturePtr.reset();
				return picturePtr;
			}
			else
			{
				return itFind->second;
			}
		}
	}

	void SetDataCenter(IDataCenter* pDataCenter)
	{
		g_pMiddlewareDataCenter = pDataCenter;
	}

	void SetPicturePath(const char *pPicturePath, const char* pResourceTargetPath, const char* pDeviceModel, const char* pLanguageSuffix)
	{
		if (nullptr != pPicturePath)
			g_strPicturePath = wxString::FromUTF8(pPicturePath);

		if (nullptr != pResourceTargetPath)
			g_strResourceTargetPath = wxString::FromUTF8(pResourceTargetPath);

		if (nullptr != pDeviceModel)
			g_strDeviceModel = pDeviceModel;
		
		if (nullptr != pLanguageSuffix)
			g_strLanguageSuffix = pLanguageSuffix;
	}

	void SetResourceSearchPath(const std::vector<std::string>& strSearchPath)
	{
		g_searchPath.insert(g_searchPath.end(), strSearchPath.begin(), strSearchPath.end());
	}

	bool SearchResourceFullPath(const std::string& strResourcePath, const std::string& strFileName, std::string& strLoadResourceFullPath)
	{
		//搜索机型+语言带文件名全路径
		for (auto& item : g_searchPath)
		{
			//std::string strPath = strResourcePath + item + std::filesystem::path::preferred_separator + strFileName;

			// 使用 std::filesystem::path 来拼接路径，确保平台兼容性
			std::filesystem::path path = std::filesystem::path(strResourcePath) / item / strFileName;
			// 检查文件是否存在
			if (wxFileName::FileExists(wxString::FromUTF8(path.string())))
			{
				strLoadResourceFullPath = path.string(); // 获取全路径并返回
				return true;
			}
		}
		return false;
	}

	bool GetAttributeColor(CXmlNode* pNode, const char* pName, wxColour& colorValue)
	{
		CXmlAttribute				*pAttribute = nullptr;
		const char					*lpValue = nullptr;

		assert(pNode);
		assert(pName);

		pAttribute = pNode->first_attribute(pName);
		if (nullptr == pAttribute)
			return false;

		lpValue = pAttribute->value();
		if ((nullptr == lpValue) || (0 == strlen(lpValue)))
			return false;

		if ('#' == lpValue[0])
		{
			uint32_t			uValue;
			uint8_t				uRed;
			uint8_t				uGreed;
			uint8_t				uBlue;

			sscanf_s(&lpValue[1], "%x", &uValue);
			uRed = GetBValue(uValue);
			uGreed = GetGValue(uValue);
			uBlue = GetRValue(uValue);
			colorValue.Set(uRed, uGreed, uBlue);
		}
		else
		{
			std::vector<std::string>			stringArray;
			int32_t								nSize;
			uint8_t								uRed;
			uint8_t								uGreed;
			uint8_t								uBlue;
			uint8_t								uAlpha;

			StringSplit(std::string(lpValue), ',', stringArray);
			nSize = (int32_t)stringArray.size();

			uRed = (nSize >= 1) ? (uint8_t)atoi(stringArray[0].c_str()) : 0;
			uGreed = (nSize >= 2) ? (uint8_t)atoi(stringArray[1].c_str()) : 0;
			uBlue = (nSize >= 3) ? (uint8_t)atoi(stringArray[2].c_str()) : 0;
			uAlpha = (nSize >= 4) ? (uint8_t)atoi(stringArray[3].c_str()) : wxALPHA_OPAQUE;
			colorValue.Set(uRed, uGreed, uBlue, uAlpha);
		}
		return true;
	}

	PicturePtr GetAttributePicture(CXmlNode *pNode, const char *pName)
	{
		CXmlAttribute		*pAttribute = nullptr;
		
		pAttribute = pNode->first_attribute(pName);
		if (nullptr == pAttribute)
			return nullptr;

		return CreatePictureFromFile(pAttribute->value());
	}

	bool LoadStretchSize(CXmlNode* pNode, wxSize& rcLeftTop, wxSize& rcTopBottom)
	{
		CXmlAttribute		*pAttribute = nullptr;

		pAttribute = pNode->first_attribute("left");
		if (nullptr == pAttribute)
			rcLeftTop.x = 0;
		else
			rcLeftTop.x = atoi(pAttribute->value());

		pAttribute = pNode->first_attribute("top");
		if (nullptr == pAttribute)
			rcLeftTop.y = 0;
		else
			rcLeftTop.y = atoi(pAttribute->value());

		pAttribute = pNode->first_attribute("right");
		if (nullptr == pAttribute)
			rcTopBottom.x = 0;
		else
			rcTopBottom.x = atoi(pAttribute->value());

		pAttribute = pNode->first_attribute("bottom");
		if (nullptr == pAttribute)
			rcTopBottom.y = 0;
		else
			rcTopBottom.y = atoi(pAttribute->value());

		return true;
	}

	bool GetAttributePoint(CXmlNode* pNode, const char* pName, wxPoint& ptPoint)
	{
		CXmlAttribute*		pAttribute = nullptr;
		const char*			lpValue = nullptr;

		assert(pNode);
		assert(pName);

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
			if (nSize < 2)
				return false;

			ptPoint.x = atoi(stringArray[0].c_str());
			ptPoint.y = atoi(stringArray[1].c_str());
		}
		return true;
	}

	bool GetTextAttributeStyle(CXmlNode* pNode, const char* pName, uint32_t& style)
	{
		CXmlAttribute*		pAttribute = nullptr;
		const char*			lpValue = nullptr;

		assert(pNode);
		assert(pName);

		pAttribute = pNode->first_attribute(pName);
		if (nullptr == pAttribute)
			return false;

		lpValue = pAttribute->value();
		if ((nullptr != lpValue) && (0 != strlen(lpValue)))
		{
			std::vector<std::string>			stringArray;

			StringSplit(std::string(lpValue), ',', stringArray);

			for (auto iter = stringArray.begin(); iter != stringArray.end(); iter++)
			{
				if ("left" == *iter) style |= wxTE_LEFT;
				if ("center" == *iter) style |= wxTE_CENTER;
				if ("right" == *iter) style |= wxTE_RIGHT;
			}
		}
		return true;
	}

	bool GetAttributeRect(CXmlNode *pNode, wxRect &rcRect)
	{
		CXmlAttribute		*pAttribute = nullptr;

		pAttribute = pNode->first_attribute("x");
		if (nullptr == pAttribute)
		{
			pAttribute = pNode->first_attribute("left");
			if (nullptr != pAttribute)
				rcRect.x = atoi(pAttribute->value());
			else
				rcRect.x = 0;
		}
		else
		{
			rcRect.x = atoi(pAttribute->value());
		}

		pAttribute = pNode->first_attribute("y");
		if (nullptr == pAttribute)
		{
			pAttribute = pNode->first_attribute("top");
			if (nullptr != pAttribute)
				rcRect.y = atoi(pAttribute->value());
			else
				rcRect.y = 0;
		}
		else
		{
			rcRect.y = atoi(pAttribute->value());
		}

		pAttribute = pNode->first_attribute("right");
		if (nullptr == pAttribute)
		{
			pAttribute = pNode->first_attribute("width");
			if (nullptr != pAttribute)
				rcRect.width = atoi(pAttribute->value());
			else
				rcRect.width = 0 - rcRect.x;
		}
		else
		{
			rcRect.width = atoi(pAttribute->value()) - rcRect.x;
		}

		pAttribute = pNode->first_attribute("bottom");
		if (nullptr == pAttribute)
		{
			pAttribute = pNode->first_attribute("height");
			if (nullptr != pAttribute)
				rcRect.height = atoi(pAttribute->value());
			else
				rcRect.height = 0 - rcRect.y;
		}
		else
		{
			rcRect.height = atoi(pAttribute->value()) - rcRect.y;
		}

		return true;
	}

	void GetAttributeMargin(CXmlNode* pNode, wxSize& szLeftTop, wxSize& szRightBottom)
	{
		if (nullptr != pNode)
		{
			CXmlAttribute* pAttribute = nullptr;

			pAttribute = pNode->first_attribute("left");
			if (nullptr != pAttribute)
				szLeftTop.x = atoi(pAttribute->value());
			else
				szLeftTop.x = 0;

			pAttribute = pNode->first_attribute("top");
			if (nullptr != pAttribute)
				szLeftTop.y = atoi(pAttribute->value());
			else
				szLeftTop.y = 0;

			pAttribute = pNode->first_attribute("right");
			if (nullptr != pAttribute)
				szRightBottom.x = atoi(pAttribute->value());
			else
				szRightBottom.x = 0;

			pAttribute = pNode->first_attribute("bottom");
			if (nullptr != pAttribute)
				szRightBottom.y = atoi(pAttribute->value());
			else
				szRightBottom.y = 0;
		}
		else
		{
			szLeftTop.x = 0;
			szLeftTop.y = 0;
			szRightBottom.x = 0;
			szRightBottom.y = 0;
		}
	}

	PicturePtr LoadPicture(CXmlNode *pNode)
	{
		PicturePtr				picturePtr;
		std::string				strUsed;
		
		GetAttributeText(pNode, "used", strUsed);
		if (0 != strUsed.length())
			picturePtr = Singleton<ResourceMgr>::Instance()->GetPicture(strUsed.c_str());

		if (nullptr == picturePtr)
		{
			std::string				strFileName;

			GetAttributeText(pNode, "file", strFileName);
			if (!strFileName.empty())
				picturePtr = CreatePictureFromFile(strFileName.c_str());
		}
		
		return picturePtr;
	}

	BackgroundPtr LoadPictureBackground(CXmlNode *pNode)
	{
		CXmlNode			*pRect = nullptr;
		CXmlNode			*pStretch = nullptr;
		wxRect				rcRect;
		wxSize				szLeftTop;
		wxSize				szRightBottom;
		PicturePtr			picturePtr;
		BackgroundPtr		backgroundPtr(nullptr, g_backgroundDeleter);

		picturePtr = LoadPicture(pNode);
		if (nullptr == picturePtr.get())
			return backgroundPtr;

		pRect = pNode->first_node("Rect");
		if (pRect)
			GetAttributeRect(pRect, rcRect);

		//按9宫格方式拉伸的图片
		pStretch = pNode->first_node("Stretch");
		if (pStretch)
			LoadStretchSize(pStretch, szLeftTop, szRightBottom);
		
		PictureBackground *pPictureBK= new PictureBackground(picturePtr, rcRect, szLeftTop, szRightBottom);
		backgroundPtr.reset(pPictureBK);

		CXmlNode *pCache = pNode->first_node("Cache");
		if (pCache)
		{
			wxColour		colorValue;

			if (GetAttributeColor(pCache, "blend_color", colorValue))
				pPictureBK->CreateCache(colorValue);
			
			for (auto pCur = pCache->first_node(); pCur != NULL; pCur = pCur->next_sibling())
			{
				if (0 == _stricmp(pCur->name(), "Size"))
				{
					uint32_t			uWdith = AttributeAsUInt(pCur, "width", 0);
					uint32_t			uHeight = AttributeAsUInt(pCur, "height", 0);

					if ((0 != uWdith) || (0 != uHeight))
						pPictureBK->GenerateCacheBuffer(uWdith, uHeight);
				}
			}
		}
		
		return backgroundPtr;
	}

	BackgroundPtr LoadFadeBackground(CXmlNode *pNode)
	{
		wxColor				crFillBegin;
		wxColor				crFillEnd;
		wxColor				crBorder;
		bool				bHorizontal;
		uint32_t			uWdith;
		BackgroundPtr		backgroundPtr(nullptr, g_backgroundDeleter);

		GetAttributeColor(pNode, "beginfill", crFillBegin);
		GetAttributeColor(pNode, "endfill", crFillEnd);
		bHorizontal = AttributeAsBool(pNode, "horizontal", FALSE);
		GetAttributeColor(pNode, "border", crBorder);
		uWdith = AttributeAsUInt(pNode, "borderwidth", 0);

		backgroundPtr.reset(new FadeBackground(crFillBegin, crFillEnd, bHorizontal, crBorder, uWdith));
		return backgroundPtr;
	}

	BackgroundPtr LoadSolidBackground(CXmlNode *pNode)
	{
		wxColor				crFill;
		wxColor				crBorder;
		uint32_t			uWdith;
		BackgroundPtr		backgroundPtr(nullptr, g_backgroundDeleter);

		GetAttributeColor(pNode, "fill", crFill);
		GetAttributeColor(pNode, "border", crBorder);
		uWdith = AttributeAsInt(pNode, "borderwidth", 0);

		backgroundPtr.reset(new SolidBackground(crFill, crBorder, uWdith));
		return backgroundPtr;
	}

	BackgroundPtr LoadBackground(CXmlNode *pNode)
	{
		BackgroundPtr			backgroundPtr(nullptr, g_backgroundDeleter);
		std::string					strUsed;

		GetAttributeText(pNode, "used", strUsed);
		if (0 != strUsed.length())
			backgroundPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strUsed.c_str());

		if (nullptr == backgroundPtr.get())
		{
			CXmlNode			*pChild;

			//图片背景
			pChild = pNode->first_node("Image");
			if (nullptr != pChild)
				return LoadPictureBackground(pChild);

			//渐变色背景
			pChild = pNode->first_node("Fade");
			if (nullptr != pChild)
				return LoadFadeBackground(pChild);

			//单色背景
			pChild = pNode->first_node("Solid");
			if (nullptr != pChild)
				return LoadSolidBackground(pChild);
		}

		return backgroundPtr;
	}

	FontPtr LoadFont(CXmlNode *pNode)
	{
		FontPtr					fontPtr;
		std::string				strUsed;

		GetAttributeText(pNode, "used", strUsed);
		if (0 != strUsed.length())
			fontPtr = Singleton<ResourceMgr>::Instance()->GetFont(strUsed.c_str());

		if (nullptr == fontPtr.get())
		{
			std::string					strFace;

			if (GetAttributeText(pNode, "face", strFace))
			{
				int32_t		nHeight = AttributeAsInt(pNode, "height", 16);
				int32_t		nWeight = AttributeAsInt(pNode, "weight", 200);
				bool		bItalic = AttributeAsBool(pNode, "italic", false);
				bool		bUnderline = AttributeAsBool(pNode, "underLine", false);

				fontPtr.reset( new wxFont(
						wxSize(0, nHeight),
						wxFONTFAMILY_DEFAULT, 
						bItalic ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL, 
						(wxFontWeight)nWeight,
						bUnderline,
						wxString::FromUTF8(strFace)
						));
			}
		}
		
		return fontPtr;
	}

	PenPtr LoadPen(CXmlNode *pNode)
	{
		PenPtr				penPtr;
		int32_t				nStyle;
		std::string			strStyle;
		int32_t				nWidth;
		wxColor				crPen;

		nStyle = wxPENSTYLE_SOLID;
		if (GetAttributeText(pNode, "style", strStyle))
		{
			if (strStyle == "dash") {
				nStyle = wxPENSTYLE_SHORT_DASH;
			}
			else if (strStyle == "dot") {
				nStyle = wxPENSTYLE_DOT;
			}
		}

		nWidth = AttributeAsInt(pNode, "width", 1);
		if (GetAttributeColor(pNode, "color", crPen))
			penPtr.reset(new wxPen(crPen, nWidth, (wxPenStyle)nStyle));

		return penPtr;
	}

	void LoadWndBK(CXmlNode *pNode, WndBackground &backgroundRes)
	{
		CXmlNode			*pCur;
		
		for (pCur = pNode->first_node(); pCur != NULL; pCur = pCur->next_sibling())
		{
			//加载正常情况下的背景
			if (0 == _stricmp(pCur->name(), "Normal"))
			{
				backgroundRes.m_backgrounds[static_cast<int32_t>(ControlStatus::e_state_normal)].m_backgroundPtr = LoadBackground(pCur);
				continue;
			}

			//加载鼠标悬停情况下的背景
			if (0 == _stricmp(pCur->name(), "Hover"))
			{
				backgroundRes.m_backgrounds[static_cast<int32_t>(ControlStatus::e_state_hover)].m_backgroundPtr = LoadBackground(pCur);
				continue;
			}

			//加载鼠标按下情况下的背景
			if (0 == _stricmp(pCur->name(), "Pushed"))
			{
				backgroundRes.m_backgrounds[static_cast<int32_t>(ControlStatus::e_state_pushed)].m_backgroundPtr = LoadBackground(pCur);
				continue;
			}

			//加载失去I/O输入能力情况下的背景
			if (0 == _stricmp(pCur->name(), "Disable"))
			{
				backgroundRes.m_backgrounds[static_cast<int32_t>(ControlStatus::e_state_disable)].m_backgroundPtr = LoadBackground(pCur);
				continue;
			}
		}
	}

	void LoadWndIcon(CXmlNode *pNode, WndIcon &iconRes)
	{
		CXmlNode			*pCur;

		iconRes.m_szIcon.x = AttributeAsInt(pNode, "width", 16);
		iconRes.m_szIcon.y = AttributeAsInt(pNode, "height", 16);
		
		for (pCur = pNode->first_node(); pCur != NULL; pCur = pCur->next_sibling())
		{
			//加载正常情况下的背景
			if (0 == _stricmp(pCur->name(), "Normal"))
			{
				iconRes.m_pictures[static_cast<int32_t>(ControlStatus::e_state_normal)] = LoadPicture(pCur);
				continue;
			}

			//加载鼠标悬停情况下的背景
			if (0 == _stricmp(pCur->name(), "Hover"))
			{
				iconRes.m_pictures[static_cast<int32_t>(ControlStatus::e_state_hover)] = LoadPicture(pCur);
				continue;
			}

			//加载鼠标按下情况下的背景
			if (0 == _stricmp(pCur->name(), "Pushed"))
			{
				iconRes.m_pictures[static_cast<int32_t>(ControlStatus::e_state_pushed)] = LoadPicture(pCur);
				continue;
			}

			//加载失去I/O输入能力情况下的背景
			if (0 == _stricmp(pCur->name(), "Disable"))
			{
				iconRes.m_pictures[static_cast<int32_t>(ControlStatus::e_state_disable)] = LoadPicture(pCur);
				continue;
			}
		}
	}

	void LoadWndTextFont(CXmlNode *pNode, WndTextConfig &fontRes)
	{
		CXmlNode			*pCur;
		wxColor				crText;

		for (pCur = pNode->first_node(); pCur != NULL; pCur = pCur->next_sibling())
		{
			//加载正常情况下的背景
			if (0 == _stricmp(pCur->name(), "Normal"))
			{
				fontRes.m_font[static_cast<int32_t>(ControlStatus::e_state_normal)] = LoadFont(pCur);
				if (GetAttributeColor(pCur, "color", crText))
					fontRes.m_color[static_cast<int32_t>(ControlStatus::e_state_normal)] = crText;
				continue;
			}

			//加载鼠标悬停情况下的背景
			if (0 == _stricmp(pCur->name(), "Hover"))
			{
				fontRes.m_font[static_cast<int32_t>(ControlStatus::e_state_hover)] = LoadFont(pCur);
				if (GetAttributeColor(pCur, "color", crText))
					fontRes.m_color[static_cast<int32_t>(ControlStatus::e_state_hover)] = crText;
				continue;
			}

			//加载鼠标按下情况下的背景
			if (0 == _stricmp(pCur->name(), "Pushed"))
			{
				fontRes.m_font[static_cast<int32_t>(ControlStatus::e_state_pushed)] = LoadFont(pCur);
				if (GetAttributeColor(pCur, "color", crText))
					fontRes.m_color[static_cast<int32_t>(ControlStatus::e_state_pushed)] = crText;
				continue;
			}

			//加载失去I/O输入能力情况下的背景
			if (0 == _stricmp(pCur->name(), "Disable"))
			{
				fontRes.m_font[static_cast<int32_t>(ControlStatus::e_state_disable)] = LoadFont(pCur);
				if (GetAttributeColor(pCur, "color", crText))
					fontRes.m_color[static_cast<int32_t>(ControlStatus::e_state_disable)] = crText;
				continue;
			}
		}
	}

	void LoadGlobalResource(CXmlNode *pNode)
	{
		CXmlNode				*pCur;
		const char				*lpName;
		std::string					strName;
		
		for (pCur = pNode->first_node(); NULL != pCur; pCur = pCur->next_sibling())
		{
			lpName = pCur->name();
			if (0 == _stricmp(lpName, "Pen"))
			{
				PenPtr			penPtr = LoadPen(pCur);

				GetAttributeText(pCur, "name", strName);
				Singleton<ResourceMgr>::Instance()->AddPenRes(strName, std::move(penPtr));
				continue;
			}

			if (0 == _stricmp(lpName, "Font"))
			{
				FontPtr			fontPtr = LoadFont(pCur);

				GetAttributeText(pCur, "name", strName);
				Singleton<ResourceMgr>::Instance()->AddFontRes(strName, std::move(fontPtr));
				continue;
			}

			if (0 == _stricmp(pCur->name(), "Background"))
			{
				BackgroundPtr	backgroundPtr = LoadBackground(pCur);
				
				GetAttributeText(pCur, "name", strName);
				LOG_DEBUG << "PictureBackground[" << backgroundPtr.get() << "] name:" << strName;
				Singleton<ResourceMgr>::Instance()->AddBackgroundRes(strName, std::move(backgroundPtr));
				continue;
			}

			if (0 == _stricmp(pCur->name(), "Picture"))
			{
				PicturePtr					picturePtr;

				picturePtr = LoadPicture(pCur);
				GetAttributeText(pCur, "name", strName);
				Singleton<ResourceMgr>::Instance()->AddPictureRes(strName, std::move(picturePtr));
				continue;
			}
			
		}
	}

	void LoadControlInfo(CXmlNode *pNode)
	{
		std::string					strName;
					
		if (GetAttributeText(pNode, "name", strName))
		{
			CXmlNode				*pCur;
			ControlResPtr			controlResPtr(new ControlRes);

			for (pCur = pNode->first_node(); NULL != pCur; pCur = pCur->next_sibling())
			{
				if (0 == _stricmp(pCur->name(), "Font"))
				{
					WndTextConfig			textRes;

					LoadWndTextFont(pCur, textRes);
					controlResPtr->UpdateTextConfig(std::move(textRes));
					continue;
				}

				if (0 == _stricmp(pCur->name(), "Background"))
				{
					WndBackground			backgroundRes;

					LoadWndBK(pCur, backgroundRes);
					controlResPtr->UpdateBackground(std::move(backgroundRes));
					continue;
				}

				if (0 == _stricmp(pCur->name(), "Icon"))
				{
					WndIcon					iconRes;

					LoadWndIcon(pCur, iconRes);
					controlResPtr->UpdateIcon(std::move(iconRes));
					continue;
				}
			}

			Singleton<ResourceMgr>::Instance()->AddControlRes(strName, std::move(controlResPtr));
		}
	}

	bool LoadUIResource(CXmlNode *pNode)
	{
		CXmlNode				*pCur;
		const char				*lpName;
		
		for (pCur = pNode->first_node(); NULL != pCur; pCur = pCur->next_sibling())
		{
			lpName = pCur->name();

			if (0 == _stricmp(lpName, "Global"))
			{
				LoadGlobalResource(pCur);
				continue;
			}

			if (0 == _stricmp(lpName, "ControlRes"))
			{
				LoadControlInfo(pCur);
				continue;
			}
		}

		LOG_DEBUG << "================================== LoadUIResource ==================================";
		return TRUE;
	}

	bool LoadResource(const char* pFileName)
	{
		std::string						strFileName = UTF8ToLocal(pFileName);
		rapidxml::file<char>			fdoc(strFileName.c_str());
		rapidxml::xml_document<char>	doc;
		CXmlNode						*pRoot;
		CXmlNode						*pResource;

		try
		{
			doc.parse<0>(fdoc.data());
		}
		catch (const rapidxml::parse_error& ex)
		{
			LOG_ERROR << "invalidate xml file:" << pFileName <<",where:"<< ex.where<char>();
			throw ex;
		}
		catch(const std::exception& e)
		{	
			LOG_ERROR << "invalidate xml file:" << pFileName;
			throw e;
		}
		 
	
		pRoot = doc.first_node();
		if (nullptr == pRoot)
		{
			LOG_ERROR << "invalidate xml file:" << pFileName;
			return false;
		}

		pResource = pRoot->first_node("Resource");
		if (nullptr == pResource)
		{
			LOG_ERROR << "xml file not found resource node";
			return false;
		}

		return LoadUIResource(pResource);
	}
}