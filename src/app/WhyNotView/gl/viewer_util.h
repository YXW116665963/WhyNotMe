#pragma once
#include "CGUtils/CGUtils.h"

using namespace CGUTILS;

class ViewerUtils
{
public:
    //  返回6个坐标轴（包含正负轴）中与dir最贴近的轴
    static Vector3f normalizeToAxis(const Vector3f& dir, const Vector3f& localX, const Vector3f& localY, const Vector3f& localZ);
};