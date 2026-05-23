#pragma once

#include <string>
#include <functional>
#include <memory>
#include "timer.h"
#include "data_center_def.h"
#include "data_center_interface.h"


typedef std::unique_ptr<why::Timer> TimerPtr;
typedef std::function<void(why::IDataCenter*)> DataCenterDeleter;
typedef std::unique_ptr<why::IDataCenter, DataCenterDeleter> DataCenterPtr;
struct GlobalPointer
{
	TimerPtr					m_timePtr;
	DataCenterPtr				m_dataCenterPtr{ nullptr, [](why::IDataCenter* pDataCenter) { pDataCenter->Destroy(); } };

	//FullKeyboardPtr			m_fullKeyboardPtr;
	//NumberKeyboardPtr			m_numberKeyboardPtr;
	//RunningLogPtr				m_runningLogPtr;
	//MonitorCenterPtr			m_monitorCenterPtr;
};


// 核心：跨 DLL 导出宏
#ifdef GLOBAL_POINTER_EXPORT
#define GLOBAL_POINTER __declspec(dllexport)
#else
#define GLOBAL_POINTER __declspec(dllimport)
#endif

// 重点：必须加 GLOBAL_API，才能共享同一个变量！
extern GLOBAL_POINTER GlobalPointer g_globalPointer;


