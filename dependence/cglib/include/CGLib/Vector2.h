#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include "CGLibDefine.h"

namespace CGUTILS
{
    template <typename T>
    class CGLIB_EXPORTS Vector2
    {
    public:
        static Vector2 Zero;

    public:
        T U;
        T V;

        Vector2();
        Vector2(T u, T v);
        template<typename VT> Vector2(const Vector2<VT>& other);
        ~Vector2();

    public:
        void SetValue(T dU, T dV);

        void Normalize();
        double DotProduct(const Vector2& right) const;
        double Length() const;

        bool IsParalell(const Vector2& right, double tolerance = 1e-5) const;
        bool IsEqual(const Vector2& right, double tolerance = 1e-5) const;

        double Angle(const Vector2& right) const;

        Vector2 operator/(double k) const;
        Vector2 operator+(const Vector2& right) const;
        Vector2 operator-(const Vector2& right) const;
        Vector2 operator-() const;
        Vector2 operator*(double value) const;

        T& operator[](int index);
        const T& operator[](int index) const;
    };

    template <typename T>
    Vector2<T> Vector2<T>::Zero = Vector2(0.0, 0.0);

    template <typename T>
    template <typename VT>
    Vector2<T>::Vector2(const Vector2<VT>& other)
        : U(other.U), V(other.V)
    {
    }

    template <typename T>
    Vector2<T> operator *(const double& d, const Vector2<T>& source)
    {
        return Vector2<T>(source.U * d, source.V * d);
    }

    typedef Vector2<float>           Vector2f;
    typedef Vector2<double>          Vector2d;
    typedef Vector2<int>             Vector2i;
    typedef Vector2<unsigned int>    Vector2ui;
}

    
#endif
