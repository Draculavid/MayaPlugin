#pragma once
struct MainHeader
{
	unsigned int type;
};
struct CreateMesh
{
	unsigned int vertexCount;
	unsigned int indexCount;
	unsigned int normalCount;
	/*The name will be placed after the vertices and poop
	, this number will be the exact number of characters
	that make up the name. So put a null byte at nameLength+1*/
	unsigned int nameLength;
	unsigned int uvCount;
	unsigned int uvIndexCount;
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