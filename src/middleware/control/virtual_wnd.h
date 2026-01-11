#pragma once

#include "resource_define.h"
#include "ui_struct_def.h"

namespace why
{
	class VirtualWndOwner;
	class VirtualWnd : public wxEvtHandler
	{
	public:
		VirtualWnd(VirtualWndOwner* pOwner, const wxString& label, const wxRect& rcRect, const wxString& name);
		~VirtualWnd() = default;

	public:
		VirtualWndOwner* Owner();
		wxString& Name();
		void SetLabel(const wxString& label);
		wxString GetLabel() const;
		bool Show(bool bShow);
		bool IsShow() const;
		wxPoint GetPosition();
		wxSize GetSize();
		wxRect GetRect();
		void SetSize(int x, int y, int width, int height);
		void SetSize(const wxRect& rect);
		ControlStatus GetControlStatus() const;
		void SetControlStatus(ControlStatus eStatus);
		void SetControlRes(const std::string& strControlRes);
		const wxColour& GetBackgroundColour() const;
		const wxColour& GetForegroundColour() const;
		const wxFont& GetFont() const;
		void SetBackgroundColour(const wxColour& colour);
		void SetForegroundColour(const wxColour& colour);
		void SetFont(const wxFont& ftText);
		void Refresh();
		virtual bool AcceptMouseEvent() { return false; }
		virtual void OnPaint(wxGraphicsContext* pGraphic);
		virtual bool OnLButtonDown(wxMouseEvent& event) { return false; }
		virtual void OnLButtonUp(wxMouseEvent& event) {}
		virtual void OnMouseMove(wxMouseEvent& event) {}
		virtual void OnEnterWindow(wxMouseEvent& event) {}
		virtual void OnLeaveWindow(wxMouseEvent& event) {}
		virtual bool OnLButtonDClick(wxMouseEvent& event) { return false; }

	private:
		struct DirtyRegion
		{
			wxPoint pos{ 0, 0 };
			wxSize	size{ 0, 0 };

			void SetRegion(const wxPoint &pt, const wxSize &sizeParam)
			{
				this->pos = pt;
				this->size = sizeParam;
			}
		};

	private:
		VirtualWndOwner				*m_pOwner;
		wxString					m_label;
		wxPoint						m_pos;
		wxSize						m_size;
		wxString					m_name;
		bool						m_show{ false };
		ControlStatus				m_eStatus{ ControlStatus::e_state_normal };
		IControlRes*				m_pControlRes{ nullptr };
		wxBrush						m_brBackground;
		wxColour					m_crBackground;
		wxColour					m_crForeground;
		wxFont						m_ftText;
		wxColour					m_crCurrentTextColor;
		DirtyRegion                 m_dirtyRegion;
	};
}