#ifndef PARSER_CPP
#define PARSER_CPP

#include "Parser.h"

#include <map>
#include <functional>
#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>

namespace xmls
{
    static std::map<std::string, std::function<Mesh *(tinyxml2::XMLElement *)>> meshFuncs =
        {{"box", Box}, {"plane", Plane}, {"cylinder", Cylinder}, {"cone", Cone}};

    static std::map<std::string, std::function<void(tinyxml2::XMLElement *, Mesh *)>> meshModifierFuncs =
        {{"color", Color}, {"normal", Normal}};

    static std::map<std::string, std::function<Object *(tinyxml2::XMLElement *)>> objectFuncs =
        {{"camera", Cam}, {"cam", Cam}};

    static std::map<std::string, std::function<void(tinyxml2::XMLElement *, Object *)>> objectModifierFuncs =
        {{"translate", Translate}, {"rotate", Rotate}, {"scale", Scale}};

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
                for (tinyxml2::XMLElement *o = e->FirstChildElement(); o; o = o->NextSiblingElement())
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
        }

        return scene;
    }
    Scene *Parse(std::string path)
    {
        std::ifstream stream(path, std::ios::binary);
        if (!stream.is_open())
            return nullptr;

        std::string content = "", line;
        while (std::getline(stream, line))
            content += line;

        tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();
        doc->Parse(content.c_str());
        return Parse(doc);
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

        return CreateCone(radius, height, sectorCount, 9);
    }

    void Color(tinyxml2::XMLElement *element, Mesh *mesh)
    {
        if (mesh == nullptr && std::strcmp(element->Name(), "color") != 0)
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
        if (mesh == nullptr && std::strcmp(element->Name(), "normal") != 0)
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

    void Translate(tinyxml2::XMLElement *element, Object *object)
    {
        if (object == nullptr && std::strcmp(element->Name(), "translate") != 0)
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
        if (object == nullptr && std::strcmp(element->Name(), "rotate") != 0)
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
        if (object == nullptr && std::strcmp(element->Name(), "scale") != 0)
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

    Camera *Cam(tinyxml2::XMLElement *element)
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
}
#endif