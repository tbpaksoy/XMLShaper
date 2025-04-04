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
        for (unsigned int i = 0, j = 0; i < toothCount; i++, j += resolution / toothCount)
        {
            mesh->AddVertex({std::cos(sectorStep * j) * toothHeight,
                             std::sin(sectorStep * j) * toothHeight,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0});
            mesh->AddVertex({std::cos(sectorStep * (j + 1)) * toothHeight,
                             std::sin(sectorStep * (j + 1)) * toothHeight,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0,
                             0});
            unsigned int index = mesh->GetVertexCount() - 2;
            mesh->AddIndices({j + resolution, j + resolution + 1, index});
            mesh->AddIndices({index + 1, j + resolution + 1, index});
        }
        return mesh;
    }
    Mesh *CreateHelicalGear(int toothCount, float radius, float height, float thickness, float toothHeight, float toothWidth, float toothSlope, float helixAngle, int vertexSize)
    {
        return nullptr;
    }
}
#endif