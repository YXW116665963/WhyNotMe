#include "panel_graphic.h"
#include "panel_graphic_element.h"
#include "resource_manager.h"
#include "singleton.h"

namespace why
{
	PanelGraphic::PanelGraphic()
	{

	}

	PanelGraphic::~PanelGraphic()
	{

	}

	IPanelGraphicElement* PanelGraphic::AddLine(const std::string& strResName, const wxPoint& ptStart, const wxPoint& ptEnd, std::string id)
	{
		IPanelGraphicElement* pElement = nullptr;
		PenPtr penPtr = Singleton<ResourceMgr>::Instance()->GetPen(strResName.c_str());

		if (nullptr != penPtr.get())
		{
			GraphicElementPtr elementPtr(new LineGraphic(penPtr, ptStart, ptEnd, id));

			pElement = elementPtr.get();
			m_elementList.push_back(std::move(elementPtr));
			m_bNeedDraw = true;
		}

		return pElement;
	}

	void PanelGraphic::ChangeLineRect(const std::string& id, const wxPoint& ptStart, const wxPoint& ptEnd)
	{
		for (auto& it : m_elementList)
		{
			if (it->ID() == id)
			{
				LineGraphic* p = (LineGraphic*)it.get();
				p->UpdateRect(wxRect(ptStart, ptEnd));
				m_bNeedDraw = true;
				break;
			}
		}
	}

	void PanelGraphic::OffsetLineRect(const std::vector<std::string>& id, const wxPoint& ptStart, const wxPoint& ptEnd)
	{
		for (auto& itID : id)
		{
			for (auto& it : m_elementList)
			{
				if (it->ID() == itID)
				{
					LineGraphic* p = (LineGraphic*)it.get();
					p->OffsetRect(wxRect(ptStart, ptEnd));
					m_bNeedDraw = true;
					break;
				}
			}
		}
	}

	IPanelGraphicElement* PanelGraphic::AddPicture(const std::string& strResName, const wxRect& rcRect, std::string id)
	{
		IPanelGraphicElement* pElement = nullptr;
		PicturePtr				picturePtr = Singleton<ResourceMgr>::Instance()->GetPicture(strResName.c_str());

		if (nullptr != picturePtr.get())
		{
			GraphicElementPtr		elementPtr(new PictureGraphic(picturePtr, rcRect, id));

			pElement = elementPtr.get();
			m_elementList.push_back(std::move(elementPtr));
			m_bNeedDraw = true;
		}

		return pElement;
	}

	void PanelGraphic::ChangePictureRect(const std::string& id, const wxRect& rcRect)
	{
		for (auto& it : m_elementList)
		{
			if (it->ID() == id)
			{
				PictureGraphic* p = (PictureGraphic*)it.get();
				p->UpdateRect(rcRect);
				m_bNeedDraw = true;
				break;
			}
		}
	}

	void PanelGraphic::OffsetPictureRect(const std::vector<std::string>& id, const wxRect& rcRect)
	{
		for (auto& itID : id)
		{
			for (auto& it : m_elementList)
			{
				if (it->ID() == itID)
				{
					PictureGraphic* p = (PictureGraphic*)it.get();
					p->OffsetRect(rcRect);
					m_bNeedDraw = true;
					break;
				}
			}
		}
	}

	IPanelGraphicElement* PanelGraphic::AddBackground(const std::string& strResName, const wxRect& rcRect, std::string id)
	{
		IPanelGraphicElement* pElement = nullptr;
		BackgroundPtr			backgroundPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strResName.c_str());

		if (nullptr != backgroundPtr.get())
		{
			GraphicElementPtr		elementPtr(new BackgroudGraphic(backgroundPtr, rcRect, id));

			pElement = elementPtr.get();
			m_elementList.push_back(std::move(elementPtr));
			m_bNeedDraw = true;
		}

		return pElement;
	}

	void PanelGraphic::ChangeBackgroundRect(const std::string& id, const wxRect& rcRect)
	{
		for (auto& it : m_elementList)
		{
			if (it->ID() == id)
			{
				BackgroudGraphic* p = (BackgroudGraphic*)it.get();
				p->UpdateRect(rcRect);
				m_bNeedDraw = true;
				break;
			}
		}
	}

	void PanelGraphic::OffsetBackgroundRect(const std::vector<std::string>& id, const wxRect& rcRect)
	{
		for (auto& itID : id)
		{
			for (auto& it : m_elementList)
			{
				if (it->ID() == itID)
				{
					BackgroudGraphic* p = (BackgroudGraphic*)it.get();
					p->OffsetRect(rcRect);
					m_bNeedDraw = true;
					break;
				}
			}
		}
	}

	void PanelGraphic::ChangeBackground(const std::string& id, const std::string& strResName)
	{
		BackgroundPtr			backgroundPtr = Singleton<ResourceMgr>::Instance()->GetBackground(strResName.c_str());

		if (!backgroundPtr) return;

		for (auto& it : m_elementList)
		{
			if (it->ID() == id)
			{
				BackgroudGraphic* p = (BackgroudGraphic*)it.get();
				p->ChangeBackground(backgroundPtr);
				m_bNeedDraw = true;
				break;
			}
		}
	}

	IPanelGraphicElement* PanelGraphic::AddText(const std::string& strResName, const wxColor& crColor, const wxString& strText, const wxRect& rcRect, std::string id, uint32_t style)
	{
		IPanelGraphicElement* pElement = nullptr;
		FontPtr					fontPtr = Singleton<ResourceMgr>::Instance()->GetFont(strResName.c_str());

		if (nullptr != fontPtr.get())
		{
			GraphicElementPtr		elementPtr(new TextGraphic(fontPtr, crColor, strText, rcRect, id, style));

			pElement = elementPtr.get();
			m_elementList.push_back(std::move(elementPtr));
			m_bNeedDraw = true;
		}

		return pElement;
	}

	void PanelGraphic::ChangeTextRect(const std::string& id, const wxRect& rcRect)
	{
		for (auto& it : m_elementList)
		{
			if (it->ID() == id)
			{
				TextGraphic* p = (TextGraphic*)it.get();
				p->UpdateRect(rcRect);
				m_bNeedDraw = true;
				break;
			}
		}
	}

	void PanelGraphic::OffsetTextRect(const std::vector<std::string>& id, const wxRect& rcRect)
	{
		for (auto& itID : id)
		{
			for (auto& it : m_elementList)
			{
				if (it->ID() == itID)
				{
					TextGraphic* p = (TextGraphic*)it.get();
					p->OffsetRect(rcRect);
					m_bNeedDraw = true;
					break;
				}
			}
		}
	}

	uint32_t PanelGraphic::ElementCount()
	{
		return (uint32_t)m_elementList.size();
	}

	void PanelGraphic::DrawGraphic(wxDC& dc)
	{
		if (m_bNeedDraw)
		{
			for (auto it = m_elementList.begin(); it != m_elementList.end(); it++)
			{
				if ((*it)->IsShow())
					(*it)->Draw(dc);
			}

			m_bNeedDraw = false;
		}
	}

	void PanelGraphic::ShowElement(const std::string& elementID, bool bShow)
	{
		for (auto it = m_elementList.begin(); it != m_elementList.end(); it++)
		{
			if ((*it)->ID() == elementID)
			{
				if (bShow != (*it)->IsShow())
				{
					(*it)->Show(bShow);
					m_bNeedDraw = true;
					break;
				}
			}
		}
	}

	void PanelGraphic::ShowElements(const std::vector<std::string>& elementIDs, bool bShow)
	{
		uint32_t				uCount = (uint32_t)elementIDs.size();

		for (uint32_t i = 0; i < uCount; i++)
		{
			for (auto it = m_elementList.begin(); it != m_elementList.end(); it++)
			{
				if ((*it)->ID() == elementIDs[i])
				{
					if (bShow != (*it)->IsShow())
					{
						(*it)->Show(bShow);
						m_bNeedDraw = true;
						break;
					}
				}
			}
		}
	}

	void PanelGraphic::Expand(const wxRect& rcExpand, bool bExpandH, bool bExpandV)
	{
		for (auto it = m_elementList.begin(); it != m_elementList.end(); it++)
			(*it)->Expand(rcExpand, bExpandH, bExpandV);
		m_bNeedDraw = true;
	}

	void PanelGraphic::Contract(const wxRect& rcContract, bool bContracH, bool bContracV)
	{
		for (auto it = m_elementList.begin(); it != m_elementList.end(); it++)
			(*it)->Contract(rcContract, bContracH, bContracV);
		m_bNeedDraw = true;
	}

	void PanelGraphic::HideAndOffsetElements(const std::string& strIncludeElement, bool bIsVertical)
	{
		auto itInclude = std::find_if(m_elementList.begin(), m_elementList.end(), [strIncludeElement](const GraphicElementPtr& ptr) {
			return ptr && ptr->ID() == strIncludeElement; });

		if (itInclude != m_elementList.end())
		{
			for (auto& it : m_elementList)
			{
				int32_t width = it->GetRect().GetWidth();
				int32_t heigh = it->GetRect().GetHeight();

				int32_t x = it->GetRect().GetX();
				int32_t y = it->GetRect().GetY();

				wxRect rectChild(x, y, width, heigh);
				wxRect rectIncludeElement((*itInclude)->GetRect().GetX(), (*itInclude)->GetRect().GetY(), (*itInclude)->GetRect().GetWidth(), (*itInclude)->GetRect().GetHeight());
				if (rectIncludeElement.Contains(rectChild))
				{
					it->Show(false);
				}
				else
				{
					if (bIsVertical)
					{
						if (y >= (*itInclude)->GetRect().GetY() + (*itInclude)->GetRect().GetHeight())
						{
							wxRect moveRect(it->GetRect().GetX(), it->GetRect().GetY() - (*itInclude)->GetRect().GetHeight(), it->GetRect().GetWidth(), it->GetRect().GetHeight());
							if (it->IsShow())
							{
								it->UpdateRect(moveRect);		//区域内以下垂直向上移动
							}
						}
					}
					else
					{
						if (x >= (*itInclude)->GetRect().GetX() + (*itInclude)->GetRect().GetWidth())
						{
							wxRect moveRect(it->GetRect().GetX() - (*itInclude)->GetRect().GetWidth(), it->GetRect().GetY(), it->GetRect().GetWidth(), it->GetRect().GetHeight());
							if (it->IsShow())
							{
								it->UpdateRect(moveRect);		//区域内右水平向左移动
							}
						}
					}
				}
			}
		}
		m_bNeedDraw = true;
	}

	void PanelGraphic::ShowIncludeAreaElements(const std::string& strIncludeName, bool bShow)
	{
		auto itInclude = std::find_if(m_elementList.begin(), m_elementList.end(), [strIncludeName](const GraphicElementPtr& ptr) {
			return ptr && ptr->ID() == strIncludeName; });

		if (itInclude != m_elementList.end())
		{
			for (auto& it : m_elementList)
			{
				int32_t width = it->GetRect().GetWidth();
				int32_t heigh = it->GetRect().GetHeight();
				int32_t x = it->GetRect().GetX();
				int32_t y = it->GetRect().GetY();

				wxRect rectIncludeElement((*itInclude)->GetRect().GetX(), (*itInclude)->GetRect().GetY(), (*itInclude)->GetRect().GetWidth(), (*itInclude)->GetRect().GetHeight());
				wxRect rectChild(x, y, width, heigh);
				if (rectIncludeElement.Contains(rectChild))
				{
					it->Show(bShow);
				}
			}
		}
		m_bNeedDraw = true;
	}

	void PanelGraphic::OffsetIncludeAreaElement(const std::string& strIncludeName, wxRect rect)
	{
		auto itInclude = std::find_if(m_elementList.begin(), m_elementList.end(), [strIncludeName](const GraphicElementPtr& ptr) {
			return ptr && ptr->ID() == strIncludeName; });

		if (itInclude != m_elementList.end())
		{
			for (auto& it : m_elementList)
			{
				int32_t width = it->GetRect().GetWidth();
				int32_t heigh = it->GetRect().GetHeight();

				int32_t x = it->GetRect().GetX();
				int32_t y = it->GetRect().GetY();

				wxRect rectIncludeElement((*itInclude)->GetRect().GetX(), (*itInclude)->GetRect().GetY(), (*itInclude)->GetRect().GetWidth(), (*itInclude)->GetRect().GetHeight());
				wxRect rectChild(x, y, width, heigh);
				if (rectIncludeElement.Contains(rectChild))
				{
					if (it->IsShow())
					{
						it->OffsetRect(rect);
					}
				}
			}
		}
		m_bNeedDraw = true;
	}

}