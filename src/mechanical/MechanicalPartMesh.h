#ifndef MECHANICAL_PART_MESH_H
#define MECHANICAL_PART_MESH_H
#include "../core/Mesh.h"
namespace parseShape
{
    Mesh *CreateGear(int toothCount, int resolution, float innerRadius, float outerRadius, float height, float toothHeight, float toothWidth, int vertexSize = 3);
    Mesh *CreateHelicalGear(int toothCount, float radius, float height, float thickness, float toothHeight, float toothWidth, float toothSlope, float helixAngle, int vertexSize = 3);

}
#endif