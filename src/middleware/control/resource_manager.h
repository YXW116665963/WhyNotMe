#pragma once

#include "resource_define.h"
#include "control_res.h"

#include <map>

namespace why
{
	typedef std::unique_ptr<ControlRes> ControlResPtr;

	class ResourceMgr : public IResourceMgr
	{
	public:
		ResourceMgr();
		~ResourceMgr();

	public:
		PenPtr GetPen(const char* pName) override;
		FontPtr GetFont(const char* pName) override;
		PicturePtr GetPicture(const char* pName) override;
		BackgroundPtr GetBackground(const char* pName) override;
		IControlRes* FindControlRes(const char* pName) override;
	
	public:
		void AddPenRes(const std::string& strName, PenPtr &&penPtr);
		void AddFontRes(const std::string &strName, FontPtr &&fontPtr);
		void AddPictureRes(const std::string &strName, PicturePtr &&picturePtr);
		void AddBackgroundRes(const std::string &strName, BackgroundPtr &&backgroundPtr);
		void AddControlRes(const std::string &strName, ControlResPtr &&controlRes);

	private:
		typedef std::map<std::string, PenPtr> PenPool;
		typedef std::map<std::string, FontPtr> FontPool;
		typedef std::map<std::string, PicturePtr> PicturePool;
		typedef std::map<std::string, BackgroundPtr> BackgroundPool;
		typedef std::map<std::string, ControlResPtr> ControlResPool;

	private:
		PenPool					m_penPool;
		FontPool				m_fontPool;
		PicturePool				m_picturePool;
		BackgroundPool			m_backgroundPool;
		ControlResPool			m_controlResPool;
	};
}
