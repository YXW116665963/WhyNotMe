#pragma once

#include "standard.h"
#include "gui_import.h"

namespace why
{
	class IBackground
	{
	public:
		/*!
		* 绘制背景
		* @param dc 绘图接口
		* @param rcDraw 背景被绘制的区域
		* @return
		* - true	绘制成功
		* -	false	绘制失败
		*/
		virtual bool Draw(wxDC &dc, const wxRect &rcDraw) = 0;

		/*!
		* 绘制背景
		* @param pGraphic 绘图接口
		* @param rcDraw 背景被绘制的区域
		* @return
		* - true	绘制成功
		* -	false	绘制失败
		*/
		virtual bool Draw(wxGraphicsContext* pGraphic, const wxRect& rcDraw) = 0;

		/*!
		* 销毁背景对象
		* return void
		*/
		virtual void Destroy() = 0;
	};

	typedef std::shared_ptr<IBackground> BackgroundPtr;
}
