#pragma once

#include "gui_import.h"
#include "data_center_interface.h"
#include "xml_util.h"
#include "picture.h"
#include "background_interface.h"
#include "control_res.h"

namespace why
{
	void SetDataCenter(IDataCenter* pDataCenter);
	void SetPicturePath(const char *pPicturePath, const char* pResourceTargetPath, const char* pDeviceModel, const char* pLanguageSuffix);
	void SetResourceSearchPath(const std::vector<std::string>& strSearchPath);
	bool SearchResourceFullPath(const std::string& strResourcePath, const std::string& strFileName, std::string& strLoadResourcePath);
	bool GetAttributeColor(CXmlNode* pNode, const char* pName, wxColour& colorValue);
	bool GetAttributePoint(CXmlNode* pNode, const char* pName, wxPoint& ptPoint);
	bool GetAttributeRect(CXmlNode* pNode, wxRect& rcRect);
	void GetAttributeMargin(CXmlNode* pNode, wxSize& szLeftTop, wxSize& szRightBottom);
	bool GetTextAttributeStyle(CXmlNode* pNode, const char* pName, uint32_t& style);
	PicturePtr GetAttributePicture(CXmlNode *pNode, const char *pName);

	PicturePtr LoadPicture(CXmlNode *pNode);
	BackgroundPtr LoadPictureBackground(CXmlNode *pNode);
	BackgroundPtr LoadFadeBackground(CXmlNode *pNode);
	BackgroundPtr LoadSolidBackground(CXmlNode *pNode);
	BackgroundPtr LoadBackground(CXmlNode *pNode);

	FontPtr LoadFont(CXmlNode *pNode);
	PenPtr LoadPen(CXmlNode *pNode);

	void LoadWndBK(CXmlNode *pNode, WndBackground &backgroundRes);
	void LoadWndIcon(CXmlNode *pNode, WndIcon &iconRes);
	void LoadWndTextFont(CXmlNode *pNode, WndTextConfig &fontRes);

	void LoadGlobalResource(CXmlNode *pNode);
	void LoadControlInfo(CXmlNode *pNode);
	bool LoadUIResource(CXmlNode *pNode);
	bool LoadResource(const char *pFileName);
	
}
