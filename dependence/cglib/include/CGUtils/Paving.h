#pragma once
#include <list>
#include <map>
#include <vector>
#include "BooleanUtils.h"

using namespace std;

namespace CGUTILS
{
    class CGLIB_EXPORTS Paving
    {
    public:
        static void Run(const list<list<Line>>& polygon, double tileLen, double tileWid, double rotateAng, const Vector3f& start, list<list<Line>>& pavingTiles);
        static void PavingPolygonTree(const PolygonTree* polygonTree, const Vector3f& normal, const list<list<Line>>& tileSet, list<list<Line>>& pavingTiles);

    };


}