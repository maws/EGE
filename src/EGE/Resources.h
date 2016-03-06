#pragma once

#include <unordered_map>
#include <string>

namespace EGE
{
	class MeshResource;
	class Resources
	{
	public:
		static MeshResource* getMesh(const std::string& path);
		static std::unordered_map<std::string, MeshResource*> m_meshes;
	};
}