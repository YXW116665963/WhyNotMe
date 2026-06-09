#ifndef __Plane_H__
#define __Plane_H__

#include "Vector3.h"

namespace CGUTILS
{
    class CGLIB_EXPORTS Plane
    {
    public:
        Plane(const Vector3d& center, const Vector3d& normal);

    public:
        bool IsSegmentIntersect(const Vector3d& start, const Vector3d& end, Vector3d& pntIntersect) const;

    private:
        Plane(const Plane& other);

    public:
        Vector3d mCenter;
        Vector3d mNormal;

    };
}
#endif
