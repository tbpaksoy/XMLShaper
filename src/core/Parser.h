#ifndef PARSER_H
#define PARSER_H

#include "Scene.h"
#include "Object.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Vertex.h"

#include <tinyxml2.h>
#include <simdjson.h>
#include <string>
#include <variant>
#include <map>
#include <functional>

namespace parseShape
{
    Scene *Parse(std::string path);
    typedef std::variant<Shader *, Scene *, Vertex> OtherType;
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
        void Set_Mesh(tinyxml2::XMLElement *element, Mesh *mesh);

        void Translate(tinyxml2::XMLElement *element, Object *object);
        void Rotate(tinyxml2::XMLElement *element, Object *object);
        void Scale(tinyxml2::XMLElement *element, Object *object);

        Camera *_Camera(tinyxml2::XMLElement *element);

        Shader *_Shader(tinyxml2::XMLElement *element);

        void Set_Shader(tinyxml2::XMLElement *element, Shader *shader);

        inline std::map<std::string, std::function<Mesh *(tinyxml2::XMLElement *)>> meshFuncs =
            {{"box", Box}, {"plane", Plane}, {"cylinder", Cylinder}, {"cone", Cone}};

        inline std::map<std::string, std::function<void(tinyxml2::XMLElement *, Mesh *)>> meshModifierFuncs =
            {{"color", Color}, {"normal", Normal}, {"set", Set_Mesh}};

        inline std::map<std::string, std::function<Object *(tinyxml2::XMLElement *)>> objectFuncs =
            {{"camera", _Camera}, {"cam", _Camera}};

        inline std::map<std::string, std::function<void(tinyxml2::XMLElement *, Object *)>> objectModifierFuncs =
            {{"translate", Translate}, {"rotate", Rotate}, {"scale", Scale}};

        inline std::map<std::string, std::function<OtherType(tinyxml2::XMLElement *)>> otherFuncs =
            {{"shader", _Shader}};

        inline std::map<std::string, std::function<void(tinyxml2::XMLElement *, Shader *)>> shaderModifierFuncs =
            {{"set", Set_Shader}, {"setshader", Set_Shader}, {"setuniform", Set_Shader}};
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

        Camera *_Camera(simdjson::dom::element *element);

        Shader *_Shader(simdjson::dom::element *element);

        void Set(simdjson::dom::element *element, Shader *shader);

        inline std::map<std::string, std::function<Mesh *(simdjson::dom::element *)>> meshFuncs =
            {{"box", Box}, {"plane", Plane}, {"cylinder", Cylinder}, {"cone", Cone}};

        inline std::map<std::string, std::function<void(simdjson::dom::element *, Mesh *)>> meshModifierFuncs =
            {{"color", Color}, {"normal", Normal}};

        inline std::map<std::string, std::function<Object *(simdjson::dom::element *)>> objectFuncs =
            {{"camera", _Camera}, {"cam", _Camera}};

        inline std::map<std::string, std::function<void(simdjson::dom::element *, Object *)>> objectModifierFuncs =
            {{"translate", Translate}, {"rotate", Rotate}, {"scale", Scale}};

        inline std::map<std::string, std::function<OtherType(simdjson::dom::element *)>> otherFuncs =
            {{"shader", _Shader}};

        inline std::map<std::string, std::function<void(simdjson::dom::element *, Shader *)>> shaderModifierFuncs =
            {{"set", Set}, {"setshader", Set}, {"setuniform", Set}};
    }
}
#endif