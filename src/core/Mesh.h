#ifndef MESH_H
#define MESH_H
#include "Object.h"
#include "Shader.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>

namespace parseShape
{
    // En: Mesh class is used to create a mesh object with vertices and indices. Inherits from Object class.
    // Tr: Mesh sınıfı, bir mesh nesnesi oluşturmak için vertex ve index'leri kullanır. Object sınıfından türetilmiştir.
    class Mesh : public Object
    {
    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        int vertexCount, vertexSize;

    public:
        Mesh(std::vector<float> vertices = {}, std::vector<unsigned int> indices = {});
        Mesh(int vertexCount, int vertexSize);
        Mesh(int vertexCount, Shader *shader);
        ~Mesh();

        // En: Adds vertices to the mesh.
        // Tr: Mesh'e vertex'ler ekler.
        void AddVertex(std::vector<float> vertex);
        // En: Adds vertices to the mesh.
        // Tr: Mesh'e vertex'ler ekler.
        void AddVertex(float *vertex, int size);
        // En: Adds indices to the mesh.
        // Tr: Mesh'e index'ler ekler.
        void AddIndices(std::vector<unsigned int> indices);
        // En: Adds indices to the mesh.
        // Tr: Mesh'e index'ler ekler.
        void AddIndices(unsigned int *indices, int size);
        // En: Adds a triangle to the mesh.
        // Tr: Mesh'e bir üçgen ekler.
        void AddIndices(unsigned int a, unsigned int b, unsigned int c);
        // En : Changes Vertex at index with value
        // Tr : index'teki vertex'i value ile değiştirir
        void ChangeVertex(glm::vec3 value, int index, int offset);
        // En : Changes Vertex at index with value
        // Tr : index'teki vertex'i value ile değiştirir
        void ChangeVertex(glm::vec2 value, int index, int offset);
        // En : Changes Vertex at index with value
        // Tr : index'teki vertex'i value ile değiştirir
        void ChangeVertex(float value, int index, int offset);
        // En: Sets indices for the mesh.
        // Tr: Mesh için indisleri ayarlar.
        void SetIndices(std::vector<unsigned int> indices);
        void DoTransformations(unsigned int begin, unsigned int offset);
        void DoTransformations() override;

        // En: Gets the vertex at the specified index and offset.
        // Tr: Belirtilen index ve offset'teki vertex'i alır.
        template <typename T>
        T GetVertex(unsigned int index, unsigned int offset);
        template <>
        float GetVertex<float>(unsigned int index, unsigned int offset);
        template <>
        glm::vec2 GetVertex<glm::vec2>(unsigned int index, unsigned int offset);
        template <>
        glm::vec3 GetVertex<glm::vec3>(unsigned int index, unsigned int offset);
        template <>
        glm::vec4 GetVertex<glm::vec4>(unsigned int index, unsigned int offset);

        int GetVertexCount() const;
        int GetVertexSize() const;

        float *GetVertices(int &size);
        unsigned int *GetIndices(int &size);
    };

    Mesh *CreateBox(float width, float height, float depth, int vertexSize = 3);
    Mesh *CreateBox(float width, float height, float depth, Shader *shader);
    Mesh *CreatePlane(float width, float height, int vertexSize = 3);
    Mesh *CreatePlane(float width, float height, Shader *shader);
    Mesh *CreateCylinder(float radius, float height, int sectorCount, int vertexSize = 3);
    Mesh *CreateCylinder(float radius, float height, int sectorCount, Shader *shader);
    Mesh *CreateCone(float radius, float height, int sectorCount, int vertexSize = 3);
    Mesh *CreateCone(float radius, float height, int sectorCount, Shader *shader);
}
#endif