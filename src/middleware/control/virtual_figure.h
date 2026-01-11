#pragma once

#include "virtual_wnd.h"
#include "resource_manager.h"

namespace why
{
	class VirtualFigure : public VirtualWnd
	{
	public:
		enum class FigureType
		{
			none,
			chip_rect,
			aim_wnd,
			cut_path,
			kerf_aim_wnd,
		};

		enum class Shape
		{
			none = 0,
			circle = 1,
			square = 2,
		};

	public:
		VirtualFigure(VirtualWndOwner* parent, const wxString& label, const wxRect& rcRect, const wxString& name);
		~VirtualFigure() = default;

		void SetFigure(FigureType eType);
		void SetMainPen(PenPtr& mainPen);
		void SetSubPen(PenPtr& subPen);

		void UpdateChipPosition(const std::string &chipName, const wxRect &rect);
		void UpdateChipPosition(const std::string &chipName, const std::vector<wxPoint> &pts);
		void UpdateCameraPosition(wxPoint pt);
		void RemoveAllChip();

	public:
		//IVirtualWnd interface
		void OnPaint(wxGraphicsContext* pGraphic) override;
		bool AcceptMouseEvent() override;
		bool OnLButtonDown(wxMouseEvent& event) override;
		void OnLButtonUp(wxMouseEvent& event)  override;
	private:
		void DrawChipRect(wxGraphicsContext* pGraphic);
		void DrawAimWnd(wxGraphicsContext* pGraphic);
		void DrawCutPath(wxGraphicsContext* pGraphic);

		void DrawChipName(wxGraphicsContext* pGraphic, const std::string &chipName, const wxPoint &pt);

		//<! 和导航图行为一致，绘制导航图中的摄像头位置
		void DrawCameraCenter(wxGraphicsContext* pGraphic);
		bool ClickInChip(wxPoint pt);

	private:
		struct ChipShapInfo
		{
			std::string chipName;
			Shape type{ Shape::none };
			wxRect rect{ 0,0,0,0 };
			std::vector<wxPoint> pts;

			ChipShapInfo(std::string chipName, wxRect rect)
			{
				this->chipName = chipName;
				this->type = Shape::circle;
				this->rect = rect;
			}

			ChipShapInfo(std::string chipName, const std::vector<wxPoint> & pts)
			{
				this->chipName = chipName;
				this->type = Shape::square;
				this->pts = pts;
			}
		};

	private:
		FigureType		m_eType{ FigureType::none };
		PenPtr			m_mainPenPtr;
		PenPtr			m_subPenPtr;
		bool			m_bMouseDown{ false };
		std::map<std::string, ChipShapInfo> m_chips;
		wxPoint         m_cameraPt;
	};
}