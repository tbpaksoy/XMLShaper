#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
namespace parseShape
{

    enum AttributeType
    {
        BOOL,
        INT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4,
        SAMPLER2D
    };

    typedef std::map<std::string, AttributeType> AttributeTypeName;
    typedef std::map<std::string, int> AttributeLocation;
    typedef std::map<std::string, unsigned int> AttributeOffset;

    inline std::map<std::string, AttributeType> attributeTable = {
        {"bool", BOOL},
        {"int", INT},
        {"float", FLOAT},
        {"vec2", VEC2},
        {"vec3", VEC3},
        {"vec4", VEC4},
        {"mat2", MAT2},
        {"mat3", MAT3},
        {"mat4", MAT4},
        {"sampler2D", SAMPLER2D}};
}
#endif