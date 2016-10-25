#pragma once
struct MainHeader
{
	unsigned int type;
};
struct CreateMesh
{
	unsigned int nameLength;
	unsigned int vertexCount;
	unsigned int indexCount;
	unsigned int normalCount;
	unsigned int normalIndexCount;
	/*The name will be placed after the vertices and poop
	, this number will be the exact number of characters
	that make up the name. So put a null byte at nameLength+1*/
	unsigned int uvCount;
	unsigned int uvIndexCount;
};
struct Transformation
{
	unsigned int nameLength;
	unsigned int modified;
};
struct nameChange
{
	unsigned int nameLength;
	unsigned int newNameLength;
};
struct modifyVertex
{
	unsigned int nameLength;
	unsigned int nrOfVertices;
	unsigned int indexLength;
	unsigned int normalIdLength;
};
struct CreateCamera
{
	unsigned int nameLength;
};
struct Vertex
{
	float x, y, z;
};
struct Index
{
	int nr;
};
struct Normals
{
	float x, y, z;
};
struct Matrix
{
	double m[16];
};
struct floatMatrix
{
	float m[16];
};
struct Vector//vectors are shit
{
	float x, y, z;
	Vector& operator=(const MVector& other)
	{
		x = (float)other.x;
		y = (float)other.y;
		z = (float)other.z;
		return *this;
	}
};
struct sendVertex
{
	unsigned int id;
	unsigned int nrNorm;
	Vector translation;
};
struct sendNormal
{
	unsigned int id;
	Vector translation;
};
struct Vector4
{
	float x, y, z, w;
};
struct sendPersp
{
	Vector translation;
	Vector4 rotation;
};
struct sendOrtho
{
	Vector translation;
	double orthoWidth;
};
#pragma region MaterialStructs
struct CreateMaterial
{
	unsigned int texturePathLength;
	unsigned int normalPathLength;
	unsigned int ambientPathLength;
	unsigned int specularPathlength;
	unsigned int nameLength;
	bool specular;
};
struct ambient
{
	float r, g, b;
};
struct diffuse
{
	float r, g, b, coeff;
};
struct specular
{
	float r, g, b, shine;
};
struct setMat
{
	unsigned int meshNameLength;
	unsigned int materialNameLength;
};
#pragma endregion
/*
enum MsgType
{
CREATE,
CHANGE,

NUMBER_OF_TYPES
};

enum MsgContain
{
MATERIAL_NEW,
TRANSFORM_NEW,
CAMERA_NEW,
MESH_NEW,
TEXTURE_NEW,
MATERAL_CHANGED,
VERTEX_CHANGE,
CAMERA_CHANGED,
DELETED,

NUMBER_OF_SETTINGS //if you wanna looop them all
};*/