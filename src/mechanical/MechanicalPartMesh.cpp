#ifndef MECHANICAL_PART_MESH_CPP
#define MECHANICAL_PART_MESH_CPP

#include "MechanicalPartMesh.h"

#include <glm/glm.hpp>
#include <vector>
namespace parseShape
{
    Mesh *CreateGear(int toothCount, int resolution, float innerRadius, float outerRadius, float height, float toothHeight, float toothWidth, int vertexSize)
    {
        Mesh *mesh = new Mesh(resolution * 2, vertexSize);
        float sectorStep = 4 * (22.0f / 7.0f) / resolution / 2;
        int toothStep = resolution / toothCount;
        for (int i = 0; i < resolution; i++)
        {
            float angle = sectorStep * i;
            mesh->ChangeVertex(glm::vec3(cos(angle), sin(angle), 0) * innerRadius, i, 0);
            mesh->ChangeVertex(glm::vec3(cos(angle), sin(angle), 0) * outerRadius, i + resolution, 0);
        }
        for (int i = 0; i < resolution - 1; i++)
        {
            mesh->AddIndices(i, i + resolution, i + 1);
            mesh->AddIndices(i + 1, i + resolution, i + resolution + 1);
        }
        mesh->AddIndices(resolution - 1, resolution * 2 - 1, 0);
        mesh->AddIndices(0, resolution * 2 - 1, resolution);
        for (unsigned int i = 0; i < resolution; i += toothStep * 2)
        {
            for (unsigned int j = 0; j <= toothStep; j++)
            {
                float angle = sectorStep * (i + j + 1);
                mesh->AddVertex({std::cos(angle) * toothHeight,
                                 std::sin(angle) * toothHeight,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0,
                                 0});
                if (j)
                {
                    unsigned int index = mesh->GetVertexCount() - 1;
                    mesh->AddIndices({i + j + resolution, index, i + j + resolution + 1});
                    mesh->AddIndices({i + j + resolution, index, index - 1});
                }
            }
        }
        size_t actualSize = mesh->GetVertexSize();
        for (size_t i = 0; i < actualSize; i++)
        {
            mesh->AddVertex({});
        }
        return mesh;
    }
    Mesh *CreateHelicalGear(int toothCount, float radius, float height, float thickness, float toothHeight, float toothWidth, float toothSlope, float helixAngle, int vertexSize)
    {
        return nullptr;
    }
}
#endif