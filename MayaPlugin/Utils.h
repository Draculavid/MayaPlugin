#pragma once
#include <maya/MGlobal.h>

#pragma once

namespace tex
{
	using namespace std;

	enum textureType
	{
		DIFFUSE,
		NORMAL,
		AMBIENT,
		SPECULAR,
		INCANDESCENCE,
		TRANSPARENCY,

		TEXTURE_TYPE_COUNT
	};

	const MString textureTypes[TEXTURE_TYPE_COUNT] = { "color", "normalCamera", "ambientColor", "specularColor", "incandescence",  "transparency" };

}