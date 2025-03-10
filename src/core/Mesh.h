#ifndef MESH_H
#define MESH_H
#include "Object.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>

namespace parseShape
{
    class Mesh : public Object
    {
    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        int vertexCount, vertexSize;

    public:
        Mesh(std::vector<float> vertices = {}, std::vector<unsigned int> indices = {});
        Mesh(int vertexCount, int vertexSize);
        ~Mesh();

        void ChangeVertex(glm::vec3 value, int index, int offset);
        void ChangeVertex(glm::vec2 value, int index, int offset);
        void ChangeVertex(float value, int index, int offset);
        void SetIndices(std::vector<unsigned int> indices);
        void DoTransformations(unsigned int begin, unsigned int offset);
        void DoTransformations() override;

        int GetVertexCount() const;
        int GetVertexSize() const;

        float *GetVertices(int &size);
        unsigned int *GetIndices(int &size);
    };

    Mesh *CreateBox(float width, float height, float depth, int vertexSize = 3);
    Mesh *CreatePlane(float width, float height, int vertexSize = 3);
    Mesh *CreateCylinder(float radius, float height, int sectorCount, int vertexSize = 3);
    Mesh *CreateCone(float radius, float height, int sectorCount, int vertexSize = 3);
}
#endif