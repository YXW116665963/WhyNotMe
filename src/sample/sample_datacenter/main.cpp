#include "data_observer_helper.h"

#include "test_dataloader.h"

int main()
{
	test_::test_DataLoader dataLoader;
	dataLoader.InitDataCenter();
	why::CallbackTest callbackTest(m_dataCenterPtr.get());


	std::vector<std::string> vec = { "people", "why" ,"string" ,"0","true" };
	dataLoader.LoadOneData(vec);

	why::SetStringValue("people", "why", "19");
}