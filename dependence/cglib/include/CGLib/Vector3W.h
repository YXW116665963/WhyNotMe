#pragma once
#include "Vector3.h"
#include <cmath>
#include "CGUtils/MathUtils.h"

namespace CGUTILS
{
	/// <summary>
	/// Represents four-dimension location/vector/offset
	/// </summary>
	class CGLIB_EXPORTS Vector3W
	{
	public:

		 Vector3W()
		 {
			  m_xyzw[0] = 0;
			  m_xyzw[1] = 0;
			  m_xyzw[2] = 0;
			  m_xyzw[3] = 0;
		 }

		 Vector3W(const Vector3W& vec)
		 {
			  m_xyzw[0] = vec.WX();
			  m_xyzw[1] = vec.WY();
			  m_xyzw[2] = vec.WZ();
			  m_xyzw[3] = vec.W();
		 }

		 Vector3W(Vector3d xyz, double w)
		 {
			  m_xyzw[0] = xyz.X * w;
			  m_xyzw[1] = xyz.Y * w;
			  m_xyzw[2] = xyz.Z * w;
			  m_xyzw[3] = w;
		 }
		 Vector3W(double wx, double wy, double wz, double w)
		 {
			  m_xyzw[0] = wx;
			  m_xyzw[1] = wy;
			  m_xyzw[2] = wz;
			  m_xyzw[3] = w;
		 }

	public:

		 double GetWX() const
		 {
			  return m_xyzw[0];
		 }
		 double GetWY() const
		 {
			  return m_xyzw[1];
		 }
		 double GetWZ() const
		 {
			  return m_xyzw[2];
		 }

		 void SetW(const double w)
		 {
			  Vector3d origin = this->ToXYZ(true);
			  *this = Vector3W(origin, w);
		 }
		 double GetW() const
		 {
			  return m_xyzw[3];
		 }

		 double WX() const
		 {
			  return m_xyzw[0];
		 }
		 double& WX()
		 {
			  return m_xyzw[0];
		 }
		 double WY() const
		 {
			  return m_xyzw[1];
		 }
		 double& WY()
		 {
			  return m_xyzw[1];
		 }
		 double WZ() const
		 {
			  return m_xyzw[2];
		 }
		 double& WZ()
		 {
			  return m_xyzw[2];
		 }
		 double W() const
		 {
			  return m_xyzw[3];
		 }
		 double& W()
		 {
			  return m_xyzw[3];
		 }

	public:

		 Vector3d ToXYZ(bool divideWeight)const
		 {
			  if (divideWeight)
			  {
					double w = m_xyzw[3];
					if (MathUtils::IsAlmostEqualTo(w, 0.0))
					{
						 return Vector3d(m_xyzw[0], m_xyzw[1], m_xyzw[2]);
					}
					else
					{
						 return Vector3d(m_xyzw[0] / w, m_xyzw[1] / w, m_xyzw[2] / w);
					}
			  }
			  else
			  {
					return Vector3d(m_xyzw[0], m_xyzw[1], m_xyzw[2]);
			  }
		 }
		 bool IsAlmostEqualTo(const Vector3W& another) const
		 {
			  Vector3W self = *this;
			  Vector3W temp = another;
			  return  self.ToXYZ(true).IsEqual(temp.ToXYZ(true));
		 }
		 double Distance(const Vector3W& another) const
		 {
			  double squareValue = std::pow((another.GetWX() - m_xyzw[0]), 2) + std::pow((another.GetWY() - m_xyzw[1]), 2) + std::pow((another.GetWZ() - m_xyzw[2]), 2) + std::pow((another.GetW() - m_xyzw[3]), 2);
			  return std::sqrt(squareValue);
		 }

	public:

		 double& operator[](int index)
		 {
			  return m_xyzw[index];
		 }
		 const double& operator[](int index) const
		 {
			  return m_xyzw[index];
		 }
		 Vector3W& operator =(const Vector3W& source)
		 {
			  if (this != &source)
			  {
					m_xyzw[0] = source.WX();
					m_xyzw[1] = source.WY();
					m_xyzw[2] = source.WZ();
					m_xyzw[3] = source.W();
			  }

			  return *this;
		 }
		 Vector3W  operator +(const Vector3W& xyzw) const
		 {
			  return Vector3W(m_xyzw[0] + xyzw.m_xyzw[0], m_xyzw[1] + xyzw.m_xyzw[1], m_xyzw[2] + xyzw.m_xyzw[2], m_xyzw[3] + xyzw.m_xyzw[3]);
		 }
		 Vector3W  operator -(const Vector3W& xyzw) const
		 {
			  return Vector3W(m_xyzw[0] - xyzw.m_xyzw[0], m_xyzw[1] - xyzw.m_xyzw[1], m_xyzw[2] - xyzw.m_xyzw[2], m_xyzw[3] - xyzw.m_xyzw[3]);
		 }
		 Vector3W& operator +=(const Vector3W& xyzw)
		 {
			  m_xyzw[0] += xyzw.m_xyzw[0];
			  m_xyzw[1] += xyzw.m_xyzw[1];
			  m_xyzw[2] += xyzw.m_xyzw[2];
			  m_xyzw[3] += xyzw.m_xyzw[3];
			  return *this;
		 }

	private:

		double m_xyzw[4];
	};

	CGLIB_EXPORTS Vector3W operator *(const Vector3W& source, const double d);
	CGLIB_EXPORTS Vector3W operator *(const double& d, const Vector3W& source);
	CGLIB_EXPORTS Vector3W operator /(const Vector3W& source, double d);

}

