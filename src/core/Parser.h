#ifndef PARSER_H
#define PARSER_H

#include "Scene.h"
#include "Object.h"
#include "Mesh.h"
#include "Camera.h"

#include <tinyxml2.h>
#include <simdjson.h>
#include <string>
namespace parseShape
{
    Scene *Parse(std::string path);
    namespace xml
    {
        Scene *Parse(tinyxml2::XMLDocument *document);
        Scene *Parse(tinyxml2::XMLElement *element);

        Mesh *Box(tinyxml2::XMLElement *element);
        Mesh *Plane(tinyxml2::XMLElement *element);
        Mesh *Cylinder(tinyxml2::XMLElement *element);
        Mesh *Cone(tinyxml2::XMLElement *element);

        void Color(tinyxml2::XMLElement *element, Mesh *mesh);
        void Normal(tinyxml2::XMLElement *element, Mesh *mesh);

        void Translate(tinyxml2::XMLElement *element, Object *object);
        void Rotate(tinyxml2::XMLElement *element, Object *object);
        void Scale(tinyxml2::XMLElement *element, Object *object);

        Camera *Cam(tinyxml2::XMLElement *element);
    }
    namespace json
    {
        Scene *Parse(simdjson::dom::document *document);
        Scene *Parse(simdjson::dom::element *element);

        Mesh *Box(simdjson::dom::element *element);
        Mesh *Plane(simdjson::dom::element *element);
        Mesh *Cylinder(simdjson::dom::element *element);
        Mesh *Cone(simdjson::dom::element *element);

        void Color(simdjson::dom::element *element, Mesh *mesh);
        void Normal(simdjson::dom::element *element, Mesh *mesh);

        void Translate(simdjson::dom::element *element, Object *realObject);
        void Rotate(simdjson::dom::element *element, Object *realObject);
        void Scale(simdjson::dom::element *element, Object *realObject);

        Camera *Cam(simdjson::dom::element *element);
    }

}
#endif