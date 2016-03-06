#pragma once

#include <EGE/Application.hpp>
#include <EGE/RenderProgram.hpp>
#include <gmtl/gmtl.h>
#include <vmath.h>

namespace EGE
{
	class MeshResource
	{
	public:
		void create(const std::string& path);

		unsigned m_numVerts;
		unsigned m_vertexBuffer;
		unsigned m_colorBuffer;
	};
}