#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>
#include <variant>
#include <vector>
#include <map>
#include <string>
#include <functional>

// En : Holds possible vertex data types for a vertex attribute.
// Tr : Bir vertex attribute için olası vertex veri türlerini tutar.
typedef std::variant<float, glm::vec2, glm::vec3, glm::vec4> VertexData;
// En : Holds possible vertex attribute types.
// Tr : Olası vertex attribute türlerini tutar.
typedef std::map<unsigned int, std::function<void(unsigned int)>> VertexAttributeMap;

// En : Vertex structure to store vertex data and its index.
// Tr : Vertex verini ve indisini saklamak için Vertex yapısı.
struct Vertex
{
    // En : The real data of the vertex.
    // Tr : Vertexin gerçek verisi.
    float *dataPtr = nullptr;
    // En : The index of the vertex in the vertex buffer.
    // Tr : Vertex buffer içindeki vertexin indeksi.
    unsigned int index = 0;
    // En : The data to manipulate the vertex.
    // Tr : Vertexi manipüle etmek için veri.
    std::vector<VertexData> data;
    // En : Sets the vertex data.
    // Tr : Vertex verisini ayarlar.
    void SetData(const VertexData &data, unsigned int index);
    // En : Applies the vertex data to the vertex buffer.
    // Tr : Vertex verisini vertex buffer'a uygular.
    void Apply();
};

#endif