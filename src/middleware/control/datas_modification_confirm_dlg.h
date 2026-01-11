#pragma once

#include "wx/wx.h"
#include "wx/dialog.h"

#include "grid_container.h"
#include "data_exchange.h"

namespace why
{
	class DatasModificationConfirmDlg : public wxDialog
	{
	public:
		typedef std::vector<DataExchange::ChangedData> ChangedDataVector;

	public:
		DatasModificationConfirmDlg(const ChangedDataVector &datas, wxWindow* parent, wxWindowID id = wxID_ANY, long style = wxDEFAULT_DIALOG_STYLE);
		virtual ~DatasModificationConfirmDlg();

	private:
		void OnClickOKButton(wxCommandEvent& event);
		void OnClickChannelButton(wxCommandEvent& event);

	private:
		GridContainerPtr				m_gridContrainerPtr;
		uint32_t						m_uNextID{ 1000 };
	};
}
