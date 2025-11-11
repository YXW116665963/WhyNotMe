#include "control_res.h"

namespace why
{
	ControlRes::ControlRes()
	{
		m_eStatus = ControlStatus::e_state_normal;
	}

	ControlRes::~ControlRes() 
	{

	}

	inline ControlStatus ControlRes::GetStatus()
	{
		return m_eStatus;
	}

	inline void ControlRes::SetStatus(ControlStatus eStatus)
	{
		m_eStatus = eStatus;
	}

	IBackground* ControlRes::Background()
	{
		IBackground*		pBackground = m_bk.m_backgrounds[static_cast<int32_t>(m_eStatus)].m_backgroundPtr.get();

		if ((nullptr == pBackground) && (ControlStatus::e_state_normal != m_eStatus))
			pBackground = m_bk.m_backgrounds[static_cast<int32_t>(ControlStatus::e_state_normal)].m_backgroundPtr.get();

		return pBackground;
	}

	wxFont* ControlRes::Font()
	{
		wxFont*		pFont = m_text.m_font[static_cast<int32_t>(m_eStatus)].get();

		if ((nullptr == pFont) && (ControlStatus::e_state_normal != m_eStatus))
			pFont = m_text.m_font[static_cast<int32_t>(ControlStatus::e_state_normal)].get();

		return pFont;
	}

	const wxColour& ControlRes::FontColor() const
	{ 
		return m_text.m_color[static_cast<int32_t>(m_eStatus)];
	}

	void ControlRes::UpdateBackground(WndBackground &&background)
	{  
		m_bk = std::move(background);
	}

	void ControlRes::UpdateIcon(WndIcon &&icon)
	{
		m_icon = std::move(icon);
	}

	void ControlRes::UpdateTextConfig(WndTextConfig &&textConfig)
	{
		m_text = std::move(textConfig);
	}
}