#include "custom_widgets.h"
#include "singleton.h"
#include "resource_manager.h"
#include <cmath>
#include "util.h"

namespace why
{
	CustomWidgets::CustomWidgets(wxWindow* pOwner)
		:m_pOwner(pOwner)
	{
	}

	CustomWidgets::~CustomWidgets()
	{
	}

	void CustomWidgets::Refresh()
	{
		if (m_pOwner) {
			m_pOwner->Refresh();
		}
	}

	void CustomWidgets::OnDraw(wxDC& dc)
	{
		std::vector<BaseInfo*> list;

		Sort(list);

		for (auto iter = list.rbegin(); iter != list.rend(); iter++)
		{
			switch ((*iter)->m_type)
			{
			case InfoType::eBackground:
				DrawBackgroundElements(dc, *iter);
				break;
			case InfoType::eIcon:
				DrawIconElements(dc, *iter);
				break;
			case InfoType::eShape:
				DrawShapeElements(dc, *iter);
				break;
			case InfoType::eLine:
				DrawLineElements(dc, *iter);
				break;
			case InfoType::eLineArc:
				DrawLineArcElements(dc, *iter);
				break;
			case InfoType::eText:
				DrawTextElements(dc, *iter);
				break;
			default:
				break;
			}
		}
	}

	void CustomWidgets::Sort(std::vector<BaseInfo*>& list)
	{
		int32_t count = 0;

		for (auto it = m_backgroundElements.begin(); it != m_backgroundElements.end(); it++)
		{
			(*it)->m_default_z = ++count;
			list.push_back((*it).get());
		}
		for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
		{
			(*it)->m_default_z = ++count;
			list.push_back((*it).get());
		}
		for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
		{
			(*it)->m_default_z = ++count;
			list.push_back((*it).get());
		}
		for (auto it = m_lineElements.begin(); it != m_lineElements.end(); it++)
		{
			(*it)->m_default_z = ++count;
			list.push_back((*it).get());
		}
		for (auto it = m_lineArcElements.begin(); it != m_lineArcElements.end(); it++)
		{
			(*it)->m_default_z = ++count;
			list.push_back((*it).get());
		}
		for (auto it = m_shapeElements.begin(); it != m_shapeElements.end(); it++)
		{
			(*it)->m_default_z = ++count;
			list.push_back((*it).get());
		}

		std::sort(list.begin(), list.end(), [](BaseInfo* a, BaseInfo* b)
			{
				if (a->m_index > b->m_index)
				{
					return true;
				}
				else if (a->m_index == b->m_index)
				{
					if ((int32_t)a->m_type > (int32_t)b->m_type)
					{
						return true;
					}
					else if ((int32_t)a->m_type == (int32_t)b->m_type)
					{
						if (a->m_default_z > b->m_default_z)
						{
							return true;
						}
					}
				}

				return false;
			});
	}

	void CustomWidgets::DrawBackgroundElements(wxDC& dc, BaseInfo* pInfo)
	{
		CustomBackgroundInfo* ptr = static_cast<CustomBackgroundInfo*>(pInfo);

		if (nullptr == ptr) return;

		if (!ptr->m_backgroundPtr || !ptr->m_show) return;

		ptr->m_backgroundPtr->Draw(dc, ptr->m_rc);
	}

	void CustomWidgets::DrawIconElements(wxDC& dc, BaseInfo* pInfo)
	{
		CustomIconInfo* ptr = static_cast<CustomIconInfo*>(pInfo);

		if (nullptr == ptr) return;

		if (!ptr->m_iconPtr || !ptr->m_show) return;

		if (ptr->m_rotate != 0)
		{
			ptr->m_iconPtr->DrawImage(dc, ptr->m_rcIcon.GetLeftTop(), ptr->m_rcIcon.GetSize(),
				ptr->m_rotate, ptr->m_rotatePoint);
		}
		else
		{
			ptr->m_iconPtr->DrawImage(dc, ptr->m_rcIcon.GetLeftTop(), ptr->m_rcIcon.GetSize());
		}
	}

	void CustomWidgets::DrawTextElements(wxDC& dc, BaseInfo* pInfo)
	{
		CustomTextInfo* ptr = static_cast<CustomTextInfo*>(pInfo);

		if (nullptr == ptr) return;

		wxFont					ftDefault = m_pOwner->GetFont();
		wxFont* pFont = &ftDefault;
		wxColour				crColor = m_pOwner->GetForegroundColour();

		if ((nullptr != m_pControlRes) && (nullptr != m_pControlRes->Font()))
		{
			pFont = m_pControlRes->Font();
			crColor = m_pControlRes->FontColor();
		}

		wxString& strTitle = ptr->m_strText;

		if (pFont && (!strTitle.empty()))
		{
			wxSize				szSize;
			wxPoint				ptPos;
			wxRect				rcText = ptr->m_rcText;

			if (nullptr != ptr->m_fontPtr.get()) {
				pFont = ptr->m_fontPtr.get();
			}

			crColor = ptr->m_crText;
			dc.SetFont(*pFont);
			dc.SetTextForeground(crColor);
			szSize = dc.GetTextExtent(strTitle);

			if (ptr->m_style & wxTE_CENTER)
			{
				ptPos.x = rcText.x + (rcText.width - szSize.x) / 2;
				ptPos.y = rcText.y + (rcText.height - szSize.y) / 2;
			}
			else if (ptr->m_style & wxTE_RIGHT)
			{
				ptPos.x = rcText.x + (rcText.width - szSize.x);
				ptPos.y = rcText.y + (rcText.height - szSize.y);
			}
			else
			{
				ptPos.x = rcText.x;
				ptPos.y = rcText.y;

				if (szSize.x > rcText.width && 2 * szSize.y <= rcText.height)
				{
					wxString tempTitle, tempTest;
					for (auto iter = strTitle.begin(); iter != strTitle.end(); iter++)
					{
						tempTest = tempTitle + *iter;
						szSize = dc.GetTextExtent(tempTest);
						if (szSize.x > rcText.width && !tempTitle.empty() && ptPos.y + 2 * szSize.y <= rcText.y + rcText.height)
						{
							dc.DrawText(tempTitle, ptPos);
							tempTitle.clear();
							ptPos.y += szSize.y;
						}
						tempTitle += *iter;
					}

					if (!tempTitle.empty()) {
						dc.DrawText(tempTitle, ptPos);
					}

					return;
				}
			}

			dc.DrawText(strTitle, ptPos);
		}
	}

	void CustomWidgets::DrawLineElements(wxDC& dc, BaseInfo* pInfo)
	{
		CustomLineInfo* ptr = static_cast<CustomLineInfo*>(pInfo);
		if (nullptr == ptr) return;

		if (ptr->m_penPtr && ptr->m_vector.size() >= 2)
		{
			dc.SetPen(*(ptr->m_penPtr.get()));
			dc.DrawLines(ptr->m_vector.size(), ptr->m_vector.data());
		}

		std::sort(ptr->m_subList.begin(), ptr->m_subList.end(), [](const CustomLineSub& a, const CustomLineSub& b)
			{
				if (a.m_index < b.m_index) {
					return true;
				}
				return false;
			});

		for (auto& iter : ptr->m_subList)
		{
			if (iter.m_penPtr && iter.m_vector.size() >= 2)
			{
				dc.SetPen(*(iter.m_penPtr.get()));
				dc.DrawLines(iter.m_vector.size(), iter.m_vector.data());
			}
		}
	}

	void CustomWidgets::DrawLineArcElements(wxDC& dc, BaseInfo* pInfo)
	{
		CustomLineArcInfo* ptr = static_cast<CustomLineArcInfo*>(pInfo);
		if (nullptr == ptr) return;

		if (ptr->m_penPtr && false == AreEqual(ptr->m_startEnd.first, ptr->m_startEnd.second))
		{
			dc.SetPen(*(ptr->m_penPtr.get()));
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.DrawEllipticArc(ptr->m_leftTop, ptr->m_size, ptr->m_startEnd.first, ptr->m_startEnd.second);
		}

		std::sort(ptr->m_subList.begin(), ptr->m_subList.end(), [](const CustomLineArcSub& a, const CustomLineArcSub& b)
			{
				if (a.m_index < b.m_index) {
					return true;
				}
				return false;
			});

		for (auto& iter : ptr->m_subList)
		{
			if (iter.m_penPtr && false == AreEqual(iter.m_startEnd.first, iter.m_startEnd.second))
			{
				dc.SetPen(*(iter.m_penPtr.get()));
				dc.SetBrush(*wxTRANSPARENT_BRUSH);
				dc.DrawEllipticArc(iter.m_leftTop, iter.m_size, iter.m_startEnd.first, iter.m_startEnd.second);
			}
		}
	}

	void CustomWidgets::DrawShapeElements(wxDC& dc, BaseInfo* pInfo)
	{
		CustomShapeInfo* ptr = static_cast<CustomShapeInfo*>(pInfo);

		if (nullptr == ptr) return;

		if (ptr->m_hasBrush)
		{
			dc.SetBrush(ptr->m_brush);
			dc.SetPen(wxPen(ptr->m_colour, 1));
		}
		else
		{
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.SetPen(wxPen(ptr->m_colour, 1));
		}

		if (CustomWidgetsBase::circle == ptr->m_shape)
		{
			wxMemoryDC* pMemDC = dynamic_cast<wxMemoryDC*>(&dc);
			std::shared_ptr<wxGraphicsContext> gcPtr;

			if (nullptr != pMemDC) {
				gcPtr.reset(wxGraphicsContext::Create(*pMemDC));
			}

			if (gcPtr)
			{
				gcPtr->SetAntialiasMode(wxANTIALIAS_DEFAULT);
				gcPtr->SetPen(wxPen(ptr->m_colour, 1));
				gcPtr->SetBrush(ptr->m_hasBrush ? ptr->m_brush : *wxTRANSPARENT_BRUSH);
				gcPtr->DrawEllipse(ptr->m_pt.x, ptr->m_pt.y, ptr->m_size.GetWidth(), ptr->m_size.GetHeight());
			}
			else
			{
				dc.DrawEllipse(ptr->m_pt, ptr->m_size);
			}
		}
		else if (CustomWidgetsBase::square == ptr->m_shape)
		{
			dc.DrawRectangle(ptr->m_pt, ptr->m_size);
		}
	}
}