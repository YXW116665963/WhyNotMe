#include "env_var_data_def.h"
#include "data_center_value_util.h"
#include "xml_util.h"
#include "child_panel.h"
#include "ui_loader.h"
#include "resource_manager.h"
#include "singleton.h"
#include "datas_modification_confirm_dlg.h"
#include "logger.h"
#include "util.h"
#include "timer.h"

namespace why
{
	extern IDataCenter* g_pMiddlewareDataCenter;

	ChildPanel::ChildPanel(const std::string& strResFile, wxWindow* parent, const wxPoint& pos, const wxSize& size, bool bBindData)
		: wxPanel(parent, wxID_ANY, pos, size)
	{
		LOG_INFO << "new ChildPanel:" << this << ", strResFile:" << strResFile;
		Show(false);

		if (bBindData)
		{
			m_dataExchangePtr.reset(new DataExchange(g_pMiddlewareDataCenter));
			Bind(wxEVT_CLOSE_WINDOW, &ChildPanel::OnCloseWindow, this);
		}

		LoadRenderRect(strResFile);

		LoadPanel(this, m_dataExchangePtr.get(), strResFile, &m_graphic);
		this->SetLabel(strResFile);

		// 绑定静态资源绘制
		if (0 != m_graphic.ElementCount())
		{
			Bind(wxEVT_PAINT, &ChildPanel::OnDrawPanelBackground, this);
			// wxEVT_ERASE_BACKGROUND：窗口滚动、被遮挡后重新露出、大小改变等触发，一般在wxEVT_PAINT之前
			Bind(wxEVT_ERASE_BACKGROUND, &ChildPanel::OnEraseBackground, this);
		}
		else
		{
			SetBackgroundColour(wxColor(240, 240, 240));
		}

		if (nullptr != m_dataExchangePtr.get())
			m_dataExchangePtr->Subscribe();
	}

	ChildPanel::~ChildPanel()
	{
		LOG_INFO << "~ChildPanel:" << this;
	}

	void ChildPanel::EnablePanelEx(bool bEnable)
	{
		wxWindowList& children = GetChildren();

		for (auto it = children.begin(); it != children.end(); it++)
		{
			if ((*it)->IsShown() && (*it)->IsEnabled())
			{
				(*it)->Enable(bEnable);
			}
		}
	}

	void ChildPanel::EnablePanel(bool bEnable)
	{
		wxWindowList& children = GetChildren();

		for (auto it = children.begin(); it != children.end(); it++)
		{
			if ((*it)->IsShown())
			{
				(*it)->Enable(bEnable);
			}
		}
	}

	wxWindow* ChildPanel::FindPanelItem(const std::string& strItemName)
	{
		return wxWindow::FindWindowByName(wxString::FromUTF8(strItemName), this);
	}

	void ChildPanel::OnCloseWindow(wxCloseEvent& event)
	{
		std::vector<DataExchange::ChangedData>		changedDatas = m_dataExchangePtr->GetChangedDatas();

		m_dataExchangePtr->Unsubscribe();
		//if (!changedDatas.empty())
		//{
		//	DatasModificationConfirmDlg		confirmDlg(changedDatas, this);
		//
		//	if (wxID_OK == confirmDlg.ShowModal())
		//		m_dataExchangePtr->Apply();
		//}

		Destroy();
	}

	void ChildPanel::CreateBufferBitmap()
	{
		wxSize			szClient = GetClientSize();

		if (!m_bufferBitmap.IsOk())
		{
			m_szBitmap = szClient;
			m_bufferBitmap = wxBitmap(szClient.x, szClient.y);
		}
		else
		{
			if ((m_szBitmap.x != szClient.x) || (m_szBitmap.y != szClient.y))
			{
				m_szBitmap = szClient;
				m_bufferBitmap = wxBitmap(szClient.x, szClient.y);
			}
		}
	}

	void ChildPanel::LoadRenderRect(std::string strFileName)
	{
		PathAppender pathAppender;
		std::string	strFullFileName = pathAppender.
			SetSourcePath(GetStringValue(envVar::g_Domain, envVar::strUIXml_dirPath)).
			AppendChildPath(strFileName).
			GetPath();

		{
			std::string						strXMLFileName = UTF8ToLocal(strFullFileName);
			rapidxml::file<char>			fdoc(strXMLFileName.c_str());
			rapidxml::xml_document<char>	doc;
			CXmlNode* pRoot = nullptr;
			CXmlNode* pRect = nullptr;
			wxPoint							szSize{ 0,0 };
			WndCoordinate					wndCoordinate;

			try
			{
				doc.parse<0>(fdoc.data());
			}
			catch (const rapidxml::parse_error& ex)
			{
				LOG_ERROR << "invalidate xml file:" << strFileName << ",where:" << ex.where<char>();
				throw ex;
			}
			catch (const std::exception& e)
			{
				LOG_ERROR << "invalidate xml file:" << strFileName;
				throw e;
			}
			pRoot = doc.first_node();
			if (nullptr == pRoot)
			{
				LOG_INFO << "invalidate xml file:" << strFileName;
				return;
			}

			pRect = pRoot->first_node("RenderRects");
			
			for (CXmlNode* pCur = pRect->first_node(); nullptr != pCur; pCur = pCur->next_sibling())
			{
				std::string strLpName = pCur->name();

				if (strLpName == "RenderRect")
				{
					std::string				strRectName = "";
					wxRect					rcRect;

					if (!GetAttributeText(pCur, "name", strRectName))
						return;
					if (!GetAttributeRect(pCur, rcRect))
						return;

					m_mapRenderRect.insert(std::make_pair(strRectName, rcRect));
				}
			}
		}
	}

	bool ChildPanel::GetRenderRect(std::string strRectName, wxRect& rect)
	{
		if (m_mapRenderRect.find(strRectName) != m_mapRenderRect.end())
		{
			rect = m_mapRenderRect[strRectName];
			return true;
		}
		LOG_ERROR << "find render_rect" << strRectName << "faild!";
		return false;
	}

	void ChildPanel::OnEraseBackground(wxEraseEvent& event)
	{
		
	}

	void ChildPanel::OnDrawPanelBackground(wxPaintEvent& event)
	{
		CreateBufferBitmap();
		if (m_bufferBitmap.IsOk())
		{
			wxPaintDC			dc(this);
			
			if (m_graphic.IsNeedDraw())
			{
				wxMemoryDC			memDC;

				memDC.SelectObject(m_bufferBitmap);
				memDC.Clear();
				m_graphic.DrawGraphic(memDC);
				memDC.SelectObject(wxNullBitmap);
			}
			dc.DrawBitmap(m_bufferBitmap, 0, 0, true);
			event.Skip(false);
		}
	}

	DataExchange* ChildPanel::GetDataExchange()
	{
		return m_dataExchangePtr.get();
	}

	PanelGraphic& ChildPanel::GetPanelGraphic()
	{
		return m_graphic;
	}

	void ChildPanel::SetWndDisplayMode(wxWindow* pWindow, DataExchange::DisplayMethon func)
	{
		if(pWindow)
			m_dataExchangePtr.get()->SetCustomDisplayMethod(pWindow, func);
	}

	void ChildPanel::RefreshWindow()
	{
		this->Refresh();
	}

	void ChildPanel::ShowButton(const std::vector<std::string>& buttons, bool bShow)
	{
		wxWindow			*pButton = nullptr;

		for (auto buttonName : buttons)
		{
			pButton = FindPanelItem(buttonName);
			if (pButton)
				pButton->Show(bShow);
		}
	}

	void ChildPanel::ShowElements(const std::vector<std::string>& elements, bool bShow)
	{
		m_graphic.ShowElements(elements, bShow);
	}

	bool ChildPanel::GetDlgItemText(const std::string &strName, std::string &strText)
	{
		wxWindow			*pDlgItem = FindPanelItem(strName);

		if (nullptr == pDlgItem)
			return false;

		wxTextCtrl* pEdit = dynamic_cast<wxTextCtrl*>(pDlgItem);
		if (nullptr != pEdit)
		{
			strText = pEdit->GetValue();
			return true;
		}

		wxString			strLabel = pDlgItem->GetLabel();

		strText = strLabel.ToStdString();
		return true;
	}

	void ChildPanel::SetDlgItemText(const std::string &strName, const std::string &strText)
	{
		wxWindow			*pDlgItem = FindPanelItem(strName);

		if (nullptr != pDlgItem)
			pDlgItem->SetLabel(wxString::FromUTF8(strText));
	}

	//@why.todo:应该是mainframe补充一个expand方法来层层通知重绘
	void ChildPanel::Expand(const wxPoint& ptPos, wxWindow* pWindow, bool bExpandH, bool bExpandV)
	{
		if (bExpandH || bExpandV)
		{
			wxSize					szExpand = pWindow->GetSize();
			wxRect					rcExpand(ptPos, szExpand);
			const wxWindowList&		children = GetChildren();
			wxWindow*				pChild = nullptr;
			wxRect					rcOldChild;
			wxRect					rcNewChild;

			m_graphic.Expand(rcExpand, bExpandH, bExpandV);
			m_graphic.Refresh();

			for (wxWindowList::const_iterator it = children.begin(); it != children.end(); it++)
			{
				pChild = *it;
				
				rcOldChild = pChild->GetRect();
				rcNewChild = rcOldChild;
				if (bExpandH)
				{
					if (rcNewChild.x > ptPos.x)
					{
						rcNewChild.x += szExpand.x;
					}
					else
					{
						if (rcNewChild.GetRight() >= ptPos.x)
							rcNewChild.width += szExpand.x;
					}
				}

				if (bExpandV)
				{
					if (rcNewChild.y > ptPos.y)
					{
						rcNewChild.y += szExpand.y;
					}
					else
					{
						if (rcNewChild.GetBottom() >= ptPos.y)
							rcNewChild.height += szExpand.y;
					}
				}

				if (rcNewChild != rcOldChild)
				{
					pChild->SetPosition(rcNewChild.GetPosition());
					pChild->SetSize(rcNewChild.GetSize());
				}
			}
			
			pWindow->SetPosition(ptPos);
			pWindow->Show(true);
		}
	}

	void ChildPanel::Contract(wxWindow* pWindow, bool bContractH, bool bContractV)
	{
		if (bContractH || bContractV)
		{
			wxRect					rcContract = pWindow->GetRect();
			wxPoint					ptPos = rcContract.GetBottomLeft();
			const wxWindowList&		children = GetChildren();
			wxWindow*				pChild = nullptr;
			wxRect					rcOldChild;
			wxRect					rcNewChild;

			m_graphic.Contract(rcContract, bContractH, bContractV);
			m_graphic.Refresh();

			pWindow->Show(false);

			for (wxWindowList::const_iterator it = children.begin(); it != children.end(); it++)
			{
				pChild = *it;

				rcOldChild = pChild->GetRect();
				rcNewChild = rcOldChild;
				if (bContractH)
				{
					if (rcNewChild.x > ptPos.x)
					{
						rcNewChild.x -= rcContract.width;
					}
					else
					{
						if (rcNewChild.GetRight() >= ptPos.x)
							rcNewChild.width -= rcContract.width;
					}
				}

				if (bContractV)
				{
					if (rcNewChild.y > ptPos.y)
					{
						rcNewChild.y -= rcContract.height;
					}
					else
					{
						if (rcNewChild.GetBottom() >= ptPos.y)
							rcNewChild.height -= rcContract.height;
					}
				}

				if (rcNewChild != rcOldChild)
				{
					if (pChild->IsShown())
					{
						pChild->SetPosition(rcNewChild.GetPosition());
						pChild->SetSize(rcNewChild.GetSize());
					}
				}
			}
		}
	}

	void ChildPanel::HideAndOffsetPanel(const std::string &strIncludeName, bool bIsVertical)
	{
		wxWindow* includeWindow = FindPanelItem(strIncludeName);

		if (nullptr == includeWindow)
			return;

		wxWindowList& children = GetChildren();

		for (auto window : children)
		{
			wxWindow* child = wxDynamicCast(window, wxWindow);

			if (child)
			{
				wxSize size = child->GetSize();
				wxPoint pos = child->GetPosition();
				int32_t x = pos.x;
				int32_t y = pos.y;

				wxRect rectChild(pos, size);
				wxRect rectInclude(includeWindow->GetPosition(), includeWindow->GetSize());
				if (rectInclude.Contains(rectChild))
				{
					child->Show(false);
				}
				else
				{
					if (bIsVertical)
					{
						if (y >= includeWindow->GetPosition().y + includeWindow->GetSize().y)
						{
							wxPoint movePos(child->GetPosition().x, child->GetPosition().y - includeWindow->GetSize().y);
							if (child->IsShown())
							{
								child->Move(movePos);      //区域内以下垂直向上移动
							}
						}
					}
					else
					{
						if (x >= includeWindow->GetPosition().x + includeWindow->GetSize().x)
						{
							wxPoint movePos(child->GetPosition().x - includeWindow->GetSize().x, child->GetPosition().y);
							if (child->IsShown())
							{
								child->Move(movePos);	 //区域内右水平向左移动
							}
						}
					}
				}
			}
		}
	}

	void ChildPanel::ShowIncludeAreaButton(const std::string &strIncludeName, bool bShow)
	{
		wxWindow* includeWindow = FindPanelItem(strIncludeName);

		if (nullptr == includeWindow)
			return;

		wxWindowList& children = GetChildren();

		for (auto &window : children)
		{
			wxWindow* child = wxDynamicCast(window, wxWindow);

			if (child)
			{
				wxSize size = child->GetSize();
				wxPoint pos = child->GetPosition();

				wxRect rectChild(pos, size);
				wxRect rectInclude(includeWindow->GetPosition(), includeWindow->GetSize());
				if (rectInclude.Contains(rectChild))
				{
					child->Show(bShow);
				}
			}
		}
	}

	void ChildPanel::OffsetIncludeAreaButton(const std::string &strIncludeName, wxRect offsetRect)
	{
		wxWindow* includeWindow = FindPanelItem(strIncludeName);

		if (nullptr == includeWindow)
			return;

		wxWindowList& children = GetChildren();

		for (auto &window : children)
		{
			wxWindow* child = wxDynamicCast(window, wxWindow);

			if (child)
			{
				wxSize size = child->GetSize();
				wxPoint pos = child->GetPosition();

				wxRect rectChild(pos, size);
				wxRect rectInclude(includeWindow->GetPosition(), includeWindow->GetSize());
				if (rectInclude.Contains(rectChild))
				{
					if (child->IsShown())
					{
						child->SetPosition(pos + offsetRect.GetPosition());
						child->SetSize(size + offsetRect.GetSize());
					}
				}
			}
		}
	}

	void ChildPanel::HideAndOffsetElements(const std::string &strIncludeElement, bool bIsVertical)
	{
		m_graphic.HideAndOffsetElements(strIncludeElement, bIsVertical);
	}

	void ChildPanel::ShowIncludeAreaElements(const std::string &strIncludeName, bool bShow)
	{
		m_graphic.ShowIncludeAreaElements(strIncludeName, bShow);
	}

	void ChildPanel::OffsetIncludeAreaElement(const std::string &strIncludeName, wxRect rect)
	{
		m_graphic.OffsetIncludeAreaElement(strIncludeName, rect);
	}

}
