#ifndef __LOOP_H__
#define __LOOP_H__

#include <list>
#include "Line.h"

using namespace std;

namespace CGUTILS
{
    class CGLIB_EXPORTS Loop
    {
    public:
        list<Line> lstLine;
    };
}
#endif
