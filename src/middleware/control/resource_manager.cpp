#include "resource_manager.h"
#include "logger.h"

namespace why
{
	namespace
	{
		auto				g_backgroundDeleter = [](IBackground *pPicture) { if (pPicture) pPicture->Destroy(); };
	}

	ResourceMgr::ResourceMgr()
	{

	}

	ResourceMgr::~ResourceMgr()
	{

	}

	PenPtr ResourceMgr::GetPen(const char* pName)
	{
		auto		itFind = m_penPool.find(std::string(pName));

		if (itFind == m_penPool.end())
			return nullptr;

		return itFind->second;
	}

	FontPtr ResourceMgr::GetFont(const char* pName)
	{
		auto		itFind = m_fontPool.find(std::string(pName));

		if (itFind == m_fontPool.end())
			return nullptr;

		return itFind->second;
	}

	PicturePtr ResourceMgr::GetPicture(const char* pName)
	{
		if ((nullptr == pName) || (0 == strlen(pName)))
			return nullptr;

		auto		itFind = m_picturePool.find(std::string(pName));

		if (itFind == m_picturePool.end())
			return nullptr;

		return itFind->second;
	}

	BackgroundPtr ResourceMgr::GetBackground(const char* pName)
	{
		auto		itFind = m_backgroundPool.find(std::string(pName));

		if (itFind == m_backgroundPool.end())
			return BackgroundPtr(nullptr, g_backgroundDeleter);

		return itFind->second;
	}

	IControlRes* ResourceMgr::FindControlRes(const char* pName)
	{
		auto		itFind = m_controlResPool.find(std::string(pName));

		if (itFind == m_controlResPool.end())
			return nullptr;

		return itFind->second.get();
	}

	void ResourceMgr::AddPenRes(const std::string& strName, PenPtr&& penPtr)
	{
		if (nullptr == penPtr)
		{
			LOG_ERROR << "AddPenRes:" << strName << ", is nullptr";
			return;
		}

		auto		itFind = m_penPool.find(strName);

		if (itFind == m_penPool.end())
			m_penPool.insert(std::make_pair(strName, std::move(penPtr)));
		else
			itFind->second = std::move(penPtr);
	}

	void ResourceMgr::AddFontRes(const std::string &strName, FontPtr &&fontPtr)
	{
		if (nullptr == fontPtr)
		{
			LOG_ERROR << "AddFontRes:" << strName << ", is nullptr";
			return;
		}

		auto		itFind = m_fontPool.find(strName);

		if (itFind == m_fontPool.end())
			m_fontPool.insert(std::make_pair(strName, std::move(fontPtr)));
		else
			itFind->second = std::move(fontPtr);
	}

	void ResourceMgr::AddPictureRes(const std::string &strName, PicturePtr &&picturePtr)
	{
		if (nullptr == picturePtr)
		{
			LOG_ERROR << "AddPictureRes:" << strName << ", is nullptr";
			return;
		}

		auto		itFind = m_picturePool.find(strName);

		if (itFind == m_picturePool.end())
			m_picturePool.insert(std::make_pair(strName, std::move(picturePtr)));
		else
			itFind->second = std::move(picturePtr);
	}

	void ResourceMgr::AddBackgroundRes(const std::string &strName, BackgroundPtr &&backgroundPtr)
	{
		if (nullptr == backgroundPtr)
		{
			LOG_ERROR << "AddBackgroundRes:" << strName << ", is nullptr";
			return;
		}

		auto		itFind = m_backgroundPool.find(strName);

		if (itFind == m_backgroundPool.end())
			m_backgroundPool.insert(std::make_pair(strName, std::move(backgroundPtr)));
		else
			itFind->second = std::move(backgroundPtr);
	}

	void ResourceMgr::AddControlRes(const std::string &strName, ControlResPtr &&controlRes)
	{
		if (nullptr == controlRes)
		{
			LOG_ERROR << "AddControlRes:" << strName << ", is nullptr";
			return;
		}

		auto		itFind = m_controlResPool.find(strName);

		if (itFind == m_controlResPool.end())
			m_controlResPool.insert(std::make_pair(strName, std::move(controlRes)));
		else
			itFind->second = std::move(controlRes);
	}
}