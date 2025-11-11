#pragma once

#include "gui_import.h"
#include "background_interface.h"
#include "picture.h"

namespace why
{
	typedef std::shared_ptr<wxPen> PenPtr;
	typedef std::shared_ptr<wxFont> FontPtr;

	enum class ControlStatus
	{
		e_state_normal = 0,		//正常情况
		e_state_hover,			//鼠标悬停
		e_state_pushed,			//鼠标按下
		e_state_disable,		//失效
	};

	class IControlRes
	{
	public:
		virtual ControlStatus GetStatus() = 0;
		virtual void SetStatus(ControlStatus status) = 0;
		virtual IBackground* Background() = 0;
		virtual wxFont* Font() = 0;
		virtual const wxColour& FontColor() const = 0;
	};

	class IResourceMgr
	{
	public:
		virtual PenPtr GetPen(const char* pName) = 0;
		virtual FontPtr GetFont(const char* pName) = 0;
		virtual PicturePtr GetPicture(const char* pName) = 0;
		virtual BackgroundPtr GetBackground(const char* pName) = 0;
		virtual IControlRes* FindControlRes(const char* pName) = 0;
	};
}
