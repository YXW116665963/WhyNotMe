#ifndef __FACE_H__
#define __FACE_H__

#include "Loop.h"
namespace CGUTILS
{
    class CGLIB_EXPORTS Face
    {
    public:
        Face() : flag(-1) {}

        Loop outerLoop;
        vector<Loop> innerLoop;
        int flag;  //根据需要赋予意义并使用的成员

        //0:node切出来的边;  1:原始面的自然边;  2:cutFace且出来的边;
        vector<pair<int, int>> boderFlags;  //标记，便于csgOct布尔运算时共面延续处理 pair<标记,切割面>
        map<int, list<Line>> mapIndex2UnFalgedBor;  //共面延续处理过程中未被传递标记的边（可能标记了边的一部分）
    };
}
#endif
