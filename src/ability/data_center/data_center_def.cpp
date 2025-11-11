#include "data_center_def.h"
#include <unordered_set>
#include "data_center_type.h"

namespace why
{


	const std::unordered_set<std::string>& ProhibitImportDomains() {
		static std::unordered_set<std::string> g_data =
		{

		};
		return g_data;
	}

}