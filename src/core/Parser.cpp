#ifndef PARSER_CPP
#define PARSER_CPP

#include "Parser.h"

#include <map>
#include <functional>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>

namespace parseShape
{
    static simdjson::dom::parser parser;
    static Shader *globalShader = nullptr;
    Scene *Parse(std::string path)
    {
        if (path.empty())
            return nullptr;

        Scene *scene = nullptr;

        std::string ext = path.substr(path.find_last_of(".") + 1);

        if (ext == "xml")
        {

            std::ifstream stream(path, std::ios::binary);
            if (!stream.is_open())
                return nullptr;

            std::string content = "", line;
            while (std::getline(stream, line))
                content += line;

            tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
            doc->Parse(content.c_str());
            scene = xml::Parse(doc);
            delete doc;
        }
        else if (ext == "json")
        {
            if (!std::filesystem::exists(path))
                return nullptr;
            auto str = simdjson::padded_string::load(path);
            simdjson::dom::document doc;
            parser.parse_into_document(doc, std::string(str.value()).c_str(), str.value().length());
            scene = json::Parse(&doc);
        }

        return scene;
    }
    namespace json
    {

        Scene *Parse(simdjson::dom::document *document)
        {
            simdjson::dom::element element = document->root();
            Scene *scene = Parse(&element);

            return scene;
        }
        Scene *Parse(simdjson::dom::element *element)
        {
            Scene *scene = new Scene();

            for (auto field : (*element)["scene"].get_array())
            {

                if (field["class"].error() != simdjson::SUCCESS)
                    continue;

                simdjson::dom::element *props = nullptr;
                if (field.at_key("props").error() == simdjson::SUCCESS)
                {
                    simdjson::dom::element e = field.at_key("props");
                    props = &e;
                }

                std::string className(field["class"].get_string().value());
                std::transform(className.begin(), className.end(), className.begin(), [](unsigned char c)
                               { return std::tolower(c); });

                if (meshFuncs.find(className) != meshFuncs.end())
                {
                    Mesh *mesh = meshFuncs[className](props);
                    scene->AddMesh(mesh);
                    for (auto modifier : field.get_object())
                    {
                        if (meshModifierFuncs.find(std::string(modifier.key)) != meshModifierFuncs.end())
                        {
                            std::string_view modfierClassName = modifier.key;
                            std::string modfierClass(modfierClassName);
                            std::transform(modfierClass.begin(), modfierClass.end(), modfierClass.begin(), [](unsigned char c)
                                           { return std::tolower(c); });

                            meshModifierFuncs[modfierClass](&(modifier.value), mesh);
                        }
                        else if (objectModifierFuncs.find(std::string(modifier.key)) != objectModifierFuncs.end())
                        {
                            std::string_view modfierClassName = modifier.key;
                            objectModifierFuncs[std::string(modfierClassName)](&(modifier.value), mesh);
                        }
                    }
                }
                else if (objectFuncs.find(className) != objectFuncs.end())
                {
                    Object *object = objectFuncs[className](props);
                    Camera *camera = dynamic_cast<Camera *>(object);
                    if (camera != nullptr)
                        scene->SetCamera(camera);
                    for (auto modifier : field.get_object())
                    {
                        if (objectModifierFuncs.find(std::string(modifier.key)) != objectModifierFuncs.end())
                        {
                            std::string_view modfierClassName = modifier.key;
                            std::string modfierClass(modfierClassName);
                            std::transform(modfierClass.begin(), modfierClass.end(), modfierClass.begin(), [](unsigned char c)
                                           { return std::tolower(c); });
                            objectModifierFuncs[modfierClass](&(modifier.value), object);
                        }
                    }
                }
                else if (otherFuncs.find(className) != otherFuncs.end())
                {
                    OtherType ot = otherFuncs[className](props);
                    if (std::holds_alternative<Shader *>(ot))
                    {
                        Shader *shader = std::get<Shader *>(ot);
                        scene->SetShader(shader);
                        for (auto modifier : field.get_object())
                        {
                            if (shaderModifierFuncs.find(std::string(modifier.key)) != shaderModifierFuncs.end())
                            {
                                std::string_view modfierClassName = modifier.key;
                                shaderModifierFuncs[std::string(modfierClassName)](&(modifier.value), shader);
                            }
                        }
                    }
                }
            }
            return scene;
        }

        Mesh *Box(simdjson::dom::element *element)
        {
            float width = 1.0f, depth = 1.0f, height = 1.0f;

            if (element)
            {

                if (element->at_key("w").error() == simdjson::SUCCESS)
                    width = element->at_key("w").get_double();
                else if (element->at_key("x").error() == simdjson::SUCCESS)
                    width = element->at_key("x").get_double();
                else if (element->at_key("width").error() == simdjson::SUCCESS)
                    width = element->at_key("width").get_double();

                if (element->at_key("d").error() == simdjson::SUCCESS)
                    depth = element->at_key("d").get_double();
                else if (element->at_key("z").error() == simdjson::SUCCESS)
                    depth = element->at_key("z").get_double();
                else if (element->at_key("depth").error() == simdjson::SUCCESS)
                    depth = element->at_key("depth").get_double();

                if (element->at_key("h").error() == simdjson::SUCCESS)
                    height = element->at_key("h").get_double();
                else if (element->at_key("y").error() == simdjson::SUCCESS)
                    height = element->at_key("y").get_double();
                else if (element->at_key("height").error() == simdjson::SUCCESS)
                    height = element->at_key("height").get_double();
            }
            return CreateBox(width, depth, height, 9);
        }
        Mesh *Plane(simdjson::dom::element *element)
        {
            float width = 1.0f, height = 1.0f;

            if (element)
            {
                if (element->at_key("w").error() == simdjson::SUCCESS)
                    width = element->at_key("w").get_double();
                else if (element->at_key("width").error() == simdjson::SUCCESS)
                    width = element->at_key("width").get_double();

                if (element->at_key("h").error() == simdjson::SUCCESS)
                    height = element->at_key("h").get_double();
                else if (element->at_key("y").error() == simdjson::SUCCESS)
                    height = element->at_key("y").get_double();
                else if (element->at_key("height").error() == simdjson::SUCCESS)
                    height = element->at_key("height").get_double();
            }
            return CreatePlane(width, height, 9);
        }
        Mesh *Cylinder(simdjson::dom::element *element)
        {
            float radius = 1.0f, height = 1.0f;
            int sectorCount = 32;
            if (element)
            {

                if (element->at_key("r").error() == simdjson::SUCCESS)
                    radius = element->at_key("r").get_double();
                else if (element->at_key("radius").error() == simdjson::SUCCESS)
                    radius = element->at_key("radius").get_double();

                if (element->at_key("h").error() == simdjson::SUCCESS)
                    height = element->at_key("h").get_double();
                else if (element->at_key("height").error() == simdjson::SUCCESS)
                    height = element->at_key("height").get_double();

                if (element->at_key("sectorCount").error() == simdjson::SUCCESS)
                    sectorCount = element->at_key("sectorCount").get_int64();
                else if (element->at_key("resolution").error() == simdjson::SUCCESS)
                    sectorCount = element->at_key("resolution").get_int64();
                else if (element->at_key("sc").error() == simdjson::SUCCESS)
                    sectorCount = element->at_key("sc").get_int64();
                else if (element->at_key("n").error() == simdjson::SUCCESS)
                    sectorCount = element->at_key("n").get_int64();
            }
            return CreateCylinder(radius, height, sectorCount, 9);
        }
        Mesh *Cone(simdjson::dom::element *element)
        {
            float radius = 1.0f, height = 1.0f;
            int sectorCount = 32;
            if (element)
            {

                if (element->at_key("r").error() == simdjson::SUCCESS)
                    radius = element->at_key("r").get_double();
                else if (element->at_key("radius").error() == simdjson::SUCCESS)
                    radius = element->at_key("radius").get_double();

                if (element->at_key("h").error() == simdjson::SUCCESS)
                    height = element->at_key("h").get_double();
                else if (element->at_key("height").error() == simdjson::SUCCESS)
                    height = element->at_key("height").get_double();

                if (element->at_key("sectorCount").error() == simdjson::SUCCESS)
                    sectorCount = element->at_key("sectorCount").get_int64();
                else if (element->at_key("resolution").error() == simdjson::SUCCESS)
                    sectorCount = element->at_key("resolution").get_int64();
                else if (element->at_key("sc").error() == simdjson::SUCCESS)
                    sectorCount = element->at_key("sc").get_int64();
                else if (element->at_key("n").error() == simdjson::SUCCESS)
                    sectorCount = element->at_key("n").get_int64();
            }
            return CreateCone(radius, height, sectorCount, 9);
        }

        void Color(simdjson::dom::element *element, Mesh *mesh)
        {
            if (mesh == nullptr)
                return;

            float r = 1.0, g = 1.0, b = 1.0;
            if ((*element).at_key("r").error() == simdjson::SUCCESS)
                r = (*element)["r"].get_double();
            else if ((*element).at_key("red").error() == simdjson::SUCCESS)
                r = (*element)["red"].get_double();

            if ((*element).at_key("g").error() == simdjson::SUCCESS)
                g = (*element)["g"].get_double();
            else if ((*element).at_key("green").error() == simdjson::SUCCESS)
                g = (*element)["green"].get_double();

            if ((*element).at_key("b").error() == simdjson::SUCCESS)
                b = (*element)["b"].get_double();
            else if ((*element).at_key("blue").error() == simdjson::SUCCESS)
                b = (*element)["blue"].get_double();

            for (int i = 0; i < mesh->GetVertexCount(); i++)
            {
                mesh->ChangeVertex(glm::vec3(r, g, b), i, 6);
            }
        }
        void Normal(simdjson::dom::element *element, Mesh *mesh)
        {
            if (mesh == nullptr)
                return;

            float x = 0.0f, y = 0.0f, z = 0.0f;
            if ((*element).at_key("x").error() == simdjson::SUCCESS)
                x = (*element)["x"].get_double();

            if ((*element).at_key("y").error() == simdjson::SUCCESS)
                y = (*element)["y"].get_double();

            if ((*element).at_key("z").error() == simdjson::SUCCESS)
                z = (*element)["z"].get_double();

            for (int i = 0; i < mesh->GetVertexCount(); i++)
            {
                mesh->ChangeVertex(glm::vec3(x, y, z), i, 3);
            }
        }

        void Translate(simdjson::dom::element *element, Object *realObject)
        {
            if (realObject == nullptr)
                return;

            float x = 0.0f, y = 0.0f, z = 0.0f;
            if ((*element).at_key("x").error() == simdjson::SUCCESS)
                x = (*element)["x"].get_double();

            if ((*element).at_key("y").error() == simdjson::SUCCESS)
                y = (*element)["y"].get_double();

            if ((*element).at_key("z").error() == simdjson::SUCCESS)
                z = (*element)["z"].get_double();

            realObject->Translate(glm::vec3(x, y, z));
        }
        void Rotate(simdjson::dom::element *element, Object *realObject)
        {
            if (realObject == nullptr)
                return;

            float x = 0.0f, y = 0.0f, z = 0.0f;

            if ((*element).at_key("x").error() == simdjson::SUCCESS)
                x = (*element)["x"].get_double();
            if ((*element).at_key("y").error() == simdjson::SUCCESS)
                y = (*element)["y"].get_double();
            if ((*element).at_key("z").error() == simdjson::SUCCESS)
                z = (*element)["z"].get_double();

            realObject->Rotate(glm::quat(glm::vec3(x, y, z)));
        }
        void Scale(simdjson::dom::element *element, Object *realObject)
        {
            if (realObject == nullptr)
                return;

            float x = 1.0f, y = 1.0f, z = 1.0f;
            if ((*element).at_key("x").error() == simdjson::SUCCESS)
                x = (*element)["x"].get_double();

            if ((*element).at_key("y").error() == simdjson::SUCCESS)
                y = (*element)["y"].get_double();

            if ((*element).at_key("z").error() == simdjson::SUCCESS)
                z = (*element)["z"].get_double();

            realObject->Scale(glm::vec3(x, y, z));
        }

        Camera *_Camera(simdjson::dom::element *element)
        {
            Camera *camera = new Camera();
            if (element->at_key("type").error() == simdjson::SUCCESS)
            {
                std::string type((*element)["type"].get_string().value());
                if (type == "perspective" || type == "p")
                {
                    camera->SetType(CameraType::Perspective);
                    float fov = 45.0f, aspect = 1.0f, pNear = 0.1f, pFar = 100.0f;
                    if (element->at_key("fov").error() == simdjson::SUCCESS)
                        fov = element->at_key("fov").get_double();
                    if (element->at_key("aspect").error() == simdjson::SUCCESS)
                        aspect = element->at_key("aspect").get_double();
                    if (element->at_key("near").error() == simdjson::SUCCESS)
                        pNear = element->at_key("near").get_double();
                    if (element->at_key("far").error() == simdjson::SUCCESS)
                        pFar = element->at_key("far").get_double();
                    camera->SetPerspective(fov, aspect, pNear, pFar);
                }
                else if (type == "orthographic" || type == "o")
                {
                    camera->SetType(CameraType::Orthographic);
                    float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f, oNear = 0.1f, oFar = 100.0f;
                    if (element->at_key("left").error() == simdjson::SUCCESS)
                        left = element->at_key("left").get_double();
                    if (element->at_key("right").error() == simdjson::SUCCESS)
                        right = element->at_key("right").get_double();
                    if (element->at_key("bottom").error() == simdjson::SUCCESS)
                        bottom = element->at_key("bottom").get_double();
                    if (element->at_key("top").error() == simdjson::SUCCESS)
                        top = element->at_key("top").get_double();
                    if (element->at_key("near").error() == simdjson::SUCCESS)
                        oNear = element->at_key("near").get_double();
                    if (element->at_key("far").error() == simdjson::SUCCESS)
                        oFar = element->at_key("far").get_double();
                    camera->SetOrthographic(left, right, bottom, top, oNear, oFar);
                }
            }
            return camera;
        }

        Shader *_Shader(simdjson::dom::element *element)
        {
            Shader *shader = nullptr;
            const char *vertexPath = element->at_key("vertex").get_c_str();
            if (!vertexPath)
                vertexPath = element->at_key("vertexPath").get_c_str();
            const char *fragmentPath = element->at_key("fragment").get_c_str();
            if (!fragmentPath)
                fragmentPath = element->at_key("fragmentPath").get_c_str();
            if (vertexPath && fragmentPath)
                shader = new Shader(vertexPath, fragmentPath);
            return shader;
        }

        void Set(simdjson::dom::element *element, Shader *shader)
        {
            const char *name = element->at_key("name").get_c_str(), *value = element->at_key("value").get_c_str();
            if (!shader || !name || !value)
                return;
        }
    }
    namespace xml
    {

        Scene *Parse(tinyxml2::XMLDocument *document)
        {
            return Parse(document->RootElement());
        }
        Scene *Parse(tinyxml2::XMLElement *element)
        {
            Scene *scene = new Scene();

            for (tinyxml2::XMLElement *e = element->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
            {
                std::string typeName(e->Name());
                std::transform(typeName.begin(), typeName.end(), typeName.begin(), [](unsigned char c)
                               { return tolower(c); });
                if (meshFuncs.find(typeName) != meshFuncs.end())
                {
                    Mesh *mesh = meshFuncs[typeName](e);
                    scene->AddMesh(mesh);
                    for (tinyxml2::XMLElement *o = e->FirstChildElement(); o != nullptr; o = o->NextSiblingElement())
                    {
                        std::string objectName(o->Name());
                        std::transform(objectName.begin(), objectName.end(), objectName.begin(), [](unsigned char c)
                                       { return tolower(c); });

                        if (meshModifierFuncs.find(objectName) != meshModifierFuncs.end())
                            meshModifierFuncs[objectName](o, mesh);

                        if (objectModifierFuncs.find(objectName) != objectModifierFuncs.end())
                            objectModifierFuncs[objectName](o, mesh);
                    }
                }
                if (objectFuncs.find(typeName) != objectFuncs.end())
                {
                    Object *object = objectFuncs[typeName](e);
                    Camera *camera = dynamic_cast<Camera *>(object);
                    if (camera != nullptr)
                        scene->SetCamera(camera);
                    for (tinyxml2::XMLElement *o = e->FirstChildElement(); o; o = o->NextSiblingElement())
                    {
                        std::string objectName(o->Name());
                        std::transform(objectName.begin(), objectName.end(), objectName.begin(), [](unsigned char c)
                                       { return tolower(c); });

                        if (objectModifierFuncs.find(objectName) != objectModifierFuncs.end())
                            objectModifierFuncs[objectName](o, object);
                    }
                }
                if (otherFuncs.find(typeName) != otherFuncs.end())
                {
                    OtherType ot = otherFuncs[typeName](e);
                    if (std::holds_alternative<Shader *>(ot))
                    {
                        Shader *shader = std::get<Shader *>(ot);
                        if (!globalShader)
                            globalShader = shader;
                        scene->SetShader(shader);
                        for (tinyxml2::XMLElement *o = e->FirstChildElement(); o; o = o->NextSiblingElement())
                        {
                            std::string objectName(o->Name());
                            std::transform(objectName.begin(), objectName.end(), objectName.begin(), [](unsigned char c)
                                           { return tolower(c); });

                            if (objectModifierFuncs.find(objectName) != objectModifierFuncs.end())
                                shaderModifierFuncs[objectName](o, shader);
                        }
                    }
                }
            }

            globalShader = nullptr;

            return scene;
        }

        Mesh *Box(tinyxml2::XMLElement *element)
        {
            float width = 1.0f, depth = 1.0f, height = 1.0f;
            if (element->Attribute("w"))
                width = element->FloatAttribute("w");
            else if (element->Attribute("x"))
                width = element->FloatAttribute("x");
            else if (element->Attribute("width"))
                width = element->FloatAttribute("width");

            if (element->Attribute("d"))
                depth = element->FloatAttribute("d");
            else if (element->Attribute("z"))
                depth = element->FloatAttribute("z");
            else if (element->Attribute("depth"))
                depth = element->FloatAttribute("depth");

            if (element->Attribute("h"))
                height = element->FloatAttribute("h");
            else if (element->Attribute("y"))
                height = element->FloatAttribute("y");
            else if (element->Attribute("height"))
                height = element->FloatAttribute("height");

            if (globalShader)
                return CreateBox(width, depth, height, globalShader);
            else
                return CreateBox(width, depth, height, 9);
        }
        Mesh *Plane(tinyxml2::XMLElement *element)
        {
            float width = 1.0f, height = 1.0f;
            if (element->Attribute("w"))
                width = element->FloatAttribute("w");
            else if (element->Attribute("width"))
                width = element->FloatAttribute("width");
            if (element->Attribute("h"))
                height = element->FloatAttribute("h");
            else if (element->Attribute("height"))
                height = element->FloatAttribute("height");

            if (globalShader)
                return CreatePlane(width, height, globalShader);
            else
                return CreatePlane(width, height, 9);
        }
        Mesh *Cylinder(tinyxml2::XMLElement *element)
        {
            float radius = 1.0f, height = 1.0f;
            int sectorCount = 32;

            if (element->Attribute("r"))
                radius = element->FloatAttribute("r");
            else if (element->Attribute("radius"))
                radius = element->FloatAttribute("radius");

            if (element->Attribute("h"))
                height = element->FloatAttribute("h");
            else if (element->Attribute("height"))
                height = element->FloatAttribute("height");

            if (element->Attribute("sectorCount"))
                sectorCount = element->IntAttribute("sectorCount");
            else if (element->Attribute("resolution"))
                sectorCount = element->IntAttribute("resolution");
            else if (element->Attribute("sc"))
                sectorCount = element->IntAttribute("sc");
            else if (element->Attribute("n"))
                sectorCount = element->IntAttribute("n");

            if (globalShader)
                return CreateCylinder(radius, height, sectorCount, globalShader);
            else
                return CreateCylinder(radius, height, sectorCount, 9);
        }
        Mesh *Cone(tinyxml2::XMLElement *element)
        {
            float radius = 1.0f, height = 1.0f;
            int sectorCount = 32;
            if (element->Attribute("r"))
                radius = element->FloatAttribute("r");
            else if (element->Attribute("radius"))
                radius = element->FloatAttribute("radius");
            if (element->Attribute("h"))
                height = element->FloatAttribute("h");
            else if (element->Attribute("height"))
                height = element->FloatAttribute("height");
            if (element->Attribute("sectorCount"))
                sectorCount = element->IntAttribute("sectorCount");
            else if (element->Attribute("resolution"))
                sectorCount = element->IntAttribute("resolution");

            if (globalShader)
                return CreateCone(radius, height, sectorCount, globalShader);
            else
                return CreateCone(radius, height, sectorCount, 9);
        }

        void Color(tinyxml2::XMLElement *element, Mesh *mesh)
        {
            if (mesh == nullptr)
                return;

            float r = 1.0f, g = 1.0f, b = 1.0f;
            if (element->Attribute("r"))
                r = element->FloatAttribute("r");
            else if (element->Attribute("red"))
                r = element->FloatAttribute("red");

            if (element->Attribute("g"))
                g = element->FloatAttribute("g");
            else if (element->Attribute("green"))
                g = element->FloatAttribute("green");

            if (element->Attribute("b"))
                b = element->FloatAttribute("b");
            else if (element->Attribute("blue"))
                b = element->FloatAttribute("blue");

            for (int i = 0; i < mesh->GetVertexCount(); i++)
            {
                mesh->ChangeVertex(glm::vec3(r, g, b), i, 6);
            }
        }
        void Normal(tinyxml2::XMLElement *element, Mesh *mesh)
        {
            if (mesh == nullptr)
                return;

            float x = 0.0f, y = 0.0f, z = 0.0f;
            if (element->Attribute("x"))
                x = element->FloatAttribute("x");

            if (element->Attribute("y"))
                y = element->FloatAttribute("y");

            if (element->Attribute("z"))
                z = element->FloatAttribute("z");

            for (int i = 0; i < mesh->GetVertexCount(); i++)
            {
                mesh->ChangeVertex(glm::vec3(x, y, z), i, 3);
            }
        }
        void Set_Mesh(tinyxml2::XMLElement *element, Mesh *mesh)
        {
            if (!mesh)
                return;
            const char *name = element->Attribute("name"), *type = element->Attribute("type");
            if (!name || !type)
                return;
            std::function<void(int)> func = nullptr;
            switch (attributeTable[std::string(type)])
            {
            case FLOAT:
            {
                float fValue = element->FloatAttribute("value");
                func = [mesh, name, fValue](int i)
                { mesh->ChangeVertex(i, fValue, name); };
                break;
            }
            case INT:
            {
                int iValue = element->IntAttribute("value");
                func = [mesh, name, iValue](int i)
                { mesh->ChangeVertex(i, iValue, name); };
                break;
            }
            case VEC2:
            {
                glm::vec2 v2Value(element->FloatAttribute("x"), element->FloatAttribute("y"));
                func = [mesh, name, v2Value](int i)
                { mesh->ChangeVertex(i, v2Value, name); };
                break;
            }
            case VEC3:
            {
                glm::vec3 v3Value(element->FloatAttribute("x"), element->FloatAttribute("y"), element->FloatAttribute("z"));
                func = [mesh, name, v3Value](int i)
                { mesh->ChangeVertex(i, v3Value, name); };
                break;
            }
            }
            for (int i = 0; i < mesh->GetVertexCount(); i++)
                func(i);
        }

        void Translate(tinyxml2::XMLElement *element, Object *object)
        {

            if (object == nullptr)
                return;

            float x = 0.0f, y = 0.0f, z = 0.0f;
            if (element->Attribute("x"))
                x = element->FloatAttribute("x");

            if (element->Attribute("y"))
                y = element->FloatAttribute("y");

            if (element->Attribute("z"))
                z = element->FloatAttribute("z");

            object->Translate(glm::vec3(x, y, z));
        }
        void Rotate(tinyxml2::XMLElement *element, Object *object)
        {
            if (object == nullptr)
                return;

            float x = 0.0f, y = 0.0f, z = 0.0f;
            if (element->Attribute("x"))
                x = element->FloatAttribute("x");

            if (element->Attribute("y"))
                y = element->FloatAttribute("y");

            if (element->Attribute("z"))
                z = element->FloatAttribute("z");

            object->Rotate(glm::quat(glm::vec3(x, y, z)));
        }
        void Scale(tinyxml2::XMLElement *element, Object *object)
        {
            if (object == nullptr)
                return;

            float x = 1.0f, y = 1.0f, z = 1.0f;
            if (element->Attribute("x"))
                x = element->FloatAttribute("x");

            if (element->Attribute("y"))
                y = element->FloatAttribute("y");

            if (element->Attribute("z"))
                z = element->FloatAttribute("z");

            object->Scale(glm::vec3(x, y, z));
        }

        Camera *_Camera(tinyxml2::XMLElement *element)
        {
            Camera *camera = new Camera();

            const char *type = element->Attribute("type");
            if (std::strcmp(type, "perspective") == 0 || std::strcmp(type, "p") == 0)
            {
                camera->SetType(CameraType::Perspective);
                float fov = 45.0f, aspect = 1.0f, pNear = 0.1f, pFar = 100.0f;
                if (element->Attribute("fov"))
                    fov = element->FloatAttribute("fov");
                if (element->Attribute("aspect"))
                    aspect = element->FloatAttribute("aspect");
                if (element->Attribute("near"))
                    pNear = element->FloatAttribute("near");
                if (element->Attribute("far"))
                    pFar = element->FloatAttribute("far");
                camera->SetPerspective(fov, aspect, pNear, pFar);
            }
            else if (std::strcmp(type, "orthographic") == 0 || std::strcmp(type, "o") == 0)
            {
                camera->SetType(CameraType::Orthographic);
                float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f, oNear = 0.1f, oFar = 100.0f;
                if (element->Attribute("left"))
                    left = element->FloatAttribute("left");
                if (element->Attribute("right"))
                    right = element->FloatAttribute("right");
                if (element->Attribute("bottom"))
                    bottom = element->FloatAttribute("bottom");
                if (element->Attribute("top"))
                    top = element->FloatAttribute("top");
                if (element->Attribute("near"))
                    oNear = element->FloatAttribute("near");
                if (element->Attribute("far"))
                    oFar = element->FloatAttribute("far");
                camera->SetOrthographic(left, right, bottom, top, oNear, oFar);
            }

            return camera;
        }

        Shader *_Shader(tinyxml2::XMLElement *element)
        {
            Shader *shader = nullptr;
            const char *vertexPath = element->Attribute("vertex");
            const char *fragmentPath = element->Attribute("fragment");
            if (vertexPath && fragmentPath)
                shader = new Shader(vertexPath, fragmentPath);
            return shader;
        }

        void Set_Shader(tinyxml2::XMLElement *element, Shader *shader)
        {
            const char *name = element->Attribute("name"), *value = element->Attribute("value");
            if (!shader || !name || !value)
                return;
        }
    }
}
#endif