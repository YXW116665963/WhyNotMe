#include "datas_modification_confirm_dlg.h"

#include "wx/event.h"
#include "wx/msgdlg.h"

#include "text_util.h"
#include "file_util.h"

namespace why
{
	namespace
	{
		const std::vector<std::pair<why::ControlType, wxString>> g_columnHeaderInfos =
		{
			{why::ControlType::e_static_ctrl, wxT("名称")},
			{why::ControlType::e_static_ctrl, wxT("旧值")},
			{why::ControlType::e_static_ctrl, wxT("新值")},
		};
	};

	DatasModificationConfirmDlg::DatasModificationConfirmDlg(const ChangedDataVector& datas, wxWindow* parent, wxWindowID id, long style)
		: wxDialog(parent, id, wxT("修改确认"), wxDefaultPosition, wxDefaultSize, style)
	{
		wxPoint			ptPos;
		wxSize			szSize;
		wxRect			rcClient = GetClientRect();
		wxButton*		pButton = nullptr;
		GridLayOutCfg   tGridCfg;
		uint32_t		uLine = (uint32_t)datas.size();

		ptPos.x = 4;
		ptPos.y = 4;
		pButton = new wxButton(this, wxID_ANY, wxT("确认"), ptPos, wxDefaultSize, 0);
		auto szButtonSize = pButton->GetSize();
		pButton->Connect(wxEVT_BUTTON, wxCommandEventHandler(DatasModificationConfirmDlg::OnClickOKButton), nullptr, this);
		ptPos.x = rcClient.x + rcClient.width - 4 - szButtonSize.x;
		ptPos.y = rcClient.y + rcClient.height - 4 - szButtonSize.y;
		pButton->SetPosition(ptPos);

		ptPos.x -= (szButtonSize.GetWidth() + 4);
		pButton = new wxButton(this, wxID_ANY, wxT("取消"), ptPos, wxDefaultSize, 0);
		szButtonSize = pButton->GetSize();
		pButton->Connect(wxEVT_BUTTON, wxCommandEventHandler(DatasModificationConfirmDlg::OnClickChannelButton), nullptr, this);

		rcClient.height -= (szButtonSize.y + 8);
		tGridCfg.m_uColumn = 3;
		tGridCfg.m_uLine = uLine;
		m_gridContrainerPtr.reset(new why::GridContainer(this, wxEmptyString, rcClient, tGridCfg));

		for (uint32_t i = 0; i < 3; i++)
			m_gridContrainerPtr->SetCell(i, 0, 1, 1, g_columnHeaderInfos[i].first, g_columnHeaderInfos[i].second, wxEmptyString);

		for (uint32_t j = 0; j < uLine; j++)
		{
			m_gridContrainerPtr->SetCell(0, 1 + j, 1, 1, ControlType::e_static_ctrl, wxString::FromUTF8(datas[j].m_strDisplay.c_str()), wxEmptyString);
			m_gridContrainerPtr->SetCell(1, 1 + j, 1, 1, ControlType::e_static_ctrl, wxString::FromUTF8(datas[j].m_strOldData.c_str()), wxEmptyString);
			m_gridContrainerPtr->SetCell(2, 1 + j, 1, 1, ControlType::e_static_ctrl, wxString::FromUTF8(datas[j].m_strNewData.c_str()), wxEmptyString);
		}
	}

	DatasModificationConfirmDlg::~DatasModificationConfirmDlg()
	{
	
	}

	void DatasModificationConfirmDlg::OnClickOKButton(wxCommandEvent& event)
	{
		EndModal(wxID_OK);
	}

	void DatasModificationConfirmDlg::OnClickChannelButton(wxCommandEvent& event)
	{
		EndModal(wxID_CANCEL);
	}
}