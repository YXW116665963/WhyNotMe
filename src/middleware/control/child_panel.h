#pragma once

#include "gui_import.h"//
#include "resource_manager.h"
#include "panel_graphic.h"
#include "data_exchange.h"


#include <memory>

// 绑定指定控件的响应函数（给定指针，关键字，控件类型，函数）
#define BIND_FUNC_BY_WND(ptr, key, control, func)\
ptr = dynamic_cast<control *>(FindPanelItem(key));\
if (nullptr != ptr){ptr->Bind(wxEVT_BUTTON, &func, this);}

// 根据关键字绑定控件的响应函数（关键字，函数）
#define BIND_FUNC_BY_KEY(key, func)\
{wxWindow* pWnd = FindPanelItem(key); \
if (pWnd) { pWnd->Bind(wxEVT_BUTTON, &func, this); }}

// 检查数据并获取控件指针（控件类型）
#define GET_CONTROL(controltype) \
if (!pWxWnd || !pINamedData) { return; }\
controltype* pControl = dynamic_cast<controltype*>(pWxWnd); \
if (!pControl){ return; } 

// 设置控件窗口的显示函数（关键字，控件类型）
#define SET_WND_SHOWMODE(key, func)\
	{\
		wxWindow* pWindow = FindPanelItem(key);\
		SetWndDisplayMode(pWindow, std::bind(&func, this, pWindow, std::placeholders::_2));\
	}

namespace why
{
	class ChildPanel : public wxPanel
    {
    public:
        ChildPanel(const std::string &strResFile, wxWindow* parent, const wxPoint& pos, const wxSize& size, bool bBindData=true);
        virtual ~ChildPanel();

    public:
		// 使能/禁用面板内所有控件
		void EnablePanel(bool bEnable);
		// 只能禁用已经被使能的面板内所有控件
		void EnablePanelEx(bool bEnable);

	public://get
        wxWindow* FindPanelItem(const std::string &strItemName);
		DataExchange* GetDataExchange();
		PanelGraphic& GetPanelGraphic();
	public:
		void RefreshWindow();
		void SetWndDisplayMode(wxWindow* pWindow, DataExchange::DisplayMethon func);
		void ShowElements(const std::vector<std::string>& elements, bool bShow);
		void ShowButton(const std::vector<std::string>& buttons, bool bShow);
		bool GetDlgItemText(const std::string &strName, std::string &strText);
		void SetDlgItemText(const std::string &strName, const std::string &strText);

		template <typename EventTag, typename Functor>
		void BindEvent(const std::string &strName, const EventTag& eventType, const Functor &functor)
		{
			wxWindow *pChild = FindPanelItem(strName);

			if (pChild)
				pChild->Bind(eventType, functor);
		}

		virtual void ReadOnlyEnableButton() {} //仅显示模式对按钮的使能
		virtual void InitializePanel() {}
		virtual bool Apply()
		{
			GetDataExchange()->Apply();
			return true;
		}

		void Expand(const wxPoint& ptPos, wxWindow* pWindow, bool bExpandH, bool bExpandV);
		void Contract(wxWindow* pWindow, bool bContractH, bool bContractV);

		void HideAndOffsetPanel(const std::string &strIncludeName, bool bIsVertical);
		void HideAndOffsetElements(const std::string &strIncludeElement, bool bIsVertical);
		void ShowIncludeAreaButton(const std::string &strIncludeName, bool bShow);
		void OffsetIncludeAreaButton(const std::string &strIncludeName, wxRect rect);
		void ShowIncludeAreaElements(const std::string &strIncludeName, bool bShow);
		void OffsetIncludeAreaElement(const std::string &strIncludeName, wxRect rect);

    private:
        void OnCloseWindow(wxCloseEvent& event);
		void OnEraseBackground(wxEraseEvent& event);
        void OnDrawPanelBackground(wxPaintEvent& event);

    private:
        typedef std::unique_ptr<DataExchange> DataExchangePtr;
		typedef std::vector<wxWindow *> VisibleWndVector;

	private:
		void CreateBufferBitmap();

	private:
        DataExchangePtr				m_dataExchangePtr;



        PanelGraphic				m_graphic;
		wxBitmap					m_bufferBitmap;
		wxSize						m_szBitmap;
    };
}