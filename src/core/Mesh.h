#ifndef MESH_H
#define MESH_H
#include "Object.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>

namespace xmls
{
    class Mesh : public Object
    {
    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

    public:
        Mesh(std::vector<float> vertices = {}, std::vector<unsigned int> indices = {});
        ~Mesh();

        void AddVertex(glm::vec3 vertex, int offset = -1);
        void AddVertex(float x, float y, float z, int offset = -1);
        void AddVertex(glm::vec2 vertex, int offset = -1);
        void AddVertex(float x, float y, int offset = -1);
        void AddVertex(float vertex, int offset = -1);
        void AddVertices(std::vector<float> vertices, int offset = -1);
        void AddIndex(unsigned int index, int offset = -1);
        void AddIndices(std::vector<unsigned int> indices, int offset = -1);
        void SetVertex(int index, glm::vec3 vertex);
        void SetVertex(int index, float x, float y, float z);
        void SetVertex(int index, glm::vec2 vertex);
        void SetVertex(int index, float x, float y);
        void SetVertex(int index, float vertex);
        void SetVertices(std::vector<float> vertices);
        void SetIndex(int index, unsigned int value);
        void SetIndices(int index, std::vector<unsigned int> indices);

        float *GetVertices(int &size);
        unsigned int *GetIndices(int &size);
    };

    Mesh *CreateBox(float width, float height, float depth);
    Mesh *CreatePlane(float width, float height);
    Mesh *CreateCylinder(float radius, float height, int sectorCount);
    Mesh *CreateCone(float radius, float height, int sectorCount);
}
#endif