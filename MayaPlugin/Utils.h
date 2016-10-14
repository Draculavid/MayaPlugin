#pragma once
#include <maya/MGlobal.h>

#pragma once

namespace tex
{
	using namespace std;

	enum textureType
	{
		DIFFUSE,
		AMBIENT,
		INCANDESCENCE,
		NORMAL,
		TRANSPARENCY,
		SPECULAR,

		TEXTURE_TYPE_COUNT
	};

	const MString textureTypes[TEXTURE_TYPE_COUNT] = { "color", "ambientColor", "incandescence", "normalCamera", "transparency", "specularColor" };

}