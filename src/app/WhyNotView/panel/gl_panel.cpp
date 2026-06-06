#include "gl_panel.h"
#include "logger.h"
#include "gl_canvas.h"
namespace why
{
	GLPanel::GLPanel(wxWindow* pParent)
		: ChildPanel("GLPanel.xml", pParent, wxPoint(0, 0), wxSize(0, 0))
	{
		wxRect rectRende;
		if (GetRenderRect("gl_rect", rectRende))
		{
			new GLCanvas(this, rectRende);
		}
	}


	GLPanel::~GLPanel()
	{

	}
}
