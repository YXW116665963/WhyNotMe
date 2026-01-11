#pragma once

#include "picture.h"
#include "control_res.h"
#include "background.h"

#include <memory>
#include <map>

namespace why
{
	class CustomWidgetsBase
	{
	public:
		CustomWidgetsBase();
		~CustomWidgetsBase();

		virtual void Refresh() = 0;

		enum Shape
		{
			circle = 1,
			square = 2,
		};

	public:
		void SetControlRes(const std::string& strControlRes);
		bool IsEmpty();

		void AddTextAndBackgoundElement(const std::string& strName, FontPtr& fontPtr, const wxColour& crText,
			const wxString& strText, const wxRect& rcText, const wxColour& background, uint32_t style = wxTE_CENTER, int32_t index = 0);
		void AddTextElement(const std::string& strName, FontPtr& fontPtr, const wxColour& crText,
			const wxString& strText, const wxRect& rcText, uint32_t style = wxTE_CENTER, int32_t index = 0);
		bool ChangeTextElement(const std::string& strName, const wxString& strText);
		bool ChangeTextElement(const std::string& strName, const wxRect& rc);
		void RemoveTextElement(const std::string& strName);
		void OffsetTextElement(const std::string& strName, const wxRect& rc);

		void AddBackgroundElement(const std::string& strName, const BackgroundPtr& ptr, const wxRect& rc, const bool& show = true, int32_t index = 0);
		void ChangeBackgroundElementRect(const std::string& strName, const wxRect& rc);
		void OffsetBackgroundElement(const std::string& strName, const wxRect& rc);

		void AddIconElement(const std::string& strName, PicturePtr& iconPtr, const wxRect& rcIcon, const bool& show = true, int32_t index = 0);
		bool ChangeIconElement(const std::string& strName, PicturePtr& iconPtr);
		void RemoveIconElement(const std::string& strName);
		void OffsetIconElement(const std::string& strName, const wxRect& rt);
		void RotateIconElement(const std::string& strName, const double& rotate, const wxPoint& rotatePt);
		void ShowIconElement(const std::string& strName, const bool& show);
		bool IsShowIconElement(const std::string& strName);
		void ChangeIconElementShow(const std::string& strName);
		void ChangeIconElement(const std::string& strName, const wxRect& rt);
		void RemoveAllIconElement();

		void AddLineElement(const std::string& strName, PenPtr& penPtr, const wxPoint& start, const wxPoint& end, int32_t index = 0);
		void AddLineElement(const std::string& strName, PenPtr& penPtr, const std::vector<wxPoint>& vector, int32_t index = 0);
		void AddSubLineElement(const std::string& strName, PenPtr& penPtr, const wxPoint& start, const wxPoint& end);
		void AddSubLineElement(const std::string& strName, const std::string& strSubName, PenPtr& penPtr, const wxPoint& start, const wxPoint& end, int32_t index = 0);
		bool ChangeLineElement(const std::string& strName, PenPtr& penPtr, const wxPoint& start, const wxPoint& end);
		bool ChangeLineElement(const std::string& strName, PenPtr& penPtr, const std::vector<wxPoint>& vector);
		bool ChangeLineElement(const std::string& strName, PenPtr& penPtr);
		void RemoveLineElement(const std::string& strName);
		void RemoveAllLineElement();

		void AddLineArcElement(const std::string& strName, PenPtr& penPtr, const wxPoint& pt, const wxSize& size,
			const std::pair<float64_t, float64_t>& startEnd, int32_t index = 0);
		void AddSubLineArcElement(const std::string& strName, PenPtr& penPtr, const wxPoint& pt, const wxSize& size,
			const std::pair<float64_t, float64_t>& startEnd);
		void AddSubLineArcElement(const std::string& strName, const std::string& strSubName, PenPtr& penPtr, const wxPoint& pt, const wxSize& size,
			const std::pair<float64_t, float64_t>& startEnd, int32_t index = 0);
		void RemoveAllLineArcElement(const std::string& strName);
		void RemoveAllLineArcElement();

		bool AddShapeElement(const std::string& strName, const Shape& shape, const wxColour& colour,
			const wxPoint& pt, const wxSize& size, const bool& needBrush = true, int32_t index = 0);
		bool AddShapeElement(const std::string& strName, const Shape& shape, const wxColour& colour, const wxColour& lineColour,
			const wxPoint& pt, const wxSize& size, int32_t index = 0);
		bool ChangeShapeElement(const std::string& strName, const wxPoint& pt, const wxSize& size);
		void RemoveShapeElement(const std::string& strName);
		void Clear();

	protected:
		bool GetPctPoints(const std::vector<wxPoint>& pts, const float64_t& pct, const float64_t& totalLength,
			std::vector<wxPoint>& pts1, std::vector<wxPoint>& pts2);

		bool GetPctPoints(const std::vector<wxPoint>& pts, const float64_t& pct, const float64_t& totalLength,
			std::vector<wxPoint2DDouble>& pts1, std::vector<wxPoint2DDouble>& pts2);

		float64_t GetTotalLength(const std::vector<wxPoint>& pts);

	protected:
		enum InfoType : uint32_t
		{
			eNone = 0,
			eBackground = 1,
			eIcon = 2,
			eShape = 3,
			eLine = 4,
			eLineArc = 5,
			eText = 6,
		};

		struct BaseInfo
		{
			int32_t         m_default_z{ 0 };
			int32_t         m_index{ 0 };
			InfoType        m_type{ eNone };

			BaseInfo(InfoType type) : m_type(type) {};
		};

		struct CustomTextInfo : public BaseInfo
		{
			std::string		m_strName;
			FontPtr			m_fontPtr;
			wxColour		m_crText;
			wxString		m_strText;
			wxRect			m_rcText;
			uint32_t        m_style{ 0 };
			wxColour        m_crBackground;

			CustomTextInfo() : BaseInfo(eText) {};
		};
		typedef std::unique_ptr<CustomTextInfo> CustomTextInfoPtr;
		typedef std::list<CustomTextInfoPtr> CustomTextList;

		struct CustomIconInfo : public BaseInfo
		{
			std::string		m_strName;
			PicturePtr		m_iconPtr;
			wxRect			m_rcIcon;
			double          m_rotate{ 0.0 };
			wxPoint         m_rotatePoint;
			bool            m_show{ true };

			CustomIconInfo() : BaseInfo(eIcon) {};
		};
		typedef std::unique_ptr<CustomIconInfo> CustomIconInfoPtr;
		typedef std::list<CustomIconInfoPtr> CustomIconList;

		struct CustomLineSub
		{
			std::string		m_strSubName;
			PenPtr		    m_penPtr;
			std::vector<wxPoint> m_vector;
			int32_t         m_index{ 0 };
		};

		struct CustomLineInfo : public BaseInfo
		{
			std::string		m_strName;
			PenPtr		    m_penPtr;
			std::vector<wxPoint> m_vector;
			std::vector<CustomLineSub> m_subList;

			CustomLineInfo() : BaseInfo(eLine) {};
		};
		typedef std::unique_ptr<CustomLineInfo> CustomLineInfoPtr;
		typedef std::list<CustomLineInfoPtr> CustomLineList;

		struct CustomLineArcSub
		{
			std::string		m_strSubName;
			PenPtr		    m_penPtr;
			wxPoint         m_leftTop;
			wxSize          m_size;
			std::pair<float64_t, float64_t> m_startEnd; //<! 角度
			int32_t         m_index{ 0 };
		};

		struct CustomLineArcInfo : public BaseInfo
		{
			std::string		m_strName;
			PenPtr		    m_penPtr;
			wxPoint         m_leftTop;
			wxSize          m_size;
			std::pair<float64_t, float64_t> m_startEnd; //<! 角度
			std::vector<CustomLineArcSub> m_subList;

			CustomLineArcInfo() : BaseInfo(eLineArc) {};
		};
		typedef std::unique_ptr<CustomLineArcInfo> CustomLineArcInfoPtr;
		typedef std::list<CustomLineArcInfoPtr> CustomLineArcList;

		struct CustomShapeInfo : public BaseInfo
		{
			std::string		m_strName;
			Shape    m_shape;
			wxPoint  m_pt;
			wxSize   m_size;
			wxBrush  m_brush;
			bool     m_hasBrush{ false };
			wxColour m_colour;

			CustomShapeInfo() : BaseInfo(eShape) {};
		};
		typedef std::unique_ptr<CustomShapeInfo> CustomShapeInfoPtr;
		typedef std::list<CustomShapeInfoPtr> CustomShapeList;

		struct CustomBackgroundInfo : public BaseInfo
		{
			std::string		m_strName;
			BackgroundPtr	m_backgroundPtr;
			wxRect			m_rc;
			bool            m_show{ true };

			CustomBackgroundInfo() : BaseInfo(eBackground) {};
		};
		typedef std::unique_ptr<CustomBackgroundInfo> CustomBackgroundInfoPtr;
		typedef std::list<CustomBackgroundInfoPtr> CustomBackgroundList;

	protected:
		IControlRes* m_pControlRes{ nullptr };
		CustomTextList				m_textElements;
		CustomIconList				m_iconElements;
		CustomLineList				m_lineElements;
		CustomLineArcList			m_lineArcElements;
		CustomShapeList				m_shapeElements;
		CustomBackgroundList		m_backgroundElements;
	};
}
