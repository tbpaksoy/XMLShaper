#ifndef PARSER_H
#define PARSER_H

#include "Scene.h"
#include "Object.h"
#include "Mesh.h"

#include <tinyxml2.h>
#include <string>
namespace xmls
{
    Scene *Parse(tinyxml2::XMLDocument *document);
    Scene *Parse(tinyxml2::XMLElement *element);
    Scene *Parse(std::string path);

    Mesh *Box(tinyxml2::XMLElement *element);
    Mesh *Plane(tinyxml2::XMLElement *element);
    Mesh *Cylinder(tinyxml2::XMLElement *element);
    Mesh *Cone(tinyxml2::XMLElement *element);

    void Color(tinyxml2::XMLElement *element, Mesh *mesh);
    void Normal(tinyxml2::XMLElement *element, Mesh *mesh);

    void Translate(tinyxml2::XMLElement *element, Object *object);
    void Rotate(tinyxml2::XMLElement *element, Object *object);
    void Scale(tinyxml2::XMLElement *element, Object *object);
}
#endif