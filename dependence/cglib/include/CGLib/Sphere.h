#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Vector3.h"
namespace CGUTILS
{
    class CGLIB_EXPORTS Sphere
    {
    public:
        Sphere();
        Sphere(const Sphere& other);

    public:
        Vector3f center;
        double radius;
    };

    inline Sphere::Sphere()
        :center(0.0, 0.0, 0.0), radius(0.0)
    {
    }

    inline Sphere::Sphere(const Sphere& other)
        : center(other.center), radius(other.radius)
    {
    }
}
#endif
