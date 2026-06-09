#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "CGLibDefine.h"

class CGLIB_EXPORTS Quaternion
{
public:
    double X;
    double Y;
    double Z;
    double W;

    Quaternion();
    Quaternion(double x, double y, double z, double w);
    Quaternion(const Quaternion& other);
    ~Quaternion();

};

#endif
