#pragma once

#include <array>
#include "CGLibDefine.h"
#include "Vector3W.h"

namespace CGUTILS
{
    class CGLIB_EXPORTS Transform
    {
    public:
        static Transform* CreateIdentity();
        static Transform* Create(const float* trans);//length is 12，column-major(three basic vector and translate)
        static Transform* Create(const float* mat3x3, const Vector3f& translate);//length of mat3x3 is 9
        static void Destroy(Transform* trs);

        Transform();
        Transform(const Transform& other);
        Transform(Transform&& other) noexcept;
        Transform(const float* trans);
        Transform(const Vector3d& basisX, const Vector3d& basisY, const Vector3d& basisZ, const Vector3d& basisW);
        Transform(const float* mat3x3, const Vector3f& translate);//length of mat3x3 is 9
        Transform(const Vector3d& vecAxis, double regAngle);//from axis and roate radian
        Transform(const Vector3f& center, const Vector3d& vecAxis, double regAngle);
        ~Transform();

    public:
        void SetValue(const float* mat3x3, const Vector3f& translate);

        void SetValue(const float* value);//length of value is 12
        float* GetValue();//length of result is 12
        const float* GetValue() const;

        void SetMat3x3(const float* mat3x3);
        const float* GetMat3x3() const;//length of result is 9

        void SetTranslate(const Vector3f& translate);
        Vector3f& GetTranslate();
        const Vector3f& GetTranslate() const;

        bool IsIdentity() const;
        bool IsEqual(const Transform& right) const;

        Transform operator*(const Transform& other) const;

        static void MultPoint(const Transform& trans, const Vector3f& origin, Vector3f& result);
        static void MultVector(const Transform& trans, const Vector3f& origin, Vector3f& result);
        static void MultWeightedPoint(const Transform& trans, const Vector3W& point, Vector3W& result);
        static void Mult(const Transform& left, const Transform& right, Transform& result);
        static void CalInverse(const Transform& origin, Transform& result);

    private:
        void rotate(const Vector3d& vecAxis, double regAngle);

        //length is 12，column-major(three basic vector and translate)
        std::array<float, 12> mValue;
    };
}