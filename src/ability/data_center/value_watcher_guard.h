#pragma once

#include "standard.h"
#include "value_watcher_interface.h"

namespace why
{
	class ValueWatcherGuard
	{
	public:
		ValueWatcherGuard(IValueWatcher* pValueWatcher)
			: m_pWatcher(pValueWatcher)
		{
			assert(m_pWatcher);
			m_pWatcher->Pause();
		}

		~ValueWatcherGuard()
		{
			m_pWatcher->Resume();
		}

	private:
		IValueWatcher* m_pWatcher;
	};
}