/*Creator: David Wigelius*/
#include "maya_includes.h"
#include "structs.h"
#include <iostream>
#include "CircularBuffer.h"//<-------------------------- fix this later so that it's a lib

using namespace std;

CircularBuffer *producer;
MCallbackIdArray myCallbackArray;
float CurrentTime = 0; //kanske g�ra en pekare f�r att kunna kontrollera minne
float modifiedTime = 0;
//float DeltaTime = 0;
char * msg;

#pragma region callbacks
float getUpdateTime() { return 0.05; }
void WorldMatrixModified(MObject &transformNode, MDagMessage::MatrixModifiedFlags &modified, void *clientData)
{
	//asyncronous callback thread here to the elapsed time funtion, so that it will be called always to update
	//the current time at all times. MThreadAsync.
	if (modifiedTime > getUpdateTime())
	{
		//if (modified & MDagMessage::kTranslation)
		{
			MFnTransform trans = transformNode;

			MGlobal::displayInfo(trans.name() + " worldmatrix changed");
			MainHeader mHead{ 4 };
			Transformation mTransform{ trans.name().length() , 3 };

			/*this will vary*/
			size_t length =
				sizeof(MainHeader)
				+ sizeof(Transformation)
				+ mTransform.nameLength
				+ sizeof(Vector) * 2
				+ sizeof(Vector4);

			/*this will also vary*/
			Vector sTrans, sScale; double tempScale[3];
			Vector4 sRot; double tempRot[4];

			trans.getScale(tempScale);
			sScale = tempScale;

			trans.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
			sRot.x = tempRot[0];
			sRot.y = tempRot[1];
			sRot.z = tempRot[2];
			sRot.w = tempRot[3];

			sTrans = trans.getTranslation(MSpace::kTransform, NULL);

			char * pek = msg;
			memcpy(pek, (char*)&mHead, sizeof(MainHeader));
			pek += sizeof(MainHeader);

			memcpy(pek, (char*)&mTransform, sizeof(Transformation));
			pek += sizeof(Transformation);

			memcpy(pek, (char*)trans.name().asChar(), mTransform.nameLength);
			pek += mTransform.nameLength;

			memcpy(pek, (char*)&sScale, sizeof(Vector));
			pek += sizeof(Vector);

			memcpy(pek, (char*)&sRot, sizeof(Vector4));
			pek += sizeof(Vector4);

			memcpy(pek, (char*)&sTrans, sizeof(Vector));

			modifiedTime = 0;

			//while (true)
			{
				//try
				{
					if (producer->push(msg, length))
					{
						//break;
					}
				}
				//catch (...)
				{
					//Sleep(1);
				}
			}
		}
	}
	/*
	kScale         = kScaleX        | kScaleY        | kScaleZ,
	kRotation      = kRotateX       | kRotateY       | kRotateZ,
    kTranslation   = kTranslateX    | kTranslateY    | kTranslateZ,*/
}

void elapsedTimeFunction(float elapsedTime, float lastTime, void*clientData)
{
	//DeltaTime = elapsedTime - lastTime;
	modifiedTime += elapsedTime;
	CurrentTime = elapsedTime;
	//MGlobal::displayInfo((MString("Current time: ")+=(CurrentTime)));

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
	MIntArray indexList, offsetIdList, normalCount, uvCount, uvIds;
	MFloatPointArray points;
	MFloatArray u, v;
	MFloatVectorArray normals;
	mMesh.getPoints(points, MSpace::kObject);
	mMesh.getTriangles(offsetIdList, indexList);

	/*assigning the main header to creation mode*/
	MainHeader sHeader{ 0 };
	MString kissaner = "";
	for (int i = 0; i < indexList.length(); i++)
	{
		kissaner += indexList[i];
		kissaner += ", ";
		//MGlobal::displayInfo(indexList[i] + "\n");
	}
	MGlobal::displayInfo(kissaner);
	kissaner = "";
	
	/*Creating the headers to send*/
	CreateMesh sMesh;
	sMesh.vertexCount = points.length();
	sMesh.indexCount = indexList.length();
	sMesh.nameLength = transform.name().length();
	mMesh.getTriangleOffsets(normalCount, offsetIdList);
	//mMesh.getVertices

	/*getting the position of the mesh*/
	Vector sTran, sScal;
	Vector4 sRot;
	double tempScale[3], tempRot[4];

	sTran = transform.getTranslation(MSpace::kTransform, NULL);
	transform.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
	sRot.x = (float)tempRot[0];
	sRot.y = (float)tempRot[1];
	sRot.z = (float)tempRot[2];
	sRot.w = (float)tempRot[3];

	transform.getScale(tempScale);
	sScal = tempScale;

	/*the normals*/
	MIntArray test, normalId;
	mMesh.getNormalIds(test, normalId);
	mMesh.getNormals(normals, MSpace::kObject);
	sMesh.normalCount = normals.length();

	kissaner = sMesh.normalCount;
	kissaner += ", ";
	kissaner += sMesh.indexCount;
	kissaner += ", nr of normalids: ";
	kissaner += normalId.length();
	kissaner += ", nr of offsetids: ";
	kissaner += offsetIdList.length();
	MGlobal::displayInfo(kissaner);
	

	kissaner = "";
	for (int i = 0; i < sMesh.indexCount; i++)
	{
		kissaner += i;
		kissaner += ": ";
		kissaner += normalId[indexList[i]];
		kissaner += ", ";
		//kissaner += offsetIdList[indexList[i]];
		kissaner += indexList[offsetIdList[i]];
		kissaner += "\n";
	}
	MGlobal::displayInfo(kissaner);
	kissaner = "";
	/*for (int i = 0; i < test.length(); i++)
	{
		kissaner += test[i];
		kissaner += ", ";
	}
	MGlobal::displayInfo(kissaner);
	kissaner = "";*/
	/*for (int i = 0; i < test1.length(); i++)
	{
		kissaner += test1[i];
		kissaner += ", ";
	}*/

	/*the uvs*/
	mMesh.getUVs(u, v);
	mMesh.getAssignedUVs(uvCount, uvIds);
	sMesh.uvIndexCount = uvIds.length();
	sMesh.uvCount = u.length();

	/*Calculating the length of the message and sending the creation info to the circular buffer*/
	//int length = (sizeof(Vertex) * points.length())
	//	+ (sizeof(Index) * indexList.length())
	//	+ (sizeof(Normals) * normals.length())
	//	+ (sizeof(Index) * offsetIdList.length())
	//	+ (sizeof(Index) * normalId.length())
	//	+ sizeof(CreateMesh)
	//	+ sizeof(MainHeader)
	//	+ sizeof(Vector)*2
	//	+ sizeof(Vector4)
	//	+ (sizeof(float) * u.length())*2
	//	+ (sizeof(Index)*sMesh.uvIndexCount)
	//	+ sMesh.nameLength;

	//sMesh.normalIndexCount = normalId.length();
	//
	///*constructing the message*/
	//char * pek = msg;

	//memcpy(pek, (char*)&sHeader, sizeof(MainHeader));
	//pek += sizeof(MainHeader);

	//memcpy(pek, (char*)&sMesh, sizeof(CreateMesh));
	//pek += sizeof(CreateMesh);

	//memcpy(pek, transform.name().asChar(), sMesh.nameLength);
	//pek += sMesh.nameLength;

	//memcpy(pek, (char*)&sScal, sizeof(Vector));
	//pek += sizeof(Vector);

	//memcpy(pek, (char*)&sRot, sizeof(Vector4));
	//pek += sizeof(Vector4);

	//memcpy(pek, (char*)&sTran, sizeof(Vector));
	//pek += sizeof(Vector);

	//memcpy(pek, (char*)mMesh.getRawPoints(NULL), (sizeof(Vertex)*sMesh.vertexCount));
	//pek += sizeof(Vertex)*sMesh.vertexCount;

	//memcpy(pek, (char*)&indexList[0], (sizeof(Index)*sMesh.indexCount));
	//pek += sizeof(Index)*sMesh.indexCount;

	//memcpy(pek, (char*)mMesh.getRawNormals(NULL), (sizeof(Normals)*sMesh.normalCount));
	//pek += sizeof(Normals)*sMesh.normalCount;

	//memcpy(pek, (char*)&normalId[0], (sizeof(Index) * sMesh.normalIndexCount));
	//pek += sizeof(Index)*sMesh.normalIndexCount;

	//memcpy(pek, (char*)&offsetIdList[0], (sizeof(Index)*sMesh.indexCount));
	//pek += sizeof(Index)*sMesh.indexCount;

	//memcpy(pek, (char*)&u[0], sizeof(float)*u.length());
	//pek += sizeof(float)*u.length();

	//memcpy(pek, (char*)&v[0], sizeof(float)*v.length());
	//pek += sizeof(float)*u.length();

	//memcpy(pek, (char*)&uvIds[0], sizeof(Index)*sMesh.uvIndexCount);


	/*while (true)
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
	}*/

	return true;
}
//bool createCamera(MObject &node)
//{
//	MFnCamera sCamera = MFnTransform(node).child(0);
//	MFnTransform transform = node;
//	CreateCamera mCam{ transform.name().length() };
//
//	Vector sTrans;
//	sTrans = transform.getTranslation(MSpace::kTransform);
//	double tempRot[4]; Vector4 sRot;
//	transform.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
//	sRot.x = tempRot[0];
//	sRot.y = tempRot[1];
//	sRot.z = tempRot[2];
//	sRot.w = tempRot[3];
//
//	size_t length =
//		sizeof(CreateCamera)
//		+ sizeof(floatMatrix)
//		+ sizeof(MainHeader)
//		+ sizeof(Vector4)
//		+ sizeof(Vector)
//		+ mCam.nameLength;
//	char * pek = msg;
//
//	MainHeader mHead{ 1 };
//
//	memcpy(pek, &mHead, sizeof(MainHeader));
//	pek += sizeof(MainHeader);
//
//	memcpy(pek, &mCam, sizeof(CreateCamera));
//	pek += sizeof(CreateCamera);
//
//	memcpy(pek, (char*)transform.name().asChar(), mCam.nameLength);
//	pek += mCam.nameLength;
//
//	/*memcpy(pek, (char*)&MFnTransform(node).transformationMatrix(), sizeof(Matrix));
//	pek += sizeof(Matrix);*/
//	
//	memcpy(pek, (char*)&sCamera.projectionMatrix(), sizeof(floatMatrix));
//	pek += sizeof(floatMatrix);
//
//	memcpy(pek, (char*)&sRot, sizeof(Vector4));
//	pek += sizeof(Vector4);
//
//	memcpy(pek, (char*)&sTrans, sizeof(Vector));
//
//	while (true)
//	{
//		try
//		{
//			if (producer->push(msg, length))
//			{
//				break;
//			}
//		}
//		catch (...)
//		{
//			Sleep(1);
//		}
//	}
//	return true;
//}
bool createViewportCamera()
{
	M3dView derp = derp.active3dView();
	MDagPath camPath;
	MStatus rs;

	rs = derp.getCamera(camPath);
	MFnCamera sCamera(camPath);

	MFnTransform transform = sCamera.parent(0);
	//MFnTransform ttransform = sCamera.parent(sCamera.parentCount());


	CreateCamera mCam{ transform.name().length() };

	if (sCamera.isOrtho(&rs))
	{
		return false;
	}
	else
	{
		Vector sTrans;
		sTrans = transform.getTranslation(MSpace::kTransform);
		double tempRot[4]; Vector4 sRot;
		transform.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
		sRot.x = tempRot[0];
		sRot.y = tempRot[1];
		sRot.z = tempRot[2];
		sRot.w = tempRot[3];

		size_t length =
			sizeof(CreateCamera)
			+ sizeof(floatMatrix)
			+ sizeof(MainHeader)
			+ sizeof(Vector4)
			+ sizeof(Vector)
			+ mCam.nameLength;
		char * pek = msg;

		MainHeader mHead{ 1 };

		memcpy(pek, &mHead, sizeof(MainHeader));
		pek += sizeof(MainHeader);

		memcpy(pek, &mCam, sizeof(CreateCamera));
		pek += sizeof(CreateCamera);

		memcpy(pek, (char*)transform.name().asChar(), mCam.nameLength);
		pek += mCam.nameLength;

		/*memcpy(pek, (char*)&MFnTransform(node).transformationMatrix(), sizeof(Matrix));
		pek += sizeof(Matrix);*/

		memcpy(pek, (char*)&sCamera.projectionMatrix(), sizeof(floatMatrix));
		pek += sizeof(floatMatrix);

		memcpy(pek, (char*)&sRot, sizeof(Vector4));
		pek += sizeof(Vector4);

		memcpy(pek, (char*)&sTrans, sizeof(Vector));

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
	producer = new CircularBuffer(L"poop3", 100, true, 256);
	msg = new char[(100 * (1 << 10))/4];

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
				//createCamera(meshIt.currentItem());
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
	}	createViewportCamera();
	/*adding callback for time*/
	MCallbackId newId = MTimerMessage::addTimerCallback(0.05, elapsedTimeFunction, NULL, &loopResults);
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