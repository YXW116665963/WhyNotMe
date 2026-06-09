#pragma once
#include <list>
#include <map>
#include <vector>
#include "CGLib/Vector3.h"
#include "CGLib/Line.h"
#include "CGUtils.h"

using namespace std;

namespace CGUTILS
{
    class PointBorderData4Boolean
    {
    public:
        Vector3f Point;
        list<int> lstBorderAlong;

        bool operator< (const PointBorderData4Boolean& other) const;
        bool operator== (const PointBorderData4Boolean& other) const;
    };

    class CGLIB_EXPORTS BooleanUtils
    {
        //  约定逆时针方向为实体轮廓，顺时针方向为负实体轮廓
    public:
        static void AntiPolygon(const list<Line>& polygon, list<Line>& result);
        static void AntiPolygon(const list<list<Line>>& polygon, list<list<Line>>& result);

        static void Union(const Vector3f& normal, const list<Line>& outer1, const list<list<Line>>& inner1, const list<Line>& outer2, const list<list<Line>>& inner2, list<list<Line>>& result);

        static void Difference(const Vector3f& normal, const list<Line>& outer1, const list<list<Line>>& inner1, const list<Line>& outer2, const list<list<Line>>& inner2, list<list<Line>>& result);

        static list<list<Line>> ExclusiveOr();

    private:
        static bool bAClockwise1/* = false*/;
        static bool bAClockwise2/* = false*/;

    public:
        static void Intersect(const Vector3f& normal, const list<Line>& outer1, const list<list<Line>>& inner1, const list<Line>& outer2, const list<list<Line>>& inner2, list<list<Line>>& result);

        static bool bFindValidPath/* = false*/;
    private:
        static void GetPolygonIntersect(const vector<PointBorderData4Boolean>& points1, const map<int, int>& mapSectionPtCount1, const vector<PointBorderData4Boolean>& points2, const map<int, int>& mapSectionPtCount2,
            const LineBSPTree* polygon1, const Vector3f& normal1, const LineBSPTree* polygon2, const Vector3f& normal2, list<vector<PointBorderData4Boolean>>& inters);

        static int GetNotTouchNode(const vector<PointBorderData4Boolean>& nodes, const list<vector<PointBorderData4Boolean>>& paths);

        //  如果已走的路径连续两次添加已添加的连续的节点，则认为路径闭合
        static bool PreLabel/* = false*/;
        static int indexPre/* = -1*/;

        static void UpdatePath(int indexCurrent, const vector<PointBorderData4Boolean>& pointsCurrent, int firstSectionPtCountCurrent, const list<int>& lstOtherSectionPtCountCurrent,
            const vector<PointBorderData4Boolean>& pointsAnother, int firstSectionPtCountAnother, const list<int>& lstOtherSectionPtCountAnother, const LineBSPTree* polygonCurrent, const Vector3f& normalCurrent, bool bAClockwiseCurrent,
            const LineBSPTree* polygonAnother, const Vector3f& normalAnother, bool bAClockwisePolygonAnother, vector<PointBorderData4Boolean>& path);

        static bool IsOnValidSameBorder(const PointBorderData4Boolean& data1, const PointBorderData4Boolean& data2,
            const LineBSPTree* polygonAnother, const Vector3f& normalAnother, bool polygonAClockwise);

        static int GetSamePoint(const PointBorderData4Boolean& dataSource, const vector<PointBorderData4Boolean>& lstData);

        static void GetCurveInPolygonVertex(const Line& curve, int borderIndex, int curOuterEdgeCount, const list<int>& lstCurInnerEdgeCount,
            const list<Line>& polygon, const Vector3f& normal, const LineBSPTree* bspTree, bool polygonAClockwise, map<pair<const Line*, const Line*>, pair<bool, Vector3f>>& mapInterCache, list<PointBorderData4Boolean>& result);

        static void getPolygonAInB(const list<Line>& polygonEdges1, int outerEdgeCount1, const list<int>& lstInnerEdgeCount1, const list<Line>& polygonEdges2,
            const Vector3f& normal2, const LineBSPTree* bspTree2, bool bAClockwise2, map<pair<const Line*, const Line*>, pair<bool, Vector3f>>& mapInterCache, vector<PointBorderData4Boolean>& points1, map<int, int>& mapSectionPtCount1);
    };

};