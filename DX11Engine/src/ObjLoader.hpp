#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "EngineTypes.hpp"
#include "Logger.hpp"

class ObjLoader
{
	struct VertexType
	{
		float3 pos;
		float2 tex;
		float3 norm;
	};
	struct FaceType
	{
		I32 v1, v2, v3;
		I32 t1, t2, t3;
		I32 n1, n2, n3;
	};
public:
	static bool LoadObj(const std::string& path, std::vector<float3>& vertices, std::vector<U32>& faces);
	static bool LoadObj(const char* path, std::vector<float3>& vertices, std::vector<U32>& faces);
private:
	static bool LoadObjPriv(const char* path, std::vector<float3>& vertices, std::vector<U32>& faces);
};

