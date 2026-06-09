#ifndef __CGUTILS_H__
#define __CGUTILS_H__

#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include "CGLib/Vector3.h"
#include "CGLib/Sphere.h"
#include "CGLib/Line.h"
#include "CGLib/Body.h"
#include "CGLib/Arc.h"
#include "CGLib/Transform.h"
#include "CGLib/TriangleMesh.h"
#include "CGLib/CGLibDefine.h"
#include "CGUtils/CurveTool.h"
#include "CGLib/Plane.h"
#include "CGLib/BSPTree.h"

using namespace std;

namespace CGUTILS
{
    class DescendingCompare {
    public:
        bool operator()(const double value1, const double value2)const {
            return value2 < value1;
        }
    };

    class DescendingCompareInt {
    public:
        bool operator()(const int value1, const int value2)const {
            return value2 < value1;
        }
    };

    class PointBorderData;
    bool ComparePoint(const Line& line, const PointBorderData& p1, const PointBorderData& p2);
    bool ComparePointBySimplePara(const Line& line, const Vector3f& p1, const Vector3f& p2);
    bool ComparePoint2(const Line& line, const Vector3f& p1, const Vector3f& p2);

    enum CGLIB_EXPORTS PointInPolygonType
    {
        Unknown,
        Inside,
        Outside,
        OnEdge,
    };

    struct PointInPolygonResult
    {
        PointInPolygonType type;
        int edgeIndex;  //当点在轮廓边上时，记录边的索引
    };

    enum CGLIB_EXPORTS PointInTriangleType
    {
        PIT_Unknown,
        PIT_InsideOrOnEdge,
        PIT_Outside,
        PIT_OnVertex,
    };

    class PointBorderData
    {
    public:
        PointBorderData() : flag(-1), flag2(-1)
        {
        }

        Vector3f point;
        map<int, float> mapBorderIndex2Para;

        int flag;  //根据需要使用，如记录对应切割体的切割边标识

        int flag2;  //根据需要使用

        bool operator< (const PointBorderData& other) const;
        bool operator== (const PointBorderData& other) const;

    };

    class CGLIB_EXPORTS PointData_1e_3
    {
    public:
        PointData_1e_3(const Vector3f& pt);

        Vector3f point;

        bool operator< (const PointData_1e_3& other) const;
        bool operator== (const PointData_1e_3& other) const;
        static bool IsLess(const PointData_1e_3& pt0, const PointData_1e_3& pt1);
    };

    class PointData
    {
    public:
        Vector3f point;
        PointData(const Vector3f& pt): point(pt) {}

        bool operator< (const PointData& other) const;
        bool operator== (const PointData& other) const;
    };

    class AdjoinInfoBody
    {
    public:
        AdjoinInfoBody();
        AdjoinInfoBody(const Body& body);
        AdjoinInfoBody(const list<Face>& lstFace);
        AdjoinInfoBody(const TriangleMesh& mesh);

        void AddOneFace(const Face& face);
        int GetOrUpdateVertexIndex(const Vector3f& vertex);
        int GetVertexIndex(const Vector3f& vertex) const;
        int GetBorderIndex(const pair<int, int>& border) const;
        int UpdateBorderIndex(const Line& line);
        int GetBelongFaceIndex(int borderIndex, int& edgeIndex) const;
        int UpdateFaceIndex(const Face& face, int originFaceIndex);

        void GetBelongFaces(int indexPt0, int indexPt1, vector<int>& originFIndex) const;
        int GetOriginFaceIndex(int indexFace) const;
    public:
        vector<Vector3f> vertexes;
        vector<pair<int, int>> borders;
        vector<pair<vector<int>, vector<vector<int>>>> faces;
        vector<int> originFacesIndex;

        //map<pair<pair<int, int>, int>, int> mapXYZ2Index;
        map<PointData, int> mapXYZ2Index;

    };

    class RoundingData
    {
    public:
        bool rounding;

        double faceAngle;
        Vector3f normal;
        Line lineTop;
        Line lineBottom;
    };

    class PointIndexFlagData
    {
    public:
        int index;
        bool flag;
    };

    class BoxPos
    {
        char pos;

    };

    //1、每个faceIn都遵循其边被其它faceIn或cutFace紧密包围；
    //2、没有歧义（无效inter direction）的faceIn（其对应的node在被切割时没有歧义）视为“榜样者”，具有引领推动力；没有被cut的faceIn视为“传播者”，其具有传递力；有歧义的node状态待定，由前述力量判断；
    //3、由“榜样者”开始引领推动，通过“传播者”来验证和标识其他faceIn的准确性，准确的faceIn作为“传播者”参与进去；不准确的node进行细化，并被传递标记（其所属一部分，即childNode被传递后作为“传播者”），直到被完全传播标记。此过程中node（不准确的node）的faceOut有可能转化为faceIn；
    //4、没有被确认准确性的faceIn可能要被转换为faceOut，如由于局部face点序不规范导致其切割的相邻node出现“假”faceIn的情况；
    enum NodeFlag
    {
        Promoter,  //倡导者，对应被cut但无歧义（face被cut时没有无效inter direction）的node，具有引领推动力
        Disseminator,  //传播者，对应（其face）没有被cut的node，具有传递力
        ToBeMarked,  //待定者，有歧义的node状态待定，由前述力量判断；
        InValid,  //无效者，没有被标记的
    };

    class NodeReCalCacheData
    {
    public:
        NodeReCalCacheData() : caledCuttedNodePolygons(false) { }

        //  共面传递时重新计算缓存数据，避免重复计算
        list<list<Line>> cuttedNodePolygons;
        vector<vector<pair<int, int>>> cuttedNodePolygonsBoderFlags;

        bool caledCuttedNodePolygons;
        map<int, bool> mapInnerFlagedPolyIndex;
    };

    class OctTree
    {
    public:
        OctTree() : flag(-1), pos(-1), deep(-1), mesh(nullptr), useOnCut(false), calibrateType(-1), nodeFlag(NodeFlag::InValid), transmitted(false)
        {
            for (size_t i = 0; i < 8; i++)
            {
                child[i] = nullptr;
                if (i < 6)
                    faceFlag.push_back(-1);
            }
        }

    public:
        OctTree* parent;
        OctTree* child[8];

        //  pair<起点，终点>，[0,3)
        vector<int> triIndexesInPart;  //  包含的三角面
        //vector<int> triIndexesCutPart;  //  切割的三角面

        vector<int> faceFlag;
        int flag;  //-1:invalid；0:out；1:in；2cutted
        char pos;  //位置
        unsigned short deep;  //八叉树深度最大为12
        Vector3f min;
        Vector3f max;
        //list<Body> partsOut;
        //list<Body> partsIn;  //  TODO是不是只用partsIn就可以了
        list<Face> facesIn;  //node内部的faces（切割后的）

        bool useOnCut;  //用作为source node cut target node

        //  记录被sourceNode切割后的面<start, count>
        pair<int, int> cuttedFaceIn;
        pair<int, int> cuttedFaceOut;

        //  记录每个cuttedNodeFaceIn被切割后对应的结果，方便共面传递后对结果进行整理更新
        vector<pair<int, int>> cuttedNodeFaceInIndex2CntOut;
        vector<pair<int, int>> cuttedNodeFaceInIndex2CntIn;

        //  被纠偏的node需要记录纠偏后的数据，方便进行传递
        //bool bCalibrated;
        int calibrateType;  //0: bCalibrated; 1: 共面传递重新计算纠偏
        list<Face*> cuttedNodeFacesIn;  //calibrateType == 0时使用
        list<Face> calibrateFacesIn;  //calibrateType == 1时使用
        map<int, bool> mapFaceIndex2Transed;  //每一轮传递时跳过以前传递过的

        //  （1）记录面传递过的信息：标记下被传递过的cuttedFaceIn。对于calibrateType == 1的node，facesIn为：标记过的cuttedFaceIn + calibrateFacesIn  20240812 21:19
        //  （2）一个node中可能有多个cuttedFaceIn，有的face是正常的（即能被正常的传递），有的face是“不正常的”（可能是有计算错误，有的part应该是out，被错误的计算为in），还有的part是缺失的（理应是in的）
        //  这样标记过后即认为是“经过检验的”，如果后续发现此node需要被重新计算纠偏，则经过检验的部分均归并到calibrateFacesIn中
        //  （3）这样的node,开始时为正常被传递，其所有的cuttedFaceIn（不管是标记过的还是未标记的）都参与预传递判断；当发现需要计算纠偏时，将标记过的cuttedFaceIn归并到calibrateFacesIn中；
        //  后续再次被传递时（calibrateType==1）即以所有的calibrateFacesIn参与预传递判断；
        list<pair<int, list<Line>>> lstSpreadedCuttedFaceIn;

        //  共面传递时重新计算缓存数据，避免重复计算
        map<int, NodeReCalCacheData> mapTargetFaceIndex2ReCalCache;

        NodeFlag nodeFlag;  //用于共面传递
        bool transmitted;  //传递过了后标记一下

        const TriangleMesh* mesh;
    };

    struct SrOBBox3D
    {
        Vector3f mCenter;
        Vector3f mAxis[3];
        float mHalfLength[3];
    };

    struct SplitEdgeData
    {
        int type;  //1:faces  2:lines
        const Face* face;
        const Line* line;
    };

    class PolygonTree
    {
    public:
        PolygonTree() : parent(nullptr), flag(-1), deep(-1)
        {
        }

    public:
        PolygonTree* parent;
        vector<PolygonTree*> childrens;

        int flag;  //按需赋予意义，如当前轮廓在轮廓列表中对应的原始索引
        list<list<Line>>::const_iterator polygon;

        unsigned short deep;  //deep为偶数时代表实体轮廓，为奇数时代表负实体轮廓
        //Vector3f min;
        //Vector3f max;
    };

    class CGLIB_EXPORTS CGUtils
    {
    public:
        //  拉伸
        static void GenerateHColumnMesh(float H, float B1, float B2, float T1, float T2, float S, float height, const Vector3f& insert, TriangleMesh& mesh);
        static void GenerateHColumn(float H, float B1, float B2, float T1, float T2, float S, float height, const Vector3f& insert, Body& body);
        static void GenerateCube(const vector<Vector3f>& sectionPts, const Vector3f& normal, float height, Body& cube);
        static void GenerateCube(const Vector3f& pt0, const Vector3f& pt1, const Vector3f& pt2, const Vector3f& pt3, const Vector3f& pt4, const Vector3f& pt5,
            const Vector3f& pt6, const Vector3f& pt7, Body& cube);
        static void GenerateCircleColumn(float radius, float height, const Vector3f& insert, Body& body);

        static void ExtrusionPolygonTree(const PolygonTree* polygonTree, const Vector3f& normal, double height, Body& body);

        static void GetPointsFromPolygon(const list<Line>& polygon, vector<Vector3f>& verts);

        //  放样
        static void CreateLoftingSolid(const vector<Vector3f>& profile, const vector<Line>& path, Body& lofting);
        static void GetBisectPlaneNormal(const Vector3f& dir1, const Vector3f& dir2, Vector3f& normal);
        static void GetAnotherNodeProfile(const vector<Vector3f>& profile, const Vector3f& dir, const Plane& plane, vector<Vector3f>& profileRe);
        static void AdjustProfileToNode(const vector<Vector3f>& profile, const Vector3f& nodeOri, const Vector3f& dirOri, const Vector3f& nodeTarget, const Vector3f& dirTarget,
            vector<Vector3f>& profileRe);
        static void PushPathPart2Solid(const vector<Vector3f>& profile1, const vector<Vector3f>& profile2, Body& lofting);

        //  曲面造型（TODO）

        //  最短距离
        static double SqureDistanceSegmentToSegment(const Line& line0, const Line& line1);

        //  切割
        static bool Cut(const Body& body, const Face& cutFace, list<Body>& keepParts, list<Body>& cuttedParts);

        //  局部造型
        //  倒圆角
        static bool Rounding(const Line* top, const Line* bottom, double faceAngle, const Vector3f& normal, list<Line>& topRe, list<Line>& bottomRe);
        static void GetTesslateRoundingArc(const Vector3f& pt0, const Vector3f& pt1, double faceAngle, const Vector3f& normal, list<Line>& roundingArc);

        //  倒角
        static bool Chamfer(Body& body, const Line& targetBorder, float expand, bool rounding);
        static void GetCuttedFaceParts(const Face& faceTarget, const vector<PointBorderData>& lstInter, list<Face>& facesIn, list<Face>& facesOut);
        static void FaceCutTargetFaces(const Body& bodyTarget, const Face& faceSource, const Vector3f& normalSource,
            const vector<int>& targetFacesIndex, vector<bool>& cuttedFacesIndex, list<pair<vector<PointBorderData>, int>>& lstAllInter, list<Face>& facesIn, list<Face>& facesOut);
        static const Line* GetTargetLine(int indexLine, const list<Line>& lstLine);
        static const Line* GetTargetLine(int indexLine, const Face& face);
        static int GetNextPtFromCommonBorderFace(int faceIndex, int borderIndex, const AdjoinInfoBody& aBody, const Body& body, const Face& cutExFace,
            const Vector3f& normalExFace, vector<PointBorderData>& lstInter, list<Face>& facesIn, list<Face>& facesOut);
        static bool GetClosedBody(const Face& cutFace, const list<Face>& lstFace, list<Face>& facesOut, Body& result);
        static int GetAdjoinFace(const Line& line, const AdjoinInfoBody& aBody, map<int, bool>& mapData2Used, list<int>& result);
        static int GetAdjoinFace(const Face& face, const AdjoinInfoBody& aBody, map<int, bool>& mapData2Used, list<int>& result);
        static bool HasAdjoinFace_ForEdge(const Face& face, const AdjoinInfoBody& aBody, map<int, bool>& mapData2Used);  //是否有共边面

        static bool GetChamferCutExFace(const Body& body, const AdjoinInfoBody& aBody, const Line& targetBorder, float expand, Face& cutExFace,
            vector<int>& adjoinFacesIndex, RoundingData& roundingData);
        static bool CutBodyByFace(const Body& body, const AdjoinInfoBody& aBody, const Face& cutExFace, const RoundingData& roundingData, vector<int>& adjoinFacesIndex,
            list<Body>& keepParts, list<Body>& cuttedParts);
        static void CutBodyByFace(const Body& body, int totalFaceCnt, const Vector3f& normalExFace, const Face& cutExFace,
            vector<int>& adjoinFacesIndex, vector<bool>& cuttedFacesIndex, list<pair<vector<PointBorderData>, int>>& lstAllInter, list<Face>& facesIn, list<Face>& facesOut);
        static void GetCuttedSectionFaces(const Body& body, const AdjoinInfoBody& aBody, const Vector3f& normalExFace, const Face& cutExFace,
            list<Face>& facesIn, list<Face>& facesOut, list<Face>& facesOutApart, vector<bool>& cuttedFacesIndex, list<pair<vector<PointBorderData>, int>>& lstAllInter,
            list<Face>& cutFaces, list<Face>& cutFacesInverse);
        static void GetTargetInParts(const list<Face>& cutFacesInverse, list<Face>& facesIn, list<Body>& keepParts);
        static void GetTargetOutParts(const list<Face>& cutFaces, const list<Face>& lstOut, list<Face>& facesOut, list<Body>& cuttedParts);
        static void UpdateRoundingFace(const RoundingData& roundingData, Body& body);

        static bool isRayCrossPoint(const Vector3f& start, const Vector3f& end, const Vector3f& point);
        static bool isLineCrossZAxis(const Vector3f& start, const Vector3f& end, const Vector3f& point, Vector3f& interPt);
        static double distanceTo(const Vector3f& left, const Vector3f& right);
        //  plane2 cut plane1,交线是有方向的
        static bool planeIntersect(const Vector3f& point1, const Vector3f& normal1, const Vector3f& point2, const Vector3f& normal2, Vector3f& ptOnInterLine, Vector3f& dirInterLineNotNormalized);

        //  布尔运算
        static void GetTransformL2W(const list<Line>& polygon, Transform& trsL2W);
        static void GetTransformL2W2(const list<Line>& polygon, Transform& trsL2W);

        //  GetArea方法废弃（请使用GetArea2方法），误差较大：因为平移至第一象限处理带来了额外的浮点运算，造成额外的误差
        //static double GetArea(const list<Line>& polygon, const Transform& trsW2L);
        static double GetArea(const list<Line>& polygon, const Transform& trsW2L);
        static double GetTriangleArea(const Vector3f& pt0, const Vector3f& pt1, const Vector3f& pt2);

        static double GetVolume(const Body& body);

        static void GetFaceFromTri(const Vector3f& pt0, const Vector3f& pt1, const Vector3f& pt2, Face& faceTri);
        static void GetFaceFromTri(const TriangleMesh* mesh, int indexTri, Face& faceTri);

        template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
        static void LineIntersectPolygon(const Line& lineOri, _Iter first, _Iter last, bool recordBorder, const Face* faceTarget, vector<PointBorderData>& lstInterPt);
        static void LineIntersectFace(const Line& line, const Face& faceTarget, bool recordBorder, vector<PointBorderData>& lstInterPt);

        static int Partition(vector<PointBorderData>& lstPoint, int indexS, int indexE, bool (*compare)(const Line&, const PointBorderData&, const PointBorderData&), const Line& lineOri);
        static void QuikSort(vector<PointBorderData>& lstPoint, int indexS, int indexE, bool (*compare)(const Line&, const PointBorderData&, const PointBorderData&), const Line& lineOri);
        static int Partition(vector<vector<const PointBorderData*>>& lstDir, int indexS, int indexE, bool (*compare)(const vector<const PointBorderData*>&, const vector<const PointBorderData*>&));
        static void QuikSort(vector<vector<const PointBorderData*>>& lstDir, int indexS, int indexE, bool (*compare)(const vector<const PointBorderData*>&, const vector<const PointBorderData*>&));
        static void OperateInterDatasOnSizeIs3(const Line& lineOri, vector<PointBorderData>& intersTargetFace, vector<PointBorderData>& lstTemp);

        static int Partition(vector<Vector3f>& lstPoint, int indexS, int indexE, bool (*compare)(const Line&, const Vector3f&, const Vector3f&), const Line& lineOri);
        static void QuikSort(vector<Vector3f>& lstPoint, int indexS, int indexE, bool (*compare)(const Line&, const Vector3f&, const Vector3f&), const Line& lineOri);

        static void GetCuttedFaces(const Face& faceTarget, const list<vector<PointBorderData>>& lstAllInter, list<Face>& facesOut, list<Face>& facesIn);
        //static void GetInverse(list<vector<PointBorderData>>& lstAllInter);
        static void GetInverse(vector<vector<PointBorderData>>& lstAllInter);

        static PointInTriangleType InTriangle(const Vector3f& pt0, const Vector3f& pt1, const Vector3f& pt2, const Vector3f& ptTarget);
        static PointInPolygonResult IsPointInFace(const Vector3f& point, const Face& face, bool needBoxDetermine = true);

        static PointInPolygonResult IsPointInPolygon2(const Vector3f& point, const list<Line>& polygon);
        static PointInPolygonResult IsPointInPolygon(const Vector3f& point, const list<Line>& polygon, bool needBoxDetermine = true, double tolerance = 1e-6);
        static PointInPolygonResult IsPointInPolygon(const Vector3f& normal, const Vector3f& point, const LineBSPTree* bspTree);

        //  点与实体关系判断，0：点在实体面上；1：点在实体外；-1：点在实体内
        static int IsPointInBody2(const Vector3f& point, const Body& body);
        static bool IsPointInBody(const Vector3f& point, const Body& body);
        //  点与实体关系判断，0：点在实体面上；1：点在实体外；-1：点在实体内
        static int IsPointInBody(const Vector3f& point, const PlaneBSPTree* bspTree);

        //  轮廓线提取&拟合（三角网格→BRep）
        static void MeshToBRep(const vector<TriangleData>& tris, list<Face>& faces);
        static void UpdateOneBoder(int triIndex, unsigned int min01, unsigned int max01, unordered_map<unsigned int, vector<array<int, 2>>>& mapBorder2TriIndex);
        static void GetTriNormal(const TriangleData& tri, Vector3f& normal);
        static void GetTriNormal(const Vector3f& pt0, const Vector3f& pt1, const Vector3f& pt2, Vector3f& normal);
        static bool IsSameNormal(const TriangleData& first, const TriangleData& second);
        static void GetAnother2Border(const TriangleData& tri, int borderNIndexMax, int borderNIndexMin,
            vector<pair<pair<unsigned int, unsigned int>, array<unsigned int, 2>>>& boders);
        static void UpdatePolygon(map<pair<unsigned int, unsigned int>, array<unsigned int, 2>>& polygon,
            unsigned int borderIMax, unsigned int borderIMin, const TriangleData& tri);
        static void GetPolygons(vector<map<pair<unsigned int, unsigned int>, array<unsigned int, 2>>> polygons, const TriangleMeshWrapper* mesh, list<Face>& faces);
        static bool NeedChangeEndPoint(const Vector3f& preStart, const Vector3f& preEnd, const Vector3f& curStart, const Vector3f& curEnd);
        static void OperatePoly(list<Line>& poly);
        static void FormatPoly(vector<Vector3f>& poly);


        //  布尔运算：八叉树CSG
        static void BodyIntersect_CSGOctTree_TEST(int mode, const TriangleMesh& meshA, const TriangleMesh& meshB, int edgeType, TriangleMesh& boxesIn, TriangleMesh& boxesOut, TriangleMesh& boxesCut);
        static void BodyBoolean_CSGOctTree_TEST(const TriangleMesh& meshA, const TriangleMesh& meshB, TriangleMesh& inter, TriangleMesh& meshUnion, TriangleMesh& meshDiffAB, TriangleMesh& meshDiffBA);

        static void BodyIntersect_CSGOctTree(const TriangleMesh& meshA, const TriangleMesh& meshB, TriangleMesh& inter);
        static void BodyDiff_CSGOctTree(const TriangleMesh& meshA, const TriangleMesh& meshB, TriangleMesh& meshDiff);
        static void BodyUnion_CSGOctTree(const TriangleMesh& meshA, const TriangleMesh& meshB, TriangleMesh& meshUnion);

        static void MeshCutMesh_CSGOCT(const TriangleMesh& meshA, const TriangleMesh& meshB, list<Face>& partsOutA_of_B, list<Face>& partsInA_of_B, list<Face>& partsOutB_of_A, list<Face>& partsInB_of_A);
        static void ConvertToOctTree(const TriangleMesh& meshA, const Vector3f& minBox, const Vector3f& maxBox, OctTree* octTree);
        static void OctTreeNodeCutNode(OctTree* nodeSource, OctTree* nodeTarget, list<Face>& partsOut, list<Face>& partsIn);
        static void FacesCutBody(const vector<Face>& faces, Body& body, list<Body>& partsOut, list<Body>& partsIn, vector<int>& faceFlag);
        static bool FacesCutBody2(const vector<Face>& faces, Body& body, list<Face>& facesIn, vector<int>& faceFlag);

        static void FormCuttedOctTreeNode(OctTree* rootNode, OctTree* octTree, vector<Face*>& partsOut, vector<Face*>& partsIn, map<OctTree*, bool>& mapNeedModifyNode);
        static void FormCuttedOctTreeNode(OctTree* octTree, list<Face>& partsOut, list<Face>& partsIn);

        static bool IsPolygonValid(const list<Line>& polygon);

        //  布尔运算：三角面
        static void BodyIntersect(const TriangleMesh& meshA, const TriangleMesh& meshB, TriangleMesh& inter, bool forceOperate);
        static void BodyUnion(const TriangleMesh& meshA, const TriangleMesh& meshB, TriangleMesh& meshUnion, bool forceOperate);
        static void BodyDifference(const TriangleMesh& meshA, const TriangleMesh& meshB, TriangleMesh& meshDiff, bool forceOperate);

        static void TestForTriBoolean(const TriangleMesh& meshA, const TriangleMesh& meshB, TriangleMesh& meshCutOut, TriangleMesh& meshCutIn, bool forceOperate);
        static void TestForTriBoolean_TEST(const TriangleMesh& meshA, const TriangleMesh& meshB, vector<TriangleMesh>& meshCutOut, bool forceOperate);

        static bool GenerateBSPTree(const list<Line>& polygon, const Vector3f& normal, LineBSPTree* bspTree);
        static bool ConstructBSPTree(const list<LineInfo>& lines, const Vector3f& normal, LineBSPTree* bspTree);
        static void ReleaseBSPTree(PlaneBSPTree* tree);
        static void ReleaseBSPTree(LineBSPTree* tree);

        static bool ConstructBSPTree(const vector<TriangleData>& tris, PlaneBSPTree* bspTree);
        static void SearchTrisFromBSPTree(const vector<TriangleData>& tris, const PlaneBSPTree* bspTree, bool forceOperate, vector<TriangleData>& cuttedTris, vector<TriangleData>& trisIn, vector<TriangleData>& trisOut);
        static void MeshCutMesh(const TriangleMesh& meshA, const TriangleMesh& meshB, vector<TriangleData>& dataAinB, vector<TriangleData>& dataAoutB, vector<TriangleData>& dataBinA,
            vector<TriangleData>& dataBoutA, map<int, vector<TriangleData>::const_iterator>& mapCoincidentOfB, bool forceOperate, bool mergeOperate = false);
        static void MergeFinelyTris(vector<TriangleData>& tris);
        static void OperateBSPTree(const TriangleMesh& mesh, PlaneBSPTree* bspTree);

        static void TestCLI(map<int, Line>& mapTest);
        static void ExtractPolygon(const list<Line>& lines, const Vector3f& normal, list<list<Line>>& polygons, map<int, map<int, double>>& mapPolyIndex2NearIndex);
        static void ExtractPolygonEdgeMidPt(const Face& faceTarget, const list<list<Line>>& polygons, map<PointData_1e_3, pair<int, int>>& mapMidPt2PolyAndEdge, map<int, list<list<Line>>::const_iterator>& mapIndex2Itr,
            vector<vector<pair<int, int>>>& cuttedNodePolygonsBoderFlags);
        static void ExtractPolygonEdgeMidPt(const Line& edgeSource, const list<list<Line>>& polygons, map<PointData_1e_3, pair<int, int>>& mapMidPt2PolyAndEdge, map<int, list<list<Line>>::const_iterator>& mapIndex2Itr);
        static void SplitEdges(const list<Line>& lines, list<Line>& splitedEdges);
        static void ConvertToPolygonTree(const list<list<Line>>& polygons, PolygonTree* polygonTree);
        static void ConvertToPolygonTree(list<list<list<Line>>::const_iterator>& polygons, PolygonTree* polygonTree);
        static void ReleasePolygonTree(PolygonTree* tree);

        static void PushBackToMesh(const vector<TriangleData>& dataPart, const map<int, vector<TriangleData>::const_iterator>& mapCoincidentOfB, int startIndex, bool considUV, bool hasOriUV, bool needInver, map<int, int>& mapIndexOri2New, TriangleMesh& mesh);

        //  当一个规范的闭合三角网格几何体A 切割另一个不规范的三角网格几何体B 时，根据切割面切开的三角面（out & in），根据切开的三角面去搜索得到AinB 和 AoutB；
        static void GetCutPartOfMesh(const vector<TriangleData>& tris, const vector<TriangleData>& trisCutted, vector<TriangleData>& resultPart);

        static void SearchFromBSTree(const TriangleData& tri, const PlaneBSPTree* bspTree, vector<TriangleData>& trisIn, vector<TriangleData>& trisOut);
        static void GetTriDataFromMesh(const TriangleMesh& mesh, vector<TriangleData>& tris);

        //  布尔运算：Brep
        static void BodyIntersect(const Body& body1, const Body& body2, vector<Face>& facesInter);
        static void BodyUnion(const Body& body1, const Body& body2, vector<Face>& facesUnion);
        static void BodyDifference(const Body& body1, const Body& body2, vector<Face>& facesResult);

        static void TransformPolygon(list<Line>& polygon, const Transform& trs);
        static void TransformPolygons(list<list<Line>>& polygons, const Transform& trs);
        static void TransformFace(Face& face, const Transform& trs);
        static void TransformBody(Body& body, const Transform& trs);
        static void TessellateBody(const Body& body, TriangleMesh& mesh);
        static void TesslateFace(const Face& face, TriangleMesh& mesh);
        static void TesslateFace(const Face& face, vector<Vector3f>& lstPoint, vector<unsigned>& lstTri, vector<unsigned>& edges, vector<Vector3f>& normals, vector<Vector2f>& uvs);
        static void TessellatePolygon(const vector<Vector3f>& lstPoint, const Vector3f& normal, TriangleMesh& mesh);
        static void TessellatePolygonTree(const PolygonTree* polygonTree, const Vector3f& normal, list<Vector3f>& lstPoints, list<unsigned int>& lstTris, list<unsigned int>& lstEdge, list<Vector3f>& lstNormal, list<Vector2f>& lstUv);
        static void TessellatePolygonTree(const PolygonTree* polygonTree, const Vector3f& normal, TriangleMesh& mesh);
        static void ConstructTriangleMesh(const vector<Vector3f>& vertexes, const vector<unsigned int>& triangles, const vector<unsigned int>& edges,
            const vector<Vector3f>& normals, const vector<Vector2f>& uvs, TriangleMesh& mesh);
        static void ConstructTriangleMesh(const list<Vector3f>& vertexes, const list<unsigned int>& triangles, const list<unsigned int>& edges,
            const list<Vector3f>& normals, const list<Vector2f>& uvs, TriangleMesh& mesh);

        static void FaceCutFace(const Face& faceTarget, const Vector3f& normalTarget, const Face& faceSource, const Vector3f& normalSource,
            vector<PointBorderData>& lstTemp);
        ///*cutted：是否切割了，当切割了，结果是无效切割时用到，比如认为是faceIn*/
        static bool BodyCutFace(const Body& body, const Face& faceTarget, list<Face>& facesOut, list<Face>& facesIn, bool& cutted, bool& irregular);
        //static bool BodyCutFaces(const Body& body, const vector<Face>& facesTarget, list<Face>& facesOut, list<Face>& facesIn);
        template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
        static bool BodyCutFaces(const Body& body, bool bodyClose, _Iter first_facesInBody, _Iter last_facesInBody, _Iter first, _Iter last, list<Face>& facesOut, list<Face>& facesIn, bool& irregular);

        static void BodyCutBody(const Body& body1, const Body& body2, list<Face>& facesOut2_Of1, list<Face>& facesIn2_Of1,
            list<Face>& facesOut1_Of2, list<Face>& facesIn1_Of2, map<int, list<Face>::const_iterator>& mapIndex2MarkedFacesIn2_Of1, map<int, list<Face>::const_iterator>& mapIndex2MarkedFacesIn1_Of2);
        static void BodyCutBody(const Body& bodySource, const Body& bodyTarget, list<Face>& facesOut, list<Face>& facesIn);
        static bool IsFaceCutted(const Body& body, const Face& faceTarget);
        static void GetInverseFace(const Face& source, Face& result);
        static void GetInverse(const list<Line>& source, list<Line>& result);
        static void GetInverse(const vector<Line>& source, vector<Line>& result);

        static void FacesCutFace_Simple(const list<pair<Face, Vector3f>>& facesSource, const Face& faceTarget, const Vector3f& normalTarget, list<Face>& facesOut, list<Face>& facesIn);
        static void FacesCutFace_Simple_TEST(const list<pair<Face, Vector3f>>& facesSource, const Face& faceTarget, const Vector3f& normalTarget, list<Face>& facesOut, list<Face>& facesPathNotClose);

        static void TrianglesCutTriangle_Simple(const vector<TriangleData>& trisSource, const TriangleData& triTarget, list<TriangleData>& trisOut, list<TriangleData>& trisIn);
        static void TrianglesCutTriangle_Simple_TEST(const vector<TriangleData>& trisSource, const TriangleData& triTarget, list<TriangleData>& trisOut, list<TriangleData>& trisPath);

        static void ConvertTri2Face(const TriangleData& tri, Face& triFace);
        static void ConvertTriFace2Tri(const Face& triFace, const Vector3f& normal, TriangleData& tri);
        static void GetTriDataFromFaces(const list<Face>& facesOut, const TriangleData& triTarget, const Vector3f& normalTarget, list<TriangleData>& tris);


        //  耳切法剖分
        static void SortPolygon(const Vector3f& axisX, list<pair<int, vector<vector<Vector3f>>::const_iterator>>& innerPolygons);

        static void MakeSimple(const vector<Vector3f>& outerPolygon, const Vector3f& normal, const vector<vector<Vector3f>>& innerPolygons,
            vector<Vector3f>& lstPoint, map<int, PointIndexFlagData, DescendingCompareInt>& mapNew2Ori, vector<int>& lstIsOuter);
        static void MakeSimple(const vector<Vector3f>& inner, int indexInner, int ptOnInner, int ptVisible, vector<Vector3f>& outer, vector<int>& lstIsOuter);
        static int GetVisibleVertex(const Vector3f& ptOnInner, const PointBorderData& ptInter, const Vector3f& axisX, const vector<Vector3f>& outerPolygon, bool selectLittleAng = true);
        static int GetMaxValuePoint(const vector<Vector3f>& polygon, const Vector3f& axisX, float& maxX);

        static void GetPointsFromFace(const Face& face, const Vector3f& normal,
            vector<Vector3f>& lstPoint, map<int, PointIndexFlagData, DescendingCompareInt>& mapNew2Ori, vector<int>& lstIsOuter);
        static void GetFaceAxisX(const Vector3f& normal, Vector3f& axisX);
        static void GetTriangulationData(const vector<Vector3f>& lstPoint, const Vector3f& normal, vector<int>& lstTris);
        static bool HasPointInTriangle(int index0, int index1, int index2, const vector<Vector3f>& lstPoint, const vector<int>& lstConvexVert);
        static bool HasPointInTriangle(const Vector3f& pt0, const Vector3f& pt1, const Vector3f& pt2, const list<Vector3f>& lstPoint);

        template <class _Iter, enable_if_t<_Is_iterator_v<_Iter>, int> = 0>
        static bool HasPointInPolygon(const list<Line>& polygon, const Vector3f& normal, _Iter firstPt, _Iter lastPt/*const list<Vector3f>& lstPoint*/);

        static void GetVectorList(const vector<Vector3f>& lstPoint, vector<Vector3f>& lstVec);
        //static void GetVectorList(const vector<Vector3d>& lstPoint, vector<Vector3d>& lstVector);
        static void GetInitArray(const vector<Vector3f>& lstPoint, const Vector3f& normal, vector<int>& lstConvexVert);
        static int IsEarVertex(const vector<Vector3f>& lstPoint, const Vector3f& normal, int nPre, int nCur, int nNext, const vector<int>& lstConvexVert);
        static bool GetPreAndNext(int nCur, const vector<int>& lstVertIdent, int& nPre, int& nNext);
        static void UpdateNearVertIdent(int nPoint, const vector<Vector3f>& lstPoint, const Vector3f& normal, vector<int>& lstConvexVert);
        static bool GetEarVert(const vector<int>& lstConvexVert, int nStart, int& ear);

        //  凸包
        static bool CalConvexHull(const list<Vector3f>& lstVertex, const Vector3f& normal, list<Line>& convexHull);
        static bool CalMinHull(const list<Vector3f>& lstVertex, const Vector3f& normal, list<Line>& hull);

        static bool Cal3DConvexHull(const list<Vector3f>& lstVertex, Body& convexHull);
        static bool Cal3DMinHull(const list<Vector3f>& lstVertex, const Body& convexBTest, Body& hull);

        //  凸分解
        static void ConvexDecompositionPolygonTree(const PolygonTree* polygonTree, const Vector3f& normal, list<list<Line>>& lstConvexPolygon);

        static void ConvexDecomposition(const list<Line>& polygon, const Vector3f& normal, list<list<Line>>& lstConvexPoly);
        static void ConvexDecomposition(const Face& face, const Vector3f& normal, list<list<Line>>& lstConvexPoly);

        //  基础工具函数
        static void getBoundingBox(const vector<pair<Vector3f, Vector3f>>& mapMeshId2BoundBox, Vector3f& boundingBoxMin, Vector3f& boundingBoxMax);
        static void getBoundingBox(const list<Vector3f>& lstVertex, Vector3f& boundingBoxMin, Vector3f& boundingBoxMax);
        static void getBoundingBox(const list<Line>& lstLine, Vector3f& boundingBoxMin, Vector3f& boundingBoxMax);
        static void getBoundingBox(const Body& body, Vector3f& boundingBoxMin, Vector3f& boundingBoxMax);
        static void getBoundingBox(const TriangleMesh* mesh, const Transform* trs, Vector3f& boundingBoxMin, Vector3f& boundingBoxMax);
        static void getBoundingBox_IdentTrs(const TriangleMesh* mesh, Vector3f& boundingBoxMin, Vector3f& boundingBoxMax);
        static void getBoundingBox_NotIdentTrs(const TriangleMesh* mesh, const Transform* trs, Vector3f& boundingBoxMin, Vector3f& boundingBoxMax);

        static bool IsRayHitSphere(const Sphere& sphere, const Vector3f& start, const Vector3f& pntOn, vector<Vector3f>& pntsHit);
        static bool IsRayHitFace(const Face& face, const Vector3f& faceNormal, const Vector3f& ptOri, const Vector3f& dir, double& t);
        static bool IsRayHitPlane(const Vector3f& ptOnPlane, const Vector3f& planeNormal, const Vector3f& ptOri, const Vector3f& dir, double& t);

        static int GetNearBorderIndex(int outerEdgeCount, const list<int>& lstInnerEdgeCount, int currentIndex, bool reqNext);
        static int GetNearBorderIndex(const Face& faceTarget, int currentIndex, bool reqNext);

        static void mergePoints(const list<Vector3f>& points, list<Vector3f>& result);

        static void GetPolygon(const vector<Vector3f>& lstVertex, list<Line>& polygon);
        static bool IntersectTriangle(const Vector3f& orig, const Vector3f& dir,
            const Vector3f& v0, const Vector3f& v1, const Vector3f& v2, double* t);

        //  rayDir须为单位向量
        static bool HitBoundingBox(const Vector3f& rayStart, const Vector3f& rayDir, const Vector3f* boundBoxMin, const Vector3f* boundBoxMax, Vector3f& interPt0, Vector3f& interPt1);
        static bool IsSameDirection(const Vector3f& dirLeft, const Vector3f& dirRight, double tolerance = 1e-5);

        static void getTransformL2W(const Vector3f& normal, Transform& trsL2W);
        static void getTransformW2L(const Vector3f& normal, Transform& trsW2L);
        static void getLocalXY(const Vector3f& normal, Vector3f& localX, Vector3f& localY);

        static bool isPointInBox(const Vector3f& point, const Vector3f& boundingBoxMin, const Vector3f& boundingBoxMax);

        //  TEST TOOL
        static void readPoints(const char* filename, list<Vector3f>& points, double scale = 1.0);
        static bool readPoint(const string& readline, Vector3f& result);
        static void readLine(ifstream* ReadFile, Line& line);

        static bool isZDirectorCrossed(const Sphere* sphere, const Vector3f& boundingBoxMin, const Vector3f& boundingBoxMax);
        static bool isStrideXAxis(const Sphere* sphere, const Vector3f& boundingBoxMin, const Vector3f& boundingBoxMax);

        static bool isPointInBoxRange(const Vector3f& point, const Vector3f& boundingBoxMin, const Vector3f& boundingBoxMax, double* minDis);

    };
}

#endif
