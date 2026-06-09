#pragma once

namespace why
{
    class ViewerSetting
    {
    public:
        //适配不同客户端屏幕缩放比例
        //只在类里声明了 static 变量，没有再进行定义 → 导致链接报错：无法解析的外部符号
        //声明：告诉编译器 “有这个变量”
        //定义：给变量真正分配内存
         static float devicePixelRatio;
    };
}

