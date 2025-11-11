#pragma once
#include <unordered_set>
#include <string>

namespace why
{
	typedef std::vector<std::vector<std::string>> DataList;
	const std::unordered_set<std::string>& ProhibitImportDomains();
}