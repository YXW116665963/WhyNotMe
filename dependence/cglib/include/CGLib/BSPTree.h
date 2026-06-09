#pragma once
#include "CGUtils/CurveTool.h"

namespace CGUTILS
{
    struct TriangleData;
    class PlaneBSPTree
    {
    public:
        PlaneBSPTree() : a(0.0f), b(0.0f), c(0.0f), d(0.0f), Left(0), Right(0)
        {
        }

        //  超平面
        //Vector3f point;
        //Vector3f normal;
        float a;
        float b;
        float c;
        float d;

        //  节点内容：三角面
        //vector<Vector3f> triangles;
        list<TriangleData> tris;

        //  左子树
        PlaneBSPTree* Left;

        //  右子树
        PlaneBSPTree* Right;

    };

    class LineInfo
    {
    public:
        LineInfo(int edgIndex, const Line& ln) : edgeIndex(edgIndex), line(ln)
        {
        }

        int edgeIndex;
        Line line;
    };

    class LineBSPTree
    {
    public:
        LineBSPTree() : Left(0), Right(0)
        {
        }

        LineBSPTree(const Line& ln) : line(ln), Left(0), Right(0)
        {
        }

        Line line;

        list<LineInfo> lines;

        LineBSPTree* Left;
        LineBSPTree* Right;

    };
}
