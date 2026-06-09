#pragma once

#include <vector>
#include "CGLibDefine.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Transform.h"

namespace CGUTILS
{
    class TriangleMeshImpl;
    class CGLIB_EXPORTS TriangleMesh
    {
    public:
        static TriangleMesh* Create();
        static TriangleMesh* Create(const TriangleMesh& other);
        static void Destroy(TriangleMesh* data);

    public:
        TriangleMesh();
        TriangleMesh(const TriangleMesh& other);
        ~TriangleMesh();

    public:
        const char* Id() const;
        void SetId(const char* id);

        int GetVertexCount() const;
        const float* GetVertexes() const;
        const Vector3f& GetVertex(int index) const;
        void SetVertexes(int vertexCount, const float* vertex);
        void AddVertex(const Vector3f& vertex);

        int GetNormalCount() const;
        const float* GetNormals() const;
        const Vector3f& GetNormal(int index) const;
        void SetNormals(int normalCount, const float* normal);
        void AddNormal(const Vector3f& normal);

        int GetUVCount() const;
        const float* GetUVs() const;
        const Vector2f& GetUV(int index) const;
        void SetUVs(int uvCount, const float* uv);
        void AddUV(const Vector2f& uv);

        int GetFacetCount() const;
        const unsigned int* GetFacets() const;
        const Vector3ui& GetFacet(int index) const;
        void SetFacets(int facetCount, const unsigned int* facet);
        void AddFacet(const Vector3ui& facet);

        int GetEdgeCount() const;
        const unsigned int* GetEdges() const;
        const Vector2ui& GetEdge(int index) const;
        void SetEdges(int edgeCount, const unsigned int* edge);
        void AddEdge(const Vector2ui& edge);

        void GetBoundingBox(Vector3f& boxMin, Vector3f& boxMax) const;

        void Transform(const Transform& trs);

    private:
        TriangleMeshImpl* mImpl;
    };

}
