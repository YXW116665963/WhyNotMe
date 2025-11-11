#pragma once

#include "standard.h"
#include "gui_import.h"
#include <map>

#include "ui_util.h"

namespace why
{
	class Picture
	{
	public:
		Picture();
		~Picture();

	public:
		/*!
		* 获取图片宽度
		* @return 图片宽度
		*/
		uint32_t Width();

		/*!
		* 获取图片高度
		* @return 图片高度
		*/
		uint32_t Height();

		/*!
		* 绘制整个图片到目标设备，图标按1：1的方式绘制，不做缩放操作
		* @param dc 绘图设备
		* @param ptDst 图片绘制的位置
		* @return void
		*/
		void DrawImage(wxDC& dc, const wxPoint& ptDst);

		/*!
		* 绘制整个图片到目标设备的指定区域，指定区域大小和图片的尺寸不一致时，会缩放操作
		* @param dc 绘图设备
		* @param ptDst 图片绘制的位置
		* @param szDst 图片在绘图设备上的显示尺寸
		* @return void
		*/
		void DrawImage(wxDC& dc, const wxPoint& ptDst, const wxSize& szDst);

		/*!
		* 绘制旋转图片
		* @param dc 绘图设备
		* @param ptDst 图片绘制的位置
		* @param szDst 图片在绘图设备上的显示尺寸
		* @param rotate 逆时针旋转百分比
		* @param rotatePt 旋转中心点坐标
		* @return void
		*/
		void DrawImage(wxDC& dc, const wxPoint& ptDst, const wxSize& szDst, const double & rotate, const wxPoint& rotatePt);

		/*!
		* 绘制图片的一部分到目标设备，图标按1：1的方式绘制，不做缩放操作
		* @param dc 绘图设备
		* @param ptDst 图片绘制的位置
		* @param ptSrc 图片上开始的
		* @param szSrc 部分图片的尺寸
		* @return void
		*/
		void DrawImage(wxDC& dc, const wxPoint& ptDst, const wxPoint& ptSrc, const wxSize& szSrc);

		/*!
		* 绘制图片的一部分到目标设备的指定区域，指定区域大小和图片的部分尺寸不一致时，会缩放操作
		* @param dc 绘图设备
		* @param ptDst 图片绘制的位置
		* @param szDst 图片在绘图设备上的显示尺寸
		* @param ptSrc 图片上开始的
		* @param szSrc 部分图片的尺寸
		* @return void
		*/
		void DrawImage(wxDC& dc, const wxPoint& ptDst, const wxSize& szDst, const wxPoint& ptSrc, const wxSize& szSrc);

		/*!
		* 绘制图片的一部分到目标设备的指定区域，指定区域大小和图片的部分尺寸不一致时，会缩放操作
		* @param dc 绘图设备
		* @param rcDst 图片绘制的区域
		* @param rcSrc 图片上被绘制的部分所占用的区域
		* @return void
		*/
		void DrawImage(wxDC& dc, const wxRect& rcDst, const wxRect& rcSrc);

		/*!
		* 将图片的整体绘制到目标设备的指定区域，指定区域大小和图片的部分尺寸不一致时，会缩放操作
		* @param dc 绘图设备
		* @param rcDst 图片绘制的区域
		* @return void
		*/
		void DrawImage(wxGraphicsContext *pGraphic, const wxRect& rcDst);

		/*!
		* 绘制图片的一部分到目标设备的指定区域，指定区域大小和图片的部分尺寸不一致时，会缩放操作
		* @param dc 绘图设备
		* @param rcDst 图片绘制的区域
		* @param rcSrc 图片上被绘制的部分所占用的区域
		* @return void
		*/
		void DrawImage(wxGraphicsContext* pGraphic, const wxRect& rcDst, const wxRect& rcSrc);

		/*!
		* 绘制旋转图片
		* @param pGraphic 绘图设备
		* @param ptDst 图片绘制的位置
		* @param szDst 图片在绘图设备上的显示尺寸
		* @param rotate 逆时针旋转百分比
		* @param rotatePt 旋转中心点坐标
		* @return void
		*/
		void DrawImage(wxGraphicsContext *pGraphics, const wxPoint& ptDst, const wxSize& szDst, const double & rotate, const wxPoint& rotatePt);

		/*!
		* 获取像素的颜色
		* @param ptPos 像素的位置
		* @return 像素颜色
		*/
		wxColour GetPixel(const wxPoint& ptPos);

	public:
		/*!
		* 创建图标接口
		* @param lpFileName 图片文件的文件名称
		* @return void
		*/
		bool CreatePicture(const wxBitmap &bitmap);

		/*!
		* 创建图标接口
		* @param lpFileName 图片文件的文件名称
		* @return void
		*/
		bool CreatePicture(const wxString &strFileName);

		/*!
		* 创建单色图标接口
		* @param lpFileName 图片文件的文件名称
		* @return void
		*/
		bool CreateGrayPicture(uint32_t uWidth, uint32_t uHeight, const uint8_t* pData);
		bool CreateRGBPicture(uint32_t uWidth, uint32_t uHeight, const uint8_t* pData);
		/*!
		* 将图片保存到文件
		* @param lpFileName 图片文件的文件名称
		* @return 
		* - true 保存成功
		* - false 保存失败
		*/
		bool SaveToFile(const wxString &strFileName);

		wxImage Scale(int32_t width, int32_t height);

	private:
		void DrawImage(wxDC& dc, const wxImage& image, const wxPoint &ptPos);
		void DrawImage(wxGraphicsContext* pGraphic, const wxImage& image, const wxPoint& ptPos);
		
		wxImage Scale(const wxRect &rt, int32_t width, int32_t height);

	private:
		typedef std::map<std::pair<int32_t, int32_t>, wxImage> ScaleChaches;

	private:
		wxString					m_strFileName;
		wxImage						m_image;
		ScaleChaches				m_scaleChaches;
	};

	typedef std::shared_ptr<Picture> PicturePtr;
}
