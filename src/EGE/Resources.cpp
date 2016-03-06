#include <EGE\Resources.h>

#include <EGE\MeshResource.hpp>

namespace EGE
{
	std::unordered_map<std::string, MeshResource*> Resources::m_meshes;

	MeshResource* Resources::getMesh(const std::string& path)
	{
		auto find = m_meshes.find(path);
		if (find != m_meshes.end())
		{
			return find->second;
		}
		else
		{
			// Load mesh
			MeshResource* m = new MeshResource();
			m->create(path.c_str());
			m_meshes[path] = m;
			return m;
		}
	}
}