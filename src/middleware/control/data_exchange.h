#pragma once

#include "standard.h"
#include "gui_import.h"
#include "data_center_interface.h"
#include "dynamic_data_handler.h"
#include <memory>
#include <list>
#include <functional>
#include <utility>

namespace why
{
	namespace DataThreshold
	{
		extern const uint32_t g_uNone;			//!< 阈值不起作用
		extern const uint32_t g_uMin;			//!< 阈值最小值起作用
		extern const uint32_t g_uMax;			//!< 阈值最大值起作用
	};

	struct BaseBindDataInfo
	{
		wxWindow*			m_pWindow{nullptr};							// 绑定数据得窗口
		std::string			m_strDomain{""};							// 和窗口绑定得命名数据得分类名称，不能为空
		std::string			m_strName{""};								// 和窗口绑定得命名数据得名称，如果为空，这将分类名称更新到控件上
		std::string			m_strChildName{""};							// 数据得子元素名称(非必须，主要针对object类型得命名数据，用于绑定某个子元素)
		std::string			m_strFormat{""};							// 数据在窗口上展示格式，可以为空，此时使用默认格式展示
		std::string			m_strDisplay{""};							// 数据的名称，如果为空，会以strDomain|strName|strChildName的组合来替代
		uint32_t			m_uThresholdMask{ DataThreshold::g_uNone }; // 数据阈值掩码
		std::string			m_strMin;									// 数据阈值--最小值
		std::string			m_strMax;									// 数据阈值--最大值
	};

	wxDECLARE_EVENT(DATA_CHECKING_EVENT, wxCommandEvent);

	/**
	* @brief 数据绑定器
	* @details 
	* - 绑定窗口控件和命名数据，当命名数据的值发生改变的时候，能自动刷新到界面
	* - 自动处理当前控件的值和命名数据相比是否发生了变化，如果发生变化，制动修改控件的背景，以示区分
	* - 更新控件当前数据到数据中心
	* - 使用数据中心的值恢复当前的界面修改
	*/
	class DataExchange : public IDataObserver, public wxEvtHandler
	{
	public:
		DataExchange(IDataCenter *pDataCenter);
		~DataExchange();

	public:
		typedef std::function<void(wxWindow*, INamedData*)> DisplayMethon;

		struct ChangedData
		{
			std::string		m_strDisplay;		// 数据的展示名称
			std::string     m_strDomain;        // dc字段名
			std::string		m_strOldData;		// 修改前的旧值
			std::string		m_strNewData;		// 修改后的新值
		};

	public:
		/**
		* @brief 绑定数据
		* @param baseDataInfo 数据信息的基本结构体
		* @return void
		*/
		void Bind(const BaseBindDataInfo& baseDataInfo);

		/**
		* @brief 绑定数据
		* @param baseDataInfo 数据信息的基本结构体(此处的name为格式)
		* @param arraySubscribeInfo 需要关注的字段，字段变化重新生成新的name
		* @return void
		*/
		void Bind(BaseBindDataInfo& baseDataInfo, const std::vector<DataNameInfo>& arraySubscribeInfo);

		/**
		* @brief 解绑数据
		* @param pWnindow 待解绑数据得窗口
		* @return void
		* @warning 一般情况下不需要调用本方法，除非在一个父窗口中动态创建得子窗口在父窗口销毁之前被销毁
		*/
		void Unbind(wxWindow* pWindow);

		/**
		* @brief 更改数据绑定
		* @param 
		* @return void
		* @warning
		*/
		void ChangeBind(
			const std::string& strOldDoMain, const std::string& strOldName,
			const std::string& strNewDoMain, const std::string& strNewName);

		/**
		 * @brief 解绑所有数据
		 * @return void
		 * @warning 一般情况下不需要调用本方法，除非在一个父窗口中动态创建得子窗口在父窗口销毁之前被销毁
		 */
		void UnbindAll();
	
		/**
		* @brief 设置定制的数据展示方式
		* @param pWnindow 需要定制数据展示方式的窗口
		* @param displayMethon 定制的数据展示方式
		* @return void
		* @warning 
		* - 定制化的展示方式不是必须的，但一个窗口同时最多只能有一个定制化的展示方法。
		* - 一当提供了定制化的数据展示方式，就必须保证提供的函数对象在Unbind之前是可用的。
		*/
		void SetCustomDisplayMethod(wxWindow* pWindow, const DisplayMethon &displayMethon);

		/**
		* @brief 订阅数据得更新
		* @return void
		* @warning 
		* - 该函数可以被多次调用，针对同一个命名数据，有且仅有第一次调用有效；该方法至少被调用一次，否则窗口无数据展示
		* - 不在数据绑定得时候自动订阅得原因是：如果命名数据是object类型得，多个控件分别绑定在不同得子元素上，由于订阅只有一次，会导致后续绑定得窗口无数据展示
		*/
		void Subscribe();

		/**
		* @brief 取消对数据更新得订阅
		* @return void
		* @warning 一般在父窗口被关闭得地方调用本方法，减少性能得消耗
		*/
		void Unsubscribe();

		/**
		* @brief 将UI上展示得数据保存到数据中心
		* @return void 
		*/
		void Apply();

		/**
		* @brief 放弃当前UI上得数据得修改，使用数据中心得当前值复位
		* @return void
		*/
		void Reset();

		/**
		* @brief 获取当前所有有改变的数据
		* @return void
		*/
		std::vector<ChangedData> GetChangedDatas();

	private:
		void OnDataChanged(INamedData* pData);
		
	private:
		struct BindWnd
		{
			wxWindow*			m_pWindow{nullptr};
			std::string			m_strChildName{""};
			std::string			m_strFormat;
			std::string			m_strDisplay;
			ControlType			m_eControlType{ ControlType::e_empty_ctrl };
			DisplayMethon		m_displayMethon{ nullptr };
			wxColor				m_crNormalBK;
			std::string			m_strBaseValue;
			std::string			m_strCurrentValue;
			bool				m_bChanged{ false };
			uint32_t			m_uThresholdMask{ DataThreshold::g_uNone }; 
			std::string			m_strMin;
			std::string			m_strMax;
			bool				m_bChecked{ true };
		};
		typedef std::list<BindWnd> BindWndList;
		typedef BindWndList::iterator BindWndListIt;

		struct BindData
		{
			std::string			m_strDomain;
			std::string			m_strName;
			BindWndList			m_list;
			bool				m_bSubscribe{false};
		};
		typedef std::list<BindData> BindDataList;
		typedef BindDataList::iterator BindDataListIt;
		
	private:
		void Subscribe(const std::string& strDomain, const std::string& strName);
		void Unsubscribe(const std::string& strDomain, const std::string& strName);
		void HookWndEvent(BindWndList &bindWndList);
		BindDataListIt FindBindData(const std::string& strDomain, const std::string& strName);
		BindWnd* FindBindWnd(wxWindow* pWnd);
		ControlType QueryControlType(wxWindow* pWnd);
		void UpdateData(BindWnd& bindWnd, INamedData* pData);
		bool GetNamedDataText(INamedData* pNamedData, const std::string &strChildName, const std::string &strFormat, std::string &strText);
		bool SetNamedDataText(INamedData* pNamedData, const std::string& strChildName, const std::string& strText);
		bool GetDataValueText(IDataValue* pDataValue, const std::string& strFormat, std::string& strText);
		bool SetDataValueText(IDataValue* pDataValue, const std::string& strText);		
		void OnTextChanged(wxCommandEvent& event);
		void OnSelectedChanged(wxCommandEvent& event);
		void OnSwitchChanged(wxCommandEvent& event);
		void OnDataChecking(wxCommandEvent& event);
		bool GetWindowText(ControlType eType, wxWindow* pWnd, std::string& strText);
		bool SetWindowText(ControlType eType, wxWindow* pWnd, const std::string& strText);

	private:
		IDataCenter*			m_pDataCenter;
		BindDataList			m_bindList;
		wxColor					m_crChangedBK;
		std::unique_ptr<DynamicDataHandler> m_pDynamicDataHandler;
		std::unique_ptr<wxTimer>	m_timerPtr;
	};
}
