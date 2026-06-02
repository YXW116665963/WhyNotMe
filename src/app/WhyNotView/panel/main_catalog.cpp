#include "main_catalog.h"
#include "main_frame.h"
#include "singleton.h"
namespace why
{
	MainCatalog::MainCatalog(wxWindow* pParent)
		: ChildPanel("MainCatalog.xml", pParent, wxPoint(0,0), wxSize(0,0))
	{

	}

	MainCatalog::~MainCatalog()
	{

	}


}