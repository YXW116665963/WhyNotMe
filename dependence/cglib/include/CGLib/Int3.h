#ifndef __INT3_H__
#define __INT3_H__

#include "CGLibDefine.h"

class CGLIB_EXPORTS Int3
{
public:
    int X;
    int Y;
    int Z;

    Int3();
    Int3(int x, int y, int z);
    Int3(const Int3& other);
    ~Int3();
};

#endif
