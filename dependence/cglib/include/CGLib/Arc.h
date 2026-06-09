#pragma once
#include "CGLib/Vector3.h"
#include "CGLib/Transform.h"
#include "CGLib/Line.h"
#include "CGUtils/CurveTool.h"

namespace CGUTILS
{
    class CGLIB_EXPORTS ProjectResult
    {
    public:
        double Distance;
        Vector3d Point;
        bool InLineRange;

        ProjectResult(double dis, const Vector3d& proPt, bool inLineRange)
            :Distance(dis), Point(proPt), InLineRange(inLineRange)
        {
        }
    };

    class CGLIB_EXPORTS Arc2d
    {
    public:
        Vector3d Center;
        double Radius;
        double AngleStart;  //角度范围[0-2PI]
        double AngleEnd;  //角度范围[0-2PI]
        bool ClockWise;
        bool Large;
        bool Whole;  //只读

    public:
        Arc2d(const Vector3d& center, double radius, double angStart, double angEnd, bool clockWise);
        Arc2d(const Vector3d& center, const Vector3d& ptStart, double dSweepAng);
        Arc2d(const Vector3d& point0, const Vector3d& point1, const Vector3d& pointOnArc);
        Arc2d(const Arc2d& arcOther);

        double Length() const;
        Vector3d Point0() const;
        Vector3d Point1() const;

        Vector3d GetEndPoint(int index) const;
        Vector3d GetCurvePoint(double para) const;

        ProjectResult Project(const Vector3d& point) const;

        /// <summary>
        /// 获取扫略角，为正时代表顺时针，为负时代表逆时针
        /// </summary>
        double GetArcSweepAngle(double& dAngStart) const;

    private:
        bool IsInArcRange(double ang) const;

        bool IsOnCurve(const Vector3d& point) const;

        static bool GetArcInfor(const Vector3d& point0, const Vector3d& point1, const Vector3d& pointOnArc,
            Vector3d& origin, bool& bLarge, bool& bClockWise, double& radius);

        //  废弃，这种实现误差比较大
        static bool GetArcInfor_old(const Vector3d& point0, const Vector3d& point1, const Vector3d& pointOnArc,
            Vector3d& origin, bool& bLarge, bool& bClockWise, double& radius);

        Arc2d CreateTranslate(const Transform& matrix) const;

        //  TODO 后续考虑 是放在基础工具里还是类里
        /*public List<IntersectResult> Intersect(ICurve curve)
        {
            return CurveTool::Intesect(this, curve);
        }*/

        /*public Arc2d Clone()
        {
            return Arc2d(*this);
        }*/

        Arc2d CreateReversed() const;

        /*public EditPointType EditPoint(Vector3d point)
        {
            if (MathTool.MathTool.RealEq((point - Point0).Length(), 2))
                return EditPointType.Node0;
            else if (MathTool.MathTool.RealEq((point - Point1).Length(), 2))
                return EditPointType.Node1;
            else if (MathTool.MathTool.RealEq((point - GetCurvePoint(0.5)).Length(), 2))
                return EditPointType.Middle;
            else if (MathTool.MathTool.RealEq((point - Center).Length(), 2))
                return EditPointType.Center;

            return EditPointType.Unknown;
        }*/

        /*public bool IsWholeInBox(Vector3d vertex1, Vector3d vertex2)
        {
            if (Whole)
            {
                return CurveTool::IsPointInBox(GetCurvePoint(0.0), vertex1, vertex2) &&
                    CurveTool::IsPointInBox(GetCurvePoint(0.25), vertex1, vertex2) &&
                    CurveTool::IsPointInBox(GetCurvePoint(0.5), vertex1, vertex2) &&
                    CurveTool::IsPointInBox(GetCurvePoint(0.75), vertex1, vertex2);
            }
            else
            {
                return CurveTool::IsPointInBox(Point0, vertex1, vertex2) &&
                    CurveTool::IsPointInBox(Point1, vertex1, vertex2) &&
                    CurveTool::IsPointInBox(GetCurvePoint(0.5), vertex1, vertex2);
            }
        }*/

        /*public bool IsPartInBox(Vector3d vertex1, Vector3d vertex2)
        {
            if (Whole)
            {
                return CurveTool::IsPointInBox(GetCurvePoint(0.0), vertex1, vertex2) ||
                    CurveTool::IsPointInBox(GetCurvePoint(0.25), vertex1, vertex2) ||
                    CurveTool::IsPointInBox(GetCurvePoint(0.5), vertex1, vertex2) ||
                    CurveTool::IsPointInBox(GetCurvePoint(0.75), vertex1, vertex2);
            }
            else
            {
                return CurveTool::IsPointInBox(Point0, vertex1, vertex2) ||
                    CurveTool::IsPointInBox(Point1, vertex1, vertex2) ||
                    CurveTool::IsPointInBox(GetCurvePoint(0.5), vertex1, vertex2);
            }
        }*/

        /*public Parameter GetParameter(Vector3d point)
        {
            throw new NotImplementedException();
        }*/

    };
}