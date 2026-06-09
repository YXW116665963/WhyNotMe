#ifndef __BOX3_H__
#define __BOX3_H__

#include "Vector3.h"
namespace CGUTILS
{
    class CGLIB_EXPORTS Box3
    {
    public:
        Vector3f min;
        Vector3f max;
    };
}
#endif
