/*Creator: David Wigelius*/
#include "maya_includes.h"
#include "structs.h"
#include <iostream>
#include "CircularBuffer.h"//<-------------------------- fix this later so that it's a lib

using namespace std;

CircularBuffer *producer;
MCallbackIdArray myCallbackArray;
float CurrentTime = 0; //kanske göra en pekare för att kunna kontrollera minne
char * msg;

#pragma region callbacks
void WorldMatrixModified(MObject &transformNode, MDagMessage::MatrixModifiedFlags &modified, void *clientData)
{
	MGlobal::displayInfo(MFnTransform(transformNode).name() + " worldmatrix changed");
}

void elapsedTimeFunction(float elapsedTime, float lastTime, void*clientData)
{
	CurrentTime += elapsedTime;
	MGlobal::displayInfo((MString("Current time: ")+=(CurrentTime)));

	/*if (CurrentTime > 0 && CurrentTime < 9)
	{
		producer->push("bajskorven", (size_t)11);
		MGlobal::displayInfo("TIMKE IS NOW SENDING!");
	}*/
}

void attributeChanged(MNodeMessage::AttributeMessage msg, MPlug &plug, MPlug &otherPlug, void*clientData)
{
	if (msg & MNodeMessage::kAttributeSet && !plug.isArray() && plug.isElement())
	{
		//plug.isCompound()
		//MFnTransform(plug.attribute()).getTranslation(MSpace::kWorld, NULL).x;
		MString values;
		float x = plug.child(0).asFloat();
		float y = plug.child(1).asFloat();
		float z = plug.child(2).asFloat();
		values += x; values += ", "; values += y; values += ", "; values += z;
		MGlobal::displayInfo("attribute changed: " + plug.name() + "\nNew value: " + values);
	}
}

void changedNameFunction(MObject &node, const MString &str, void*clientData)
{
	MGlobal::displayInfo("name changed, new name: " + MFnDagNode(node).name());
}
//fix adde node funtion so that you just use a function that creates everything/adds callbakcs
//add topology changed callback
void addedNodeFunction(MObject &node, void*clientData) //look at this function with teachers
{
	MGlobal::displayInfo("created: " + MFnTransform(node).name());

	/*connecting the new node the the worldmatrix function*/
	if (node.hasFn(MFn::kTransform))
	{
		MStatus Result = MS::kSuccess;
		MFnTransform trans = node;
		MDagPath meshDag = MDagPath::getAPathTo(trans.child(0));
		MCallbackId newId = MDagMessage::addWorldMatrixModifiedCallback(meshDag, WorldMatrixModified, NULL, &Result);
		if (Result == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
			{
				MGlobal::displayInfo(trans.name() + " Successfully added to the MatrixModified Function");
			}
		}
		newId = MNodeMessage::addNameChangedCallback(trans.child(0), changedNameFunction, NULL, &Result);
		if (Result == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
			{
				MGlobal::displayInfo("Connected the NameChangeFunction");
			}
			else
				MGlobal::displayInfo("failed to connect NameChangeFunction");
		}
		if (trans.child(0).hasFn(MFn::kMesh))
		{
			newId = MNodeMessage::addAttributeChangedCallback(trans.child(0), attributeChanged, NULL, &Result);
			if (Result == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
					MGlobal::displayInfo("made connection to the attributtes");
			}
			else
				MGlobal::displayInfo("failed to connect attributes");
		}
	}
}
#pragma endregion
#pragma region Creation
bool createMesh(MObject &node)
{
	
	MFnTransform transform = node;
	MFnMesh mMesh(((MFnTransform)(node)).child(0), NULL);
	MIntArray indexList, vertexList, normalCount, uvCount, uvIds;
	MFloatPointArray points;
	MFloatArray u, v;
	MFloatVectorArray normals;
	mMesh.getPoints(points, MSpace::kObject);
	mMesh.getTriangles(vertexList, indexList);

	/*assigning the main header to creation mode*/
	MainHeader sHeader{ 0 };

	/*Creating the headers to send*/
	CreateMesh sMesh;
	sMesh.vertexCount = points.length();
	sMesh.indexCount = indexList.length();
	sMesh.nameLength = transform.name().length();

	//mMesh.getVertexNormals(true, normals, MSpace::kObject);
	//mMesh.getFaceVertexNormals()
	//mMesh.getNormalIds
	//const float * punkts = mMesh.getRawPoints(NULL);
	mMesh.getTriangleOffsets(normalCount, vertexList);
	//mMesh.getVertices(normalCount, vertexList);

	const float * normz = mMesh.getRawNormals(NULL);
	mMesh.getNormals(normals, MSpace::kObject);

	MStatus hejsan = mMesh.getUVs(u, v);
	//double * kukuk = new double[u.length()];
	//u.get(kukuk);
	//mMesh.getUVs(u, v);
	mMesh.getAssignedUVs(uvCount, uvIds);
	sMesh.uvIndexCount = uvIds.length();

	sMesh.uvCount = u.length();

	MString info;
	/*for (int i = 0; i < u.length(); i++)
	{
		info += u[i];
		info += ", ";
		info += v[i];
		info = "\n";
	}*/
	MGlobal::displayInfo(info);

	sMesh.normalCount = normals.length();

	/*for (int i = 0; i < normals.length(); i++)
	{
		info += normals[i].x;
		info += ", ";
		info += normals[i].y;
		info += ", ";
		info += normals[i].z;
		info += "\n";
	}
	MGlobal::displayInfo(info);
	info = "";
	for (int i = 0; i < vertexList.length(); i++)
	{
		info += vertexList[i];
		info += "; ";
	}
	MGlobal::displayInfo(info);
	info = "";*/
	//Vertex *mVertex = new Vertex[points.length()];
	//Index *mIndex = new Index[indexList.length()];

	/*putting references to the points (vertex points)*/
	/*for (int i = 0; i < points.length(); i++)
	{
		info += points[i].x;
		info += ", ";
		info += points[i].y;
		info += ", ";
		info += points[i].z;
		info += "\n";
	}
	MGlobal::displayInfo(info);
	info = "";*/
	/*mVertex->x = points[0].x;
	mVertex->y = points[0].y;
	mVertex->z = points[0].z;*/

	/*putting reference to the indexes of the triangles*/
	/*for (int i = 0; i < indexList.length(); i++)
	{
		info += indexList[i];
		info += "; ";
	}
	MGlobal::displayInfo(info);
	info = "";
*/
	/*Calculating the length of the message and sending the creation info to the circular buffer*/
	int length = (sizeof(Vertex) * points.length())
		+ (sizeof(Index) * indexList.length())
		+ (sizeof(Normals) * normals.length())
		+ (sizeof(Index) * vertexList.length())
		+ sizeof(CreateMesh)
		+ sizeof(MainHeader)
		+ sizeof(Matrix)
		+ (sizeof(float) * u.length())*2
		+ (sizeof(Index)*sMesh.uvIndexCount)
		+ sMesh.nameLength;

	
	/*constructing the message*/
	char * pek = msg;

	memcpy(pek, (char*)&sHeader, sizeof(MainHeader));
	pek += sizeof(MainHeader);

	memcpy(pek, (char*)&sMesh, sizeof(CreateMesh));
	pek += sizeof(CreateMesh);

	memcpy(pek, (char*)&transform.transformationMatrix(), sizeof(Matrix));
	pek += sizeof(Matrix);

	memcpy(pek, (char*)mMesh.getRawPoints(NULL), (sizeof(Vertex)*sMesh.vertexCount));
	pek += sizeof(Vertex)*sMesh.vertexCount;

	memcpy(pek, (char*)&indexList[0], (sizeof(Index)*sMesh.indexCount));
	pek += sizeof(Index)*sMesh.indexCount;

	memcpy(pek, (char*)normz, (sizeof(Normals)*sMesh.normalCount));
	pek += sizeof(Normals)*sMesh.normalCount;

	memcpy(pek, (char*)&vertexList[0], (sizeof(Index)*sMesh.indexCount));
	pek += sizeof(Index)*sMesh.indexCount;

	memcpy(pek, (char*)&u[0], sizeof(float)*u.length());
	pek += sizeof(float)*u.length();

	memcpy(pek, (char*)&v[0], sizeof(float)*v.length());
	pek += sizeof(float)*u.length();

	memcpy(pek, (char*)&uvIds[0], sizeof(Index)*sMesh.uvIndexCount);
	pek += sizeof(Index)*sMesh.uvIndexCount;

	memcpy(pek, transform.name().asChar(), sMesh.nameLength);

	while (true)
	{
		try
		{
			if (producer->push(msg, length))
			{
				MGlobal::displayInfo("Sent the mesh to the circular buffer");
				break;
			}
		}
		catch (...)
		{
			Sleep(1);
		}
	}

	/*deleting the allocated variables*/
	//delete[] mVertex;
	//delete[] mIndex;
	return true;
}
bool createCamera(MObject &node)
{
	MFnCamera sCamera = MFnTransform(node).child(0);
	
	size_t length =
		sizeof(Matrix)
		+ sizeof(floatMatrix)
		+ sizeof(MainHeader);
	char * pek = msg;

	MainHeader mHead{ 1 };

	memcpy(pek, &mHead, sizeof(MainHeader));
	pek += sizeof(MainHeader);

	memcpy(pek, (char*)&MFnTransform(node).transformationMatrix(), sizeof(Matrix));
	pek += sizeof(Matrix);
	
	memcpy(pek, (char*)&sCamera.projectionMatrix(), sizeof(floatMatrix));

	while (true)
	{
		try
		{
			if (producer->push(msg, length))
			{
				break;
			}
		}
		catch (...)
		{
			Sleep(1);
		}
	}
	return true;
}
#pragma endregion
#pragma region Modified
#pragma endregion
EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus res = MS::kSuccess;

	MFnPlugin myPlugin(obj, "maya plinxxx", "1.0", "any", &res);
	if (MFAIL(res))
		CHECK_MSTATUS(res);

	MGlobal::displayInfo("Maya plugin loaded!");

	MStatus loopResults = MS::kSuccess;

	/*creating the circular buffer*/
	producer = new CircularBuffer(L"poop3", 10, true, 256);
	msg = new char[(10 * 1 << 10)/4];

	/*adding callback for matrix change in items that already exist*/
	MItDag meshIt(MItDag::kBreadthFirst, MFn::kTransform, &res);
	//bajs.transformationMatrix();
	//MItMeshPolygon
	for (; !meshIt.isDone(); meshIt.next())
	{
		MFnTransform trans = meshIt.currentItem();
		if (trans.child(0).hasFn(MFn::kCamera) || trans.child(0).hasFn(MFn::kMesh))
		{
			MDagPath meshDag = MDagPath::getAPathTo(trans.child(0));
			MCallbackId newId = MDagMessage::addWorldMatrixModifiedCallback(meshDag, WorldMatrixModified, NULL, &loopResults);
			if (loopResults == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
					MGlobal::displayInfo("added worldmatrix for " + trans.name());
				else
					MGlobal::displayInfo("failed to add worldmatrix callback");
			}
			else
			{
				MGlobal::displayInfo("failed to connect");
			}
			newId = MNodeMessage::addNameChangedCallback(trans.child(0), changedNameFunction, NULL, &loopResults);
			if (loopResults == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
				{
					MGlobal::displayInfo("Connected the NameChangeFunction");
				}
				else
					MGlobal::displayInfo("failed to connect NameChangeFunction");
			}
		}
		if (trans.child(0).hasFn(MFn::kMesh))
		{
			MCallbackId newId = MNodeMessage::addAttributeChangedCallback(trans.child(0), attributeChanged, NULL, &loopResults);

			if (loopResults == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
					MGlobal::displayInfo("made connection to the attributtes");
			}
			else
				MGlobal::displayInfo("failed to connect attributes");
			createMesh(meshIt.currentItem());
			//MItMeshPolygon( const MObject & polyObject, MStatus * ReturnStatus = NULL );
			//producer->push(trans.name().asChar(), trans.name().length());
		}
		if (trans.child(0).hasFn(MFn::kCamera))
		{
			if (MFnTransform(meshIt.currentItem()).name() == "persp")
			{
				createCamera(meshIt.currentItem());
			}
		}

		/*MDagPath meshDag = MDagPath::getAPathTo(meshIt.currentItem());
		MCallbackId newId = MDagMessage::addWorldMatrixModifiedCallback(meshDag, WorldMatrixModified, NULL, &loopResults);
		if (loopResults == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
				MGlobal::displayInfo("added worldmatrix");
			else
				MGlobal::displayInfo("failed to add worldmatrix callback");
		}
		else
		{
			MGlobal::displayInfo("failed to connect");
		}*/


			//bajs = myCallbackArray.append(MDagMessage::addWorldMatrixModifiedCallback(meshDag, WorldMatrixModified, NULL, NULL));
		//MFnTransform myMeshFn(meshIt.currentItem());
		//MGlobal::displayInfo(myMeshFn.name());
		/*if (meshIt.currentItem().hasFn(MFn::kMesh))
		{
			//MGlobal::displayInfo("mesh found!");
			//MFnMesh kuk(meshIt.currentItem());
			//MFn::kWorld
			if (MFAIL(bajs))
				MGlobal::displayInfo("failed to connest");
		} */
	}
	/*adding callback for time*/
	MCallbackId newId = MTimerMessage::addTimerCallback(5, elapsedTimeFunction, NULL, &loopResults);
	if (loopResults == MS::kSuccess)
	{
		if (myCallbackArray.append(newId) == MS::kSuccess);
		{
			MGlobal::displayInfo("timer made");
		}
	}
	else
		MGlobal::displayInfo("failed to create timer");

	/*newId = MNodeMessage::addNameChangedCallback(MObject::kNullObj, changedNameFunction, NULL, &loopResults);
	if (loopResults == MS::kSuccess)
	{
		if (myCallbackArray.append(newId) == MS::kSuccess)
		{
			MGlobal::displayInfo("Connected the NameChangeFunction");
		}
		else
			MGlobal::displayInfo("failed to connect NameChangeFunction");
	}*/

	/*adding callback for creating node*/
	newId = MDGMessage::addNodeAddedCallback(addedNodeFunction, kDefaultNodeType, NULL, &loopResults);
	if (loopResults == MS::kSuccess)
	{
		if (myCallbackArray.append(newId) == MS::kSuccess)
		{
			MGlobal::displayInfo("created addNodeCallback function");
		}
	}
	else
		MGlobal::displayInfo("Failed to create addNodeCallback function");

	return res;
}

/*MStatus 	remove (unsigned int index)
 	Remove the array element at the given index. More...*/

EXPORT MStatus uninitializePlugin(MObject obj)
{
	/*deleting the producer*/
	delete producer;

	delete msg;

	//free resources here
	MFnPlugin plugin(obj);

	MMessage::removeCallbacks(myCallbackArray);

	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}