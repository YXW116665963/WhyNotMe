#include "virtual_figure.h"
#include "singleton.h"
#include "resource_manager.h"
#include "virtual_wnd_owner.h"

namespace why
{
	VirtualFigure::VirtualFigure(VirtualWndOwner* parent, const wxString& label, const wxRect& rcRect, const wxString& name)
			: VirtualWnd(parent, label, rcRect, name)
	{
		
	}

	void VirtualFigure::SetFigure(FigureType eType)
	{
		m_eType = eType;
	}

	void VirtualFigure::SetMainPen(PenPtr& mainPen)
	{
		m_mainPenPtr = mainPen;
	}

	void VirtualFigure::SetSubPen(PenPtr& subPen)
	{
		m_subPenPtr = subPen;
	}

	void VirtualFigure::UpdateChipPosition(const std::string &chipName, const wxRect &rect)
	{
		auto iter = m_chips.find(chipName);
		if (iter == m_chips.end()){
			m_chips.emplace(chipName, ChipShapInfo(chipName, rect));
		}
		else{
			iter->second.rect = rect;
		}
		Refresh();
	}

	void VirtualFigure::UpdateChipPosition(const std::string &chipName, const std::vector<wxPoint> &pts)
	{
		auto iter = m_chips.find(chipName);
		if (iter == m_chips.end()){
			m_chips.emplace(chipName, ChipShapInfo(chipName, pts));
		}
		else{
			iter->second.pts = pts;
		}
		Refresh();
	}

	void VirtualFigure::UpdateCameraPosition(wxPoint pt)
	{
		Owner()->RefreshRect(wxRect(wxPoint(m_cameraPt.x - 15, m_cameraPt.y - 15), wxSize(31, 31)));
		Owner()->RefreshRect(wxRect(wxPoint(pt.x - 15, pt.y - 15), wxSize(31, 31)));
		m_cameraPt = pt;
		Refresh();
	}

	void VirtualFigure::RemoveAllChip()
	{
		m_chips.clear();
	}

	void VirtualFigure::OnPaint(wxGraphicsContext* pGraphic)
	{
		wxString				strTitle = GetLabel();

		VirtualWnd::OnPaint(pGraphic);

		switch (m_eType)
		{
			case FigureType::chip_rect:
				DrawChipRect(pGraphic);
				break;
			case FigureType::aim_wnd:
				DrawAimWnd(pGraphic);
				break;
			case FigureType::cut_path:
				DrawCutPath(pGraphic);
				break;
			default:
				break;
		}
	}

	bool VirtualFigure::AcceptMouseEvent()
	{
		if (FigureType::chip_rect == m_eType) {
			return true;
		}
		return false;
	}

	bool VirtualFigure::OnLButtonDown(wxMouseEvent& event)
	{
		if (false == ClickInChip(wxPoint(event.GetX(), event.GetY()))) {
			return false;
		}

		m_bMouseDown = true;
		wxMouseEvent			mouseEvent(wxEVT_LEFT_DOWN);

		mouseEvent.SetEventObject(this);
		AddPendingEvent(mouseEvent);
		return true;
	}

	void VirtualFigure::OnLButtonUp(wxMouseEvent& event)
	{
		if (m_bMouseDown)
		{
			wxMouseEvent			mouseEvent(wxEVT_LEFT_UP);
			wxCommandEvent			buttonClickEvent(wxEVT_BUTTON);
			wxPoint pt = event.GetPosition();

			mouseEvent.SetEventObject(this);
			AddPendingEvent(mouseEvent);

			buttonClickEvent.SetEventObject(this);
			buttonClickEvent.SetString(Name() + "," + std::to_string(pt.x) + "," + std::to_string(pt.y));
			AddPendingEvent(buttonClickEvent);
			m_bMouseDown = false;
		}
	}

	void VirtualFigure::DrawChipRect(wxGraphicsContext* pGraphic)
	{
		wxRect			rcRect = GetRect();

		if (!rcRect.IsEmpty())
		{
			pGraphic->SetBrush(*wxTRANSPARENT_BRUSH);
			pGraphic->SetPen(*m_mainPenPtr.get());

			for (auto iter = m_chips.begin(); iter != m_chips.end(); iter++)
			{
				if (Shape::square == iter->second.type)
				{
					pGraphic->StrokeLine(iter->second.pts[0].x, iter->second.pts[0].y, iter->second.pts[1].x, iter->second.pts[1].y);
					pGraphic->StrokeLine(iter->second.pts[1].x, iter->second.pts[1].y, iter->second.pts[2].x, iter->second.pts[2].y);
					pGraphic->StrokeLine(iter->second.pts[2].x, iter->second.pts[2].y, iter->second.pts[3].x, iter->second.pts[3].y);
					pGraphic->StrokeLine(iter->second.pts[3].x, iter->second.pts[3].y, iter->second.pts[0].x, iter->second.pts[0].y);
					
					wxPoint pt{ iter->second.pts[0].x, iter->second.pts[0].y };
					pt.x = min(pt.x, iter->second.pts[1].x);
					pt.x = min(pt.x, iter->second.pts[2].x);
					pt.x = min(pt.x, iter->second.pts[3].x);

					pt.y = min(pt.y, iter->second.pts[1].y);
					pt.y = min(pt.y, iter->second.pts[2].y);
					pt.y = min(pt.y, iter->second.pts[3].y);

					DrawChipName(pGraphic, iter->first, pt);
				}
				else if (Shape::circle == iter->second.type)
				{
					pGraphic->DrawEllipse(iter->second.rect.x, iter->second.rect.y,
						iter->second.rect.width, iter->second.rect.height);
					DrawChipName(pGraphic, iter->first, wxPoint(iter->second.rect.x, iter->second.rect.y));
				}
			}
			pGraphic->SetBrush(*wxTRANSPARENT_BRUSH);

			DrawCameraCenter(pGraphic);
		}
	}

	void VirtualFigure::DrawAimWnd(wxGraphicsContext* pGraphic)
	{
		wxRect			rcRect = GetRect();

		if (!rcRect.IsEmpty())
		{
			wxPoint		ptCenter;
			wxPoint		ptRightBottom;

			ptCenter.x = rcRect.x + rcRect.width / 2;
			ptCenter.y = rcRect.y + rcRect.height / 2;

			ptRightBottom.x = rcRect.x + rcRect.width;
			ptRightBottom.y = rcRect.y + rcRect.height;

			pGraphic->SetBrush(*wxTRANSPARENT_BRUSH);
			pGraphic->SetPen(*m_mainPenPtr.get());
			pGraphic->DrawRectangle(rcRect.x, rcRect.y, rcRect.width, rcRect.height);
			pGraphic->StrokeLine(rcRect.x, ptCenter.y, ptRightBottom.x, ptCenter.y);
			pGraphic->StrokeLine(ptCenter.x, rcRect.y, ptCenter.x, ptRightBottom.y);
			pGraphic->SetBrush(*wxTRANSPARENT_BRUSH);
		}
	}

	void VirtualFigure::DrawCutPath(wxGraphicsContext* pGraphic)
	{
		wxRect			rcRect = GetRect();

		if (!rcRect.IsEmpty())
		{
			wxPoint		ptCenter;
			wxPoint		ptRightBottom;
			int			nCrossHeight = 5;
			ptCenter.x = rcRect.x + rcRect.width / 2;
			ptCenter.y = rcRect.y + rcRect.height / 2;

			ptRightBottom.x = rcRect.x + rcRect.width;
			ptRightBottom.y = rcRect.y + rcRect.height;

			pGraphic->SetPen(*m_subPenPtr.get());
			pGraphic->StrokeLine(rcRect.x, rcRect.y, ptRightBottom.x, rcRect.y);
			pGraphic->StrokeLine(ptCenter.x, rcRect.y - nCrossHeight, ptCenter.x, rcRect.y + nCrossHeight);

			pGraphic->StrokeLine(rcRect.x, ptRightBottom.y, ptRightBottom.x, ptRightBottom.y);
			pGraphic->StrokeLine(ptCenter.x, ptRightBottom.y - nCrossHeight, ptCenter.x, ptRightBottom.y + nCrossHeight);

			if (m_mainPenPtr)
			{
				pGraphic->SetPen(*m_mainPenPtr.get());
				pGraphic->StrokeLine(rcRect.x, ptCenter.y, ptRightBottom.x, ptCenter.y);
				pGraphic->StrokeLine(ptCenter.x, ptCenter.y - nCrossHeight, ptCenter.x, ptCenter.y + nCrossHeight);

			}
		}
	}

	void VirtualFigure::DrawChipName(wxGraphicsContext* pGraphic, const std::string &chipName, const wxPoint &pt)
	{
		if (!chipName.empty())
		{
			wxFont					ftDefault = GetFont();
			wxColour				crColor = GetForegroundColour();

			pGraphic->SetFont(ftDefault, crColor);
			pGraphic->DrawText(chipName, pt.x, pt.y);
		}
	}

	void VirtualFigure::DrawCameraCenter(wxGraphicsContext* pGraphic)
	{
		/*   //<! 在区域外隐藏
		if (false == ClickInChip(m_cameraPt)) {
			return;
		}
		*/

		pGraphic->SetBrush(*wxTRANSPARENT_BRUSH);
		pGraphic->SetPen(*m_subPenPtr.get());

		pGraphic->StrokeLine(m_cameraPt.x - 15, m_cameraPt.y - 15, m_cameraPt.x - 2, m_cameraPt.y - 15);
		pGraphic->StrokeLine(m_cameraPt.x + 2, m_cameraPt.y - 15, m_cameraPt.x + 15, m_cameraPt.y - 15);
		pGraphic->StrokeLine(m_cameraPt.x - 15, m_cameraPt.y + 15, m_cameraPt.x - 2, m_cameraPt.y + 15);
		pGraphic->StrokeLine(m_cameraPt.x + 2, m_cameraPt.y + 15, m_cameraPt.x + 15, m_cameraPt.y + 15);

		pGraphic->StrokeLine(m_cameraPt.x - 15, m_cameraPt.y - 15, m_cameraPt.x - 15, m_cameraPt.y - 2);
		pGraphic->StrokeLine(m_cameraPt.x - 15, m_cameraPt.y + 2, m_cameraPt.x - 15, m_cameraPt.y + 15);
		pGraphic->StrokeLine(m_cameraPt.x + 15, m_cameraPt.y - 15, m_cameraPt.x + 15, m_cameraPt.y - 2);
		pGraphic->StrokeLine(m_cameraPt.x + 15, m_cameraPt.y + 1, m_cameraPt.x + 15, m_cameraPt.y + 15);

		pGraphic->StrokeLine(m_cameraPt.x, m_cameraPt.y - 13, m_cameraPt.x, m_cameraPt.y - 4);
		pGraphic->StrokeLine(m_cameraPt.x, m_cameraPt.y + 13, m_cameraPt.x, m_cameraPt.y + 4);
		pGraphic->StrokeLine(m_cameraPt.x - 13, m_cameraPt.y, m_cameraPt.x - 4, m_cameraPt.y);
		pGraphic->StrokeLine(m_cameraPt.x + 4, m_cameraPt.y, m_cameraPt.x + 13, m_cameraPt.y);

		pGraphic->SetBrush(*wxTRANSPARENT_BRUSH);
	}

	bool VirtualFigure::ClickInChip(wxPoint pt)
	{
		if (FigureType::chip_rect != m_eType) {
			return false;
		}

		for (auto iter = m_chips.begin(); iter != m_chips.end(); iter++)
		{
			if (Shape::square == iter->second.type)
			{
				int32_t left{ 0 }, top{ 0 }, right{ 0 }, bottom{ 0 };
				wxPoint center(0, 0);
				for (int32_t i = 0; i < 4; i++)
				{
					if (0 == i)
					{
						left = right = iter->second.pts[i].x;
						top = bottom = iter->second.pts[i].y;
						continue;
					}
					left = min(iter->second.pts[i].x, left);
					top = min(iter->second.pts[i].y, top);
					right = max(iter->second.pts[i].x, right);
					bottom = max(iter->second.pts[i].y, bottom);
				}

				if (pt.x < left || pt.y < top || pt.x >= right || pt.y >= bottom) {
					continue;
				}

				center.x = (left + right) / 2;
				center.y = (top + bottom) / 2;

				double radians = atan2((iter->second.pts[0].y + iter->second.pts[1].y) / 2 - center.y,
					(iter->second.pts[0].x + iter->second.pts[1].x) / 2 - center.x);

				for (int32_t i = 0; i < 4; i++)
				{
					if (0 == i)
					{
						left = right = (int32_t)((iter->second.pts[i].x - center.x)*cos(radians) - (iter->second.pts[i].y - center.y)*sin(radians));
						top = bottom = (int32_t)((iter->second.pts[i].x - center.x)*sin(radians) + (iter->second.pts[i].y - center.y)*cos(radians));
						continue;
					}

					left = min(left, (int32_t)((iter->second.pts[i].x - center.x)*cos(radians) - (iter->second.pts[i].y - center.y)*sin(radians)));
					top = min(top, (int32_t)((iter->second.pts[i].x - center.x)*sin(radians) + (iter->second.pts[i].y - center.y)*cos(radians)));
					right = max(right, (int32_t)((iter->second.pts[i].x - center.x)*cos(radians) - (iter->second.pts[i].y - center.y)*sin(radians)));
					bottom = max(bottom, (int32_t)((iter->second.pts[i].x - center.x)*sin(radians) + (iter->second.pts[i].y - center.y)*cos(radians)));
				}

				wxPoint pt2((int32_t)((pt.x - center.x)*cos(radians) - (pt.y - center.y)*sin(radians)),
					(int32_t)((pt.x - center.x)*sin(radians) + (pt.y - center.y)*cos(radians)));

				if (pt2.x >= left && pt2.y >= top && pt2.x < right && pt2.y < bottom) {
					return true;
				}
			}
			else if (Shape::circle == iter->second.type)
			{
				wxPoint center(0, 0);
				center.x = iter->second.rect.GetWidth() / 2 + iter->second.rect.x;
				center.y = iter->second.rect.GetHeight() / 2 + iter->second.rect.y;

				double length = sqrt(std::pow(pt.x - center.x, 2) + std::pow(pt.y - center.y, 2));
				if (length <= iter->second.rect.GetWidth() / 2) {
					return true;
				}
			}
		}

		return false;
	}
}