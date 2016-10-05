#pragma once
struct MainHeader
{
	unsigned int type;
};
struct TypeHeader
{
	unsigned int type;
};
struct CreateMesh
{
	unsigned int vertexCount;
	unsigned int indexCount;
	unsigned int normalCount;
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