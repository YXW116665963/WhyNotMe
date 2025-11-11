#pragma once

#include "gui_import.h"
#include "resource_define.h"
#include "Picture.h"

namespace why
{
	class BackgroundRes
	{
	public:
		BackgroundRes() : m_backgroundPtr(nullptr, [](IBackground *pPicture) { if (pPicture) pPicture->Destroy(); })
		{

		}

	public:
		BackgroundPtr		m_backgroundPtr;
	};

	class WndBackground
	{
	public:
		static const int32_t m_nStateSize = static_cast<int32_t>(ControlStatus::e_state_disable) + 1;

	public:
		typedef std::shared_ptr<IBackground> BackgroundPtr;

	public:
		WndBackground()
		{
		
		}

		WndBackground(const WndBackground &other) = delete;

		WndBackground(WndBackground &&other)
		{
			for (int32_t i = 0; i < m_nStateSize; i++)
				m_backgrounds[i].m_backgroundPtr = std::move(other.m_backgrounds[i].m_backgroundPtr);
		}

		~WndBackground()
		{
			
		}

		const WndBackground& operator=(const WndBackground &other) = delete;

		WndBackground& operator=(WndBackground &&other)
		{
			if (this != &other)
			{
				for (int32_t i = 0; i < m_nStateSize; i++)
					m_backgrounds[i].m_backgroundPtr = std::move(other.m_backgrounds[i].m_backgroundPtr);
			}

			return *this;
		}

		void Clear()
		{
			for (int32_t i = 0; i < m_nStateSize; i++)
				m_backgrounds[i].m_backgroundPtr.reset();
		}

	public:
		BackgroundRes		m_backgrounds[m_nStateSize];
	};

	/*!
	*@class WndTextConfig
	*@brief 窗口文本设定
	*/
	class WndTextConfig
	{
	public:
		static const int32_t m_nStateSize = static_cast<int32_t>(ControlStatus::e_state_disable) + 1;

	public:
		WndTextConfig()
		{

		}

		WndTextConfig(const WndTextConfig &other) = delete;

		WndTextConfig(WndTextConfig &&other)
		{
			for (int32_t i = 0; i < m_nStateSize; i++)
			{
				m_font[i] = std::move(other.m_font[i]);
				m_color[i] = other.m_color[i];
			}
		}

		~WndTextConfig()
		{
			
		}

		const WndTextConfig& operator=(const WndTextConfig &other) = delete;

		WndTextConfig& operator=(WndTextConfig &&other)
		{
			if (this != &other)
			{
				for (int32_t i = 0; i < m_nStateSize; i++)
				{
					m_font[i].swap(other.m_font[i]);
					m_color[i] = other.m_color[i];
				}
			}

			return *this;
		}

		void Clear()
		{
			for (int32_t i = 0; i < m_nStateSize; i++)
			{
				m_font[i].reset();
				m_color[i].Set(0);
			}
		}
	
	public:
		FontPtr			m_font[m_nStateSize];
		wxColor			m_color[m_nStateSize];
	};

	class WndIcon
	{
	public:
		static const int32_t m_nStateSize = static_cast<int32_t>(ControlStatus::e_state_disable) + 1;

	public:
		WndIcon()
		{
			m_szIcon.x = 0;
			m_szIcon.y = 0;
		}

		WndIcon(const WndIcon &other) = delete;

		WndIcon(WndIcon &&other)
		{
			m_szIcon.x = other.m_szIcon.x;
			m_szIcon.y = other.m_szIcon.y;
			for (int32_t i = 0; i < m_nStateSize; i++)
				m_pictures[i].swap(other.m_pictures[i]);
		}

		~WndIcon()
		{
			
		}

		const WndIcon& operator=(const WndIcon &other) = delete;

		WndIcon& operator=(WndIcon &&other)
		{
			if (this != &other)
			{
				m_szIcon.x = other.m_szIcon.x;
				m_szIcon.y = other.m_szIcon.y;
				for (int32_t i = 0; i < m_nStateSize; i++)
					m_pictures[i].swap(other.m_pictures[i]);
			}

			return *this;
		}

		void Clear()
		{
			
		}

	public:
		wxSize						m_szIcon;
		PicturePtr					m_pictures[m_nStateSize];
	};

	class ControlRes : public IControlRes
	{
	public:
		ControlRes();
		~ControlRes();

	public:
		ControlStatus GetStatus() override;
		void SetStatus(ControlStatus eStatus) override;
		IBackground* Background() override;
		wxFont* Font() override;
		const wxColour& FontColor() const override;

	public:
		void UpdateBackground(WndBackground &&background);
		void UpdateIcon(WndIcon &&icon);
		void UpdateTextConfig(WndTextConfig &&textConfig);

	private:
		ControlStatus				m_eStatus;				//!< 空间状态
		WndBackground				m_bk;					//!< 控件背景
		WndIcon						m_icon;					//!< 控件的图标
		WndTextConfig				m_text;					//!< 控件文本设定
	};
}
