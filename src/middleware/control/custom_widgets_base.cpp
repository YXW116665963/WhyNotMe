#include "custom_widgets_base.h"
#include "singleton.h"
#include "resource_manager.h"
#include <cmath>

namespace why
{
	CustomWidgetsBase::CustomWidgetsBase()
	{
	}

	CustomWidgetsBase::~CustomWidgetsBase()
	{

	}

	void CustomWidgetsBase::SetControlRes(const std::string& strControlRes)
	{
		IControlRes* pControlRes = Singleton<ResourceMgr>::Instance()->FindControlRes(strControlRes.c_str());

		if (m_pControlRes != pControlRes)
		{
			m_pControlRes = pControlRes;
			Refresh();
		}
	}

	bool CustomWidgetsBase::IsEmpty()
	{
		if (!m_textElements.empty())
			return false;

		if (!m_iconElements.empty())
			return false;

		if (!m_lineElements.empty())
			return false;

		if (!m_shapeElements.empty())
			return false;

		return true;
	}

	bool CustomWidgetsBase::GetPctPoints(const std::vector<wxPoint>& pts, const float64_t& pct, const float64_t& totalLength,
		std::vector<wxPoint>& pts1, std::vector<wxPoint>& pts2)
	{
		if (pts.size() <= 1 || 0 == totalLength) {
			return false;
		}
		if (pct >= 1.0f)
		{
			pts1 = pts;
			return true;
		}

		float64_t total = 0.0f;
		float64_t lengthPct = totalLength * pct;
		bool bOut = false;

		auto iter = pts.begin();
		pts1.push_back(*iter);

		wxPoint pt(*iter);

		for (++iter; iter != pts.end(); iter++)
		{
			if (true == bOut)
			{
				pts2.push_back(*iter);
				continue;
			}

			float64_t length = std::sqrt(((*iter).x - pt.x) * ((*iter).x - pt.x) + ((*iter).y - pt.y) * ((*iter).y - pt.y));
			if (total + length > lengthPct)
			{
				float64_t scale = (lengthPct - total) / length;
				float64_t x = pt.x + ((*iter).x - pt.x) * scale;
				float64_t y = pt.y + ((*iter).y - pt.y) * scale;
				pts1.push_back(wxPoint((int32_t)x, (int32_t)y));
				pts2.push_back(wxPoint((int32_t)x, (int32_t)y));
				pts2.push_back(*iter);
				bOut = true;
			}
			else if (total + length == lengthPct)
			{
				pts1.push_back(*iter);
				pts2.push_back(*iter);
				bOut = true;
			}
			else
			{
				pts1.push_back(*iter);
			}

			pt = *iter;
			total += length;
		}

		return true;
	}

	bool CustomWidgetsBase::GetPctPoints(const std::vector<wxPoint>& pts, const float64_t& pct, const float64_t& totalLength,
		std::vector<wxPoint2DDouble>& pts1, std::vector<wxPoint2DDouble>& pts2)
	{
		std::vector<wxPoint> pts1_temp, pts2_temp;

		if (false == GetPctPoints(pts, pct, totalLength, pts1_temp, pts2_temp)) {
			return false;
		}

		for (auto& iter : pts1_temp)
		{
			pts1.push_back(wxPoint2DDouble(iter.x, iter.y));
		}

		for (auto& iter : pts2_temp)
		{
			pts2.push_back(wxPoint2DDouble(iter.x, iter.y));
		}

		return true;
	}

	float64_t CustomWidgetsBase::GetTotalLength(const std::vector<wxPoint>& pts)
	{
		float64_t total = 0.0f;
		auto iter = pts.begin();
		wxPoint pt(*iter);

		for (++iter; iter != pts.end(); iter++)
		{
			float64_t length = std::sqrt(((*iter).x - pt.x) * ((*iter).x - pt.x) + ((*iter).y - pt.y) * ((*iter).y - pt.y));
			total += length;
			pt = *iter;
		}

		return total;
	}

	void CustomWidgetsBase::AddTextAndBackgoundElement(const std::string& strName, FontPtr& fontPtr, const wxColour& crText,
		const wxString& strText, const wxRect& rcText, const wxColour& background, uint32_t style, int32_t index)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_fontPtr = fontPtr;
					(*it)->m_crText = crText;
					(*it)->m_strText = strText;
					(*it)->m_rcText = rcText;
					(*it)->m_index = index;
					(*it)->m_style = style;
					(*it)->m_crBackground = background;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomTextInfoPtr		customTextPtr(new CustomTextInfo);

			customTextPtr->m_strName = strName;
			customTextPtr->m_fontPtr = fontPtr;
			customTextPtr->m_crText = crText;
			customTextPtr->m_strText = strText;
			customTextPtr->m_rcText = rcText;
			customTextPtr->m_index = index;
			customTextPtr->m_style = style;
			customTextPtr->m_crBackground = background;
			m_textElements.push_back(std::move(customTextPtr));
		}

		Refresh();
	}

	void CustomWidgetsBase::AddTextElement(const std::string& strName, FontPtr& fontPtr, const wxColour& crText,
		const wxString& strText, const wxRect& rcText, uint32_t style, int32_t index)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_fontPtr = fontPtr;
					(*it)->m_crText = crText;
					(*it)->m_strText = strText;
					(*it)->m_rcText = rcText;
					(*it)->m_index = index;
					(*it)->m_style = style;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomTextInfoPtr		customTextPtr(new CustomTextInfo);

			customTextPtr->m_strName = strName;
			customTextPtr->m_fontPtr = fontPtr;
			customTextPtr->m_crText = crText;
			customTextPtr->m_strText = strText;
			customTextPtr->m_rcText = rcText;
			customTextPtr->m_index = index;
			customTextPtr->m_style = style;
			m_textElements.push_back(std::move(customTextPtr));
		}

		Refresh();
	}

	bool CustomWidgetsBase::ChangeTextElement(const std::string& strName, const wxString& strText)
	{
		bool		bChanged = false;

		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_strText = strText;
					Refresh();
					bChanged = true;
					break;
				}
			}
		}

		return bChanged;
	}

	bool CustomWidgetsBase::ChangeTextElement(const std::string& strName, const wxRect& rc)
	{
		bool		bChanged = false;

		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_rcText = rc;
					Refresh();
					bChanged = true;
					break;
				}
			}
		}

		return bChanged;
	}

	void CustomWidgetsBase::RemoveTextElement(const std::string& strName)
	{
		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					m_textElements.erase(it);
					Refresh();
					break;
				}
			}
		}
	}

	void CustomWidgetsBase::OffsetTextElement(const std::string& strName, const wxRect& rc)
	{
		if (!strName.empty())
		{
			for (auto it = m_textElements.begin(); it != m_textElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_rcText.SetX((*it)->m_rcText.GetX() + rc.GetX());
					(*it)->m_rcText.SetY((*it)->m_rcText.GetY() + rc.GetY());
					(*it)->m_rcText.SetWidth((*it)->m_rcText.GetWidth() + rc.GetWidth());
					(*it)->m_rcText.SetHeight((*it)->m_rcText.GetHeight() + rc.GetHeight());
					Refresh();
					break;
				}
			}
		}
	}

	void CustomWidgetsBase::AddBackgroundElement(const std::string& strName, const BackgroundPtr& ptr, const wxRect& rc, const bool& show, int32_t index)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_backgroundElements.begin(); it != m_backgroundElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_backgroundPtr = ptr;
					(*it)->m_rc = rc;
					(*it)->m_index = index;
					(*it)->m_show = show;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomBackgroundInfoPtr	customPtr(new CustomBackgroundInfo);

			customPtr->m_strName = strName;
			customPtr->m_backgroundPtr = ptr;
			customPtr->m_rc = rc;
			customPtr->m_index = index;
			customPtr->m_show = show;
			m_backgroundElements.push_back(std::move(customPtr));
		}

		Refresh();
	}

	void CustomWidgetsBase::ChangeBackgroundElementRect(const std::string& strName, const wxRect& rc)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_backgroundElements.begin(); it != m_backgroundElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_rc = rc;
					bFind = true;
				}
			}
		}

		if (bFind) {
			Refresh();
		}
	}

	void CustomWidgetsBase::OffsetBackgroundElement(const std::string& strName, const wxRect& rc)
	{
		if (!strName.empty())
		{
			for (auto it = m_backgroundElements.begin(); it != m_backgroundElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_rc.SetX((*it)->m_rc.GetX() + rc.GetX());
					(*it)->m_rc.SetY((*it)->m_rc.GetY() + rc.GetY());
					(*it)->m_rc.SetWidth((*it)->m_rc.GetWidth() + rc.GetWidth());
					(*it)->m_rc.SetHeight((*it)->m_rc.GetHeight() + rc.GetHeight());
					Refresh();
					break;
				}
			}
		}
	}

	void CustomWidgetsBase::AddIconElement(const std::string& strName, PicturePtr& iconPtr, const wxRect& rcIcon, const bool& show, int32_t index)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_iconPtr = iconPtr;
					(*it)->m_rcIcon = rcIcon;
					(*it)->m_index = index;
					(*it)->m_show = show;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomIconInfoPtr		customIconPtr(new CustomIconInfo);

			customIconPtr->m_strName = strName;
			customIconPtr->m_iconPtr = iconPtr;
			customIconPtr->m_rcIcon = rcIcon;
			customIconPtr->m_index = index;
			customIconPtr->m_show = show;
			m_iconElements.push_back(std::move(customIconPtr));
		}

		Refresh();
	}

	bool CustomWidgetsBase::ChangeIconElement(const std::string& strName, PicturePtr& iconPtr)
	{
		bool		bChanged = false;

		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_iconPtr = iconPtr;
					Refresh();
					bChanged = true;
					break;
				}
			}
		}

		return bChanged;
	}

	void CustomWidgetsBase::ChangeIconElement(const std::string& strName, const wxRect& rt)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_rcIcon = rt;
					bFind = true;
				}
			}
		}

		if (bFind) {
			Refresh();
		}
	}

	void CustomWidgetsBase::RemoveAllIconElement()
	{
		m_iconElements.clear();
		Refresh();
	}

	void CustomWidgetsBase::RemoveIconElement(const std::string& strName)
	{
		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					m_iconElements.erase(it);
					Refresh();
					break;
				}
			}
		}
	}

	void CustomWidgetsBase::OffsetIconElement(const std::string& strName, const wxRect& rt)
	{
		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_rcIcon.SetX((*it)->m_rcIcon.GetX() + rt.GetX());
					(*it)->m_rcIcon.SetY((*it)->m_rcIcon.GetY() + rt.GetY());
					(*it)->m_rcIcon.SetWidth((*it)->m_rcIcon.GetWidth() + rt.GetWidth());
					(*it)->m_rcIcon.SetHeight((*it)->m_rcIcon.GetHeight() + rt.GetHeight());
					Refresh();
					break;
				}
			}
		}
	}

	void CustomWidgetsBase::ShowIconElement(const std::string& strName, const bool& show)
	{
		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_show = show;
					Refresh();
					break;
				}
			}
		}
	}

	bool CustomWidgetsBase::IsShowIconElement(const std::string& strName)
	{
		bool show = false;

		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					show = (*it)->m_show;
					break;
				}
			}
		}

		return show;
	}

	void CustomWidgetsBase::ChangeIconElementShow(const std::string& strName)
	{
		if (!strName.empty())
		{
			for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_show = !(*it)->m_show;
					Refresh();
					break;
				}
			}
		}
	}

	void CustomWidgetsBase::RotateIconElement(const std::string& strName, const double& rotate, const wxPoint& rotatePt)
	{
		if (strName.empty()) {
			return;
		}

		for (auto it = m_iconElements.begin(); it != m_iconElements.end(); it++)
		{
			if ((*it)->m_strName == strName)
			{
				if ((*it)->m_rotate != rotate || (*it)->m_rotatePoint != rotatePt)
				{
					(*it)->m_rotate = rotate;
					(*it)->m_rotatePoint = rotatePt;
					Refresh();
				}
				break;
			}
		}
	}

	void CustomWidgetsBase::AddLineElement(const std::string& strName, PenPtr& penPtr, const wxPoint& start, const wxPoint& end, int32_t index)
	{
		std::vector<wxPoint> vector;
		vector.push_back(start);
		vector.push_back(end);

		AddLineElement(strName, penPtr, vector, index);
	}

	void CustomWidgetsBase::AddSubLineElement(const std::string& strName, const std::string& strSubName, PenPtr& penPtr,
		const wxPoint& start, const wxPoint& end, int32_t index)
	{
		CustomLineSub info;
		info.m_strSubName = strSubName;
		info.m_penPtr = penPtr;
		info.m_index = index;
		info.m_vector.push_back(start);
		info.m_vector.push_back(end);

		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_lineElements.begin(); it != m_lineElements.end(); it++)
			{
				if ((*it)->m_strName != strName) continue;

				bool bFindSub = false;

				if (!strSubName.empty())
				{
					for (auto& iter : (*it)->m_subList)
					{
						if (iter.m_strSubName != strSubName) continue;

						iter = info;
						bFindSub = true;
					}
				}

				if (false == bFindSub) {
					(*it)->m_subList.push_back(std::move(info));
				}

				bFind = true;
				break;
			}
		}

		if (!bFind)
		{
			CustomLineInfoPtr		customLinePtr(new CustomLineInfo);

			customLinePtr->m_strName = strName;
			customLinePtr->m_penPtr = penPtr;
			customLinePtr->m_vector = info.m_vector;
			customLinePtr->m_subList.push_back(std::move(info));
			customLinePtr->m_index = 0;
			m_lineElements.push_back(std::move(customLinePtr));
		}

		Refresh();
	}

	void CustomWidgetsBase::AddSubLineElement(const std::string& strName, PenPtr& penPtr, const wxPoint& start, const wxPoint& end)
	{
		AddSubLineElement(strName, "", penPtr, start, end);
	}

	void CustomWidgetsBase::AddLineElement(const std::string& strName, PenPtr& penPtr, const std::vector<wxPoint>& vector, int32_t index)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_lineElements.begin(); it != m_lineElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_penPtr = penPtr;
					(*it)->m_vector = vector;
					(*it)->m_index = index;
					bFind = true;
					break;
				}
			}
		}

		if (!bFind)
		{
			CustomLineInfoPtr		customLinePtr(new CustomLineInfo);

			customLinePtr->m_strName = strName;
			customLinePtr->m_penPtr = penPtr;
			customLinePtr->m_vector = vector;
			customLinePtr->m_index = index;
			m_lineElements.push_back(std::move(customLinePtr));
		}

		Refresh();
	}

	bool CustomWidgetsBase::ChangeLineElement(const std::string& strName, PenPtr& penPtr, const wxPoint& start, const wxPoint& end)
	{
		std::vector<wxPoint> vector;
		vector.push_back(start);
		vector.push_back(end);

		return ChangeLineElement(strName, penPtr, vector);
	}

	bool CustomWidgetsBase::ChangeLineElement(const std::string& strName, PenPtr& penPtr, const std::vector<wxPoint>& vector)
	{
		bool		bChanged = false;

		if (!strName.empty())
		{
			for (auto it = m_lineElements.begin(); it != m_lineElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_penPtr = penPtr;
					(*it)->m_vector = vector;
					Refresh();
					bChanged = true;
					break;
				}
			}
		}

		return bChanged;
	}

	bool CustomWidgetsBase::ChangeLineElement(const std::string& strName, PenPtr& penPtr)
	{
		bool		bChanged = false;

		if (!strName.empty())
		{
			for (auto it = m_lineElements.begin(); it != m_lineElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_penPtr = penPtr;
					Refresh();
					bChanged = true;
					break;
				}
			}
		}

		return bChanged;
	}

	void CustomWidgetsBase::RemoveLineElement(const std::string& strName)
	{
		if (!strName.empty())
		{
			for (auto it = m_lineElements.begin(); it != m_lineElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					m_lineElements.erase(it);
					Refresh();
					break;
				}
			}
		}
	}

	void CustomWidgetsBase::RemoveAllLineElement()
	{
		m_lineElements.clear();
	}

	void CustomWidgetsBase::AddLineArcElement(const std::string& strName, PenPtr& penPtr, const wxPoint& pt, const wxSize& size,
		const std::pair<float64_t, float64_t>& startEnd, int32_t index)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_lineArcElements.begin(); it != m_lineArcElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_penPtr = penPtr;
					(*it)->m_index = index;
					(*it)->m_leftTop = pt;
					(*it)->m_size = size;
					(*it)->m_startEnd = startEnd;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomLineArcInfoPtr customLinePtr(new CustomLineArcInfo);

			customLinePtr->m_strName = strName;
			customLinePtr->m_penPtr = penPtr;
			customLinePtr->m_index = index;
			customLinePtr->m_leftTop = pt;
			customLinePtr->m_size = size;
			customLinePtr->m_startEnd = startEnd;

			m_lineArcElements.push_back(std::move(customLinePtr));
		}

		Refresh();
	}

	void CustomWidgetsBase::AddSubLineArcElement(const std::string& strName, PenPtr& penPtr, const wxPoint& pt, const wxSize& size,
		const std::pair<float64_t, float64_t>& startEnd)
	{
		AddSubLineArcElement(strName, "", penPtr, pt, size, startEnd);
	}

	void CustomWidgetsBase::AddSubLineArcElement(const std::string& strName, const std::string& strSubName, PenPtr& penPtr, const wxPoint& pt,
		const wxSize& size, const std::pair<float64_t, float64_t>& startEnd, int32_t index)
	{
		CustomLineArcSub info;
		info.m_strSubName = strSubName;
		info.m_penPtr = penPtr;
		info.m_index = index;
		info.m_leftTop = pt;
		info.m_size = size;
		info.m_startEnd = startEnd;

		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_lineArcElements.begin(); it != m_lineArcElements.end(); it++)
			{
				if ((*it)->m_strName != strName) continue;

				bool bFindSub = false;

				if (!strSubName.empty())
				{
					for (auto& iter : (*it)->m_subList)
					{
						if (iter.m_strSubName != strSubName) continue;

						iter = info;
						bFindSub = true;
					}
				}

				if (false == bFindSub) {
					(*it)->m_subList.push_back(std::move(info));
				}

				bFind = true;
			}
		}

		if (!bFind)
		{
			CustomLineArcInfoPtr		customLinePtr(new CustomLineArcInfo);

			customLinePtr->m_strName = strName;
			customLinePtr->m_penPtr = penPtr;
			customLinePtr->m_index = 0;
			customLinePtr->m_leftTop = pt;
			customLinePtr->m_size = size;
			customLinePtr->m_startEnd = startEnd;
			customLinePtr->m_subList.push_back(std::move(info));

			m_lineArcElements.push_back(std::move(customLinePtr));
		}

		Refresh();
	}

	void CustomWidgetsBase::RemoveAllLineArcElement()
	{
		m_lineArcElements.clear();
	}

	void CustomWidgetsBase::RemoveAllLineArcElement(const std::string& strName)
	{
		for (auto it = m_lineArcElements.begin(); it != m_lineArcElements.end(); it++)
		{
			if ((*it)->m_strName == strName)
			{
				m_lineArcElements.erase(it);
				break;
			}
		}
	}

	bool CustomWidgetsBase::AddShapeElement(const std::string& strName, const Shape& shape, const wxColour& colour,
		const wxPoint& pt, const wxSize& size, const bool& needBrush, int32_t index)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_shapeElements.begin(); it != m_shapeElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_shape = shape;
					(*it)->m_pt = pt;
					(*it)->m_size = size;
					(*it)->m_brush = wxBrush(colour, wxBRUSHSTYLE_SOLID);
					(*it)->m_hasBrush = needBrush;
					(*it)->m_colour = colour;
					(*it)->m_index = index;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomShapeInfoPtr		customShapePtr(new CustomShapeInfo);

			customShapePtr->m_strName = strName;
			customShapePtr->m_shape = shape;
			customShapePtr->m_pt = pt;
			customShapePtr->m_size = size;
			customShapePtr->m_brush = wxBrush(colour, wxBRUSHSTYLE_SOLID);
			customShapePtr->m_hasBrush = needBrush;
			customShapePtr->m_colour = colour;
			customShapePtr->m_index = index;

			m_shapeElements.push_back(std::move(customShapePtr));
		}

		Refresh();

		return true;
	}

	bool CustomWidgetsBase::AddShapeElement(const std::string& strName, const Shape& shape, const wxColour& colour, const wxColour& borderColour,
		const wxPoint& pt, const wxSize& size, int32_t index)
	{
		bool			bFind = false;

		if (!strName.empty())
		{
			for (auto it = m_shapeElements.begin(); it != m_shapeElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_shape = shape;
					(*it)->m_pt = pt;
					(*it)->m_size = size;
					(*it)->m_brush = wxBrush(colour, wxBRUSHSTYLE_SOLID);
					(*it)->m_hasBrush = true;
					(*it)->m_colour = borderColour;
					(*it)->m_index = index;
					bFind = true;
				}
			}
		}

		if (!bFind)
		{
			CustomShapeInfoPtr		customShapePtr(new CustomShapeInfo);

			customShapePtr->m_strName = strName;
			customShapePtr->m_shape = shape;
			customShapePtr->m_pt = pt;
			customShapePtr->m_size = size;
			customShapePtr->m_brush = wxBrush(colour, wxBRUSHSTYLE_SOLID);
			customShapePtr->m_hasBrush = true;
			customShapePtr->m_colour = borderColour;
			customShapePtr->m_index = index;

			m_shapeElements.push_back(std::move(customShapePtr));
		}

		Refresh();

		return true;
	}

	bool CustomWidgetsBase::ChangeShapeElement(const std::string& strName, const wxPoint& pt, const wxSize& size)
	{
		bool		bChanged = false;

		if (!strName.empty())
		{
			for (auto it = m_shapeElements.begin(); it != m_shapeElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					(*it)->m_pt = pt;
					(*it)->m_size = size;

					Refresh();

					bChanged = true;
					break;
				}
			}
		}

		return bChanged;
	}

	void CustomWidgetsBase::RemoveShapeElement(const std::string& strName)
	{
		if (!strName.empty())
		{
			for (auto it = m_shapeElements.begin(); it != m_shapeElements.end(); it++)
			{
				if ((*it)->m_strName == strName)
				{
					m_shapeElements.erase(it);
					Refresh();
					break;
				}
			}
		}
	}

	void CustomWidgetsBase::Clear()
	{
		m_textElements.clear();
		m_iconElements.clear();
		m_lineElements.clear();
		m_shapeElements.clear();
		m_backgroundElements.clear();
	}


}