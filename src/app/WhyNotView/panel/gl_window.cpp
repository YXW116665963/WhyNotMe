#include "gl_window.h"


namespace why
{
	GLWindow::GLWindow(wxWindow* pParent)
		: ChildPanel("GLWindow.xml", pParent, wxPoint(0, 0), wxSize(0, 0))
	{

	}

	GLWindow::~GLWindow()
	{

	}

}
