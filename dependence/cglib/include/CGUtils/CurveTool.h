#pragma once
#include <list>
#include <map>
#include <vector>
#include "CGLib/Vector3.h"
#include "CGLib/Vector2.h"
#include "CGLib/Line.h"
#include "CGLib/TriangleMesh.h"

using namespace std;

namespace CGUTILS
{
    class PairInt3PtCompareInt {
    public:
        bool operator()(const pair<int, pair<int, int>>& value1, const pair<int, pair<int, int>>& value2)const {
            int temp = abs(value1.first - value2.first) + abs(value1.second.first - value2.second.first) + abs(value1.second.second - value2.second.second);
            if (temp < 13)
                return false;

            return value2 < value1;
        }
    };

    class PairInt3DisCompareInt {
    public:
        bool operator()(const int value1, const int value2)const {
            if (abs(value1 - value2) < 20/*13*/)
                return false;

            return value2 < value1;
        }
    };

    class TriangleMeshWrapper
    {
    public:
        TriangleMeshWrapper(TriangleMesh* mesh) : mMesh(mesh)
        {
            //  此处的mesh已是顶点合并处理后的
            bool hasUv = mMesh->GetUVCount() == mMesh->GetVertexCount();
            for (size_t i = 0; i < mMesh->GetVertexCount(); i++)
            {
                auto& vert = mMesh->GetVertex(i);
                auto& normal = mMesh->GetNormal(i);

                auto vertKey = make_pair(make_pair(make_pair(make_pair(make_pair(roundf(vert.X * ptMergePara), roundf(vert.Y * ptMergePara)), roundf(vert.Z * ptMergePara)), roundf(normal.X * ptMergePara)), roundf(normal.Y * ptMergePara)), roundf(normal.Z * ptMergePara));
                auto itrFind = vertexesWithNormal.find(vertKey);
                if (itrFind != vertexesWithNormal.end())
                {
                    //throw exception("find unMerged vertex of mesh on vertexes merging.");
                    continue;
                }

                vertexesWithNormal.insert(make_pair(vertKey, i));
            }
        }

        int AddVertex(const Vector3f& vert, const Vector3f& normal)
        {
            return AddVertex(vert, normal, nullptr);
        }

        int AddVertex(const Vector3f& vert, const Vector3f& normal, const Vector2f& uv)
        {
            return AddVertex(vert, normal, &uv);
        }

        const Vector3f& GetVertex(int index) const
        {
            return mMesh->GetVertex(index);
        }

        int GetVertexCount() const
        {
            return mMesh->GetVertexCount();
        }

        const Vector3f& GetNormal(int index) const
        {
            return mMesh->GetNormal(index);
        }

        int GetUVCount() const
        {
            return mMesh->GetUVCount();
        }

        const Vector2f& GetUV(int index) const
        {
            return mMesh->GetUV(index);
        }

        map<pair<pair<pair<pair<pair<float, float>, float>, float>, float>, float>, int> vertexesWithNormal;
        TriangleMesh* mMesh;
        const static int ptMergePara = 1e5;

    private:
        int AddVertex(const Vector3f& vert, const Vector3f& normal, const Vector2f* uv)
        {
            auto vertKey = make_pair(make_pair(make_pair(make_pair(make_pair(roundf(vert.X * ptMergePara), roundf(vert.Y * ptMergePara)), roundf(vert.Z * ptMergePara)), roundf(normal.X * ptMergePara)), roundf(normal.Y * ptMergePara)), roundf(normal.Z * ptMergePara));
            auto itrFind = vertexesWithNormal.find(vertKey);
            if (itrFind != vertexesWithNormal.end())
                return itrFind->second;

            int nRe = mMesh->GetVertexCount();
            vertexesWithNormal.insert(make_pair(vertKey, nRe));
            mMesh->AddVertex(vert);
            mMesh->AddNormal(normal);
            if (uv != nullptr)
                mMesh->AddUV(*uv);
            return nRe;
        }
    };

    struct TriangleData
    {
        //  TODO 还要记录meshId，方便对三角切割时激动计算uv等
        unsigned int verts[3];
        int indexOrigin;

        TriangleMeshWrapper* mesh;
        //TriangleMesh* mesh;
        //const TriangleMesh* oriMesh;
    };

	class CGLIB_EXPORTS CurveTool
	{
    public:
        /// <summary>
        /// 弧度角规范表示为0-2PI之间
        /// </summary>
        static double NormalizeAngle(double regAng);
        static bool IsWholeCircle(double angStart, double angEnd, bool large, bool clockWise);

        static bool IsOnLine(const Vector3d& pt, const Vector3d& pt1, const Vector3d& pt2, float tolerance = 1e-3, float tolerance2 = 1e-4);
        static double distanceToUnboundLine(const Vector3f& point, const Vector3f& linePt0, const Vector3f& linePt1, Vector3f& ptTarget);
        static double distanceToUnboundLine2(const Vector3f& point, const Vector3f& linePt0, const Vector3f& linePt1, Vector3f& ptTarget);

        //  废弃，不建议使用，因为a b c为normal.normalized()，有精度损失。建议使用下面的同名函数
        static double distanceToUnboundPlane(const Vector3f& point, const Vector3f& normal, const Vector3f& ptOnPlane);

        //  精度较高，推荐使用
        static double distanceToUnboundPlane2(const Vector3f& point, const Vector3f& planeNormalNotNormalized, const Vector3f& ptOnPlane);

        static double distanceToUnboundPlane(const Vector3f& point, double a, double b, double c, double d);

        //  返回值：三角面在超平面上时为false；其他情况为true
        static bool PlaneCutTriangle(double a, double b, double c, double d, const TriangleData& tri,
            vector<TriangleData>& resultIn, vector<TriangleData>& resultOut);

        //  废弃，不建议使用，因为a b c为normal.normalized()，有精度损失。建议使用下面的同名函数
        static int PlaneCutLine(const Vector3f& vert1, const Vector3f& vert2, float a, float b, float c, float d, Vector3f& inter, bool& node1Out);

        //  精度较高，推荐使用
        static int PlaneCutLine(const Vector3f& vert1, const Vector3f& vert2, const Vector3f& planeNormalNotNormalized, double d, Vector3f& inter, bool& node1Out);

        static int PlaneCutPoint(const Vector3f& point, float a, float b, float c, float d);

        static Vector3f GetNormal(const list<Line>& polygon);
        static Vector3f GetNormal(const list<Line>& polygon, double& area);

        //  请注意：尚未测试
        static float Determinant(const vector<std::vector<float>>& data);
        static float Determinant2(const vector<std::vector<float>>& data);

        //  最新的，待测试是否精度更高
        //  返回值：0代表在平面外面；1代表完全在平面里面；2代表在切割了；【3和4 代表切割端点；其中3代表在平面外面，4代表在平面里面；其中node1Out代表是否切割的是端点1】；-1代表在平面上
        static int UnBoundLineCutLine4Inter(const Line& unboundLine, const Vector3f& vert1, const Vector3f& vert2, const Vector3f& normalNotNormalized, Vector3f& inter, bool& node1Out);
        static bool UnBoundLineCutLine2(const Vector3f& normal, const Line& unboundLine, const Line& target, list<Line>& in, list<Line>& out);

        //  返回值：0代表在右侧（外面）；1代表完全在左侧（里面）；2代表在切割了；-1代表在线上；-2代表异面
        static int UnBoundLineCutLine(const Vector3d& normal, const Line& unboundLine, const Line& target, Vector3f& inter, bool& node1Out);
        //  返回值：线在直线上时为false；其他情况为true
        static bool UnBoundLineCutLine(const Vector3f& normal, const Line& unboundLine, const Line& target, list<Line>& in, list<Line>& out);
        static int UnBoundLineCutPoint(const Vector3f& normal, const Line& unboundLine, const Vector3f& target, double& disToUnboundLine);

        static void UpdateCuttedTrisInfo(const TriangleData& tri, bool node0Out, unsigned int vert0, unsigned int vert1, unsigned int vert2, const Vector3f& inter01, const Vector3f& inter12,
            vector<TriangleData>& resultIn, vector<TriangleData>& resultOut);
        static void UpdateCuttedTrisInfo_cutOneLine(const TriangleData& tri, bool node0Out, unsigned int vert0, unsigned int vert1, unsigned int vert2, const Vector3f& inter01,
            vector<TriangleData>& resultIn, vector<TriangleData>& resultOut);
        static void CalNormalAndUV(const TriangleData& tri, int vert0, int vert1, const Vector3f& inter01, Vector3f& normalRe, Vector2f& uvRe, bool& hasUV);

        static void GetPlanePara(const Vector3f& vert0, const Vector3f& vert1, const Vector3f& vert2, float& a, float& b, float& c, float& d);
        static void GetPlanePara(const Vector3f& normal, const Vector3f& pointOnPlane, float& a, float& b, float& c, float& d);

        //  空间两直线（可能为异面）的最短距离
        static double DistanceOfUnboundLines(const Line& unboundLine0, const Line& unboundLine1);
            

        //static bool LineIntersect(const Line& line1, const Line& line2, Vector3f& intersection);
        static bool LineIntersect(const Line& line1, const Line& line2, Vector3d& intersection);
        static bool LineIntersect2(const Line& line1, const Line& line2, Vector3d& result);

        static void Project(const Vector3f& point, const Vector3f& pt0, const Vector3f& pt1, Vector3f& prjPt);

        static void GetPointByParameter(const Line& line, float parameter, Vector3f& re);
        static bool GetParameter(const Line& line, const Vector3f& point, double& parameter);
        static double GetParameter_NotCheck(const Line& line, const Vector3f& point);
        static double GetDirectedParameter_NotCheck(const Line& line, const Vector3f& point);
        static double GetProjectDirectedParameter_NotCheck(const Line& line, const Vector3f& point);

        //  对线集合进行去重归并
        static void duplicateRemoval(const Vector3f& boundBoxMin, list<Line>& lines, bool ignoreDir);
        static void GetNodeParameter(const Line& line, const Vector3f& pt0, const Vector3f& pt1, double& paraLower, double& paraUpper);

	};

}