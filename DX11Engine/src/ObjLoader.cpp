#include "ObjLoader.hpp"

bool ObjLoader::LoadObj(const std::string& path, std::vector<float3>& vertices, std::vector<U32>& faces)
{
    return LoadObjPriv(path.c_str(), vertices, faces);
}

bool ObjLoader::LoadObj(const char* path, std::vector<float3>& vertices, std::vector<U32>& faces)
{
    return LoadObjPriv(path, vertices, faces);
}

bool ObjLoader::LoadObjPriv(const char* path, std::vector<float3>& vertices, std::vector<U32>& faces)
{
    std::ifstream f(path);

    if (!f.is_open() || !f.good())
    {
        LOG("Failed to open File, it may not be the proper path to the file.");
        return false;
    }

    std::string line;
    while (std::getline(f, line))
    {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            float3 xyz{};
            if (!(ss >> xyz.x >> xyz.y >> xyz.z))
            {
                LOG("Failed to parse vertex line properly");
            }

            vertices.emplace_back(xyz.x, xyz.y, -xyz.z);
        }

        if (prefix == "f")
        {
            U32 x = 0, y = 0, z = 0;
            if (!(ss >> x >> y >> z))
            {
                LOG("Failed to parse face line properly");
            }

            faces.push_back(x--);
            faces.push_back(y--);
            faces.push_back(z--);
        }
    }

    return true;
}
