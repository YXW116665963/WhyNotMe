#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <math.h>
#include <random>
#include "CGLibDefine.h"

namespace CGUTILS
{
    template <typename T>
    class CGLIB_EXPORTS Vector3
    {
    public:
        static Vector3 BasicX;
        static Vector3 BasicY;
        static Vector3 BasicZ;
        static Vector3 Zero;

    public:
        T X;
        T Y;
        T Z;

        Vector3();
        Vector3(T x, T y, T z);

        template<typename VT>
        Vector3(const Vector3<VT>& other);

        ~Vector3();

    public:
        void Normalize();
        void SetValue(T dX, T dY, T dZ);

        Vector3 CrossProduct(const Vector3& right) const;
        double DotProduct(const Vector3& right) const;

        void Added(const Vector3& right);
        double Length() const;
        double SqrLength() const;
        double Distance(const Vector3& point) const;

        bool IsParalell(const Vector3& right, double tolerance = 1e-5) const;
        bool IsEqual(const Vector3& right, double tolerance = 1e-5) const;

        double Angle(const Vector3& right) const;
        double SinAngle(const Vector3& right) const;
        double AngleOnPlaneTo(const Vector3& vector2, bool upAxis = true) const;
        double AngleOnPlaneTo(const Vector3<T>& vector3, const Vector3<T>& planeNormal) const;

        Vector3 operator/(double k) const;
        Vector3 operator+(const Vector3& right) const;
        Vector3 operator-(const Vector3& right) const;
        Vector3 operator-() const;
        //Vector3 operator*(float value) const;
        Vector3 operator*(double value) const;
        void operator+= (const Vector3& right);
        void operator*= (float right);

        T& operator[](int index);
        const T& operator[](int index) const;

        static Vector3 CreateRandomOrthogonal(const Vector3& value);
    };

    template <typename T>
    Vector3<T> Vector3<T>::BasicX = Vector3(1.0, 0.0, 0.0);
    template <typename T>
    Vector3<T> Vector3<T>::BasicY = Vector3(0.0, 1.0, 0.0);
    template <typename T>
    Vector3<T> Vector3<T>::BasicZ = Vector3(0.0, 0.0, 1.0);
    template <typename T>
    Vector3<T> Vector3<T>::Zero = Vector3(0.0, 0.0, 0.0);

    template<typename T>
    template<typename VT>
    inline Vector3<T>::Vector3(const Vector3<VT>& other)
        : X(other.X), Y(other.Y), Z(other.Z) {
    }

    template <typename T>
    Vector3<T> operator *(const double& d, const Vector3<T>& source)
    {
        return Vector3<T>(source.X * d, source.Y * d, source.Z * d);
    }

    typedef Vector3<float>             Vector3f;
    typedef Vector3<double>            Vector3d;
    typedef Vector3<int>               Vector3i;
    typedef Vector3<unsigned int>      Vector3ui;
}

#endif
