/*Creator: David Wigelius*/
#include "maya_includes.h"
#include <iostream>
#include "../../shared/shared/CircularBuffer.h" //<-------------------------- fix this later so that it's a lib

using namespace std;

CircularBuffer *producer;
MCallbackIdArray myCallbackArray;
float CurrentTime = 0; //kanske göra en pekare för att kunna kontrollera minne
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
	
	MFnMesh mMesh(node, NULL);
	MIntArray indexList, vertexList;
	MFloatPointArray points;
	mMesh.getPoints(points, MSpace::kObject);
	mMesh.getTriangles(vertexList, indexList);
	/*assigning the main header to creation mode*/
	CircularBuffer::MainHeader sHeader{ 0 };

	//mMesh.getPoints()
	/*Assigning the type to mesh*/
	CircularBuffer::TypeHeader sType{ 0 };

	/*Creating the headers to send*/
	CircularBuffer::CreateMesh sMesh;

	sMesh.vertexCount = points.length();
	sMesh.topologyCount = indexList.length();

	CircularBuffer::Vertex *mVertex = new CircularBuffer::Vertex[points.length()];
	CircularBuffer::topology *mTopology = new CircularBuffer::topology[indexList.length()];

	/*putting references to the points (vertex points)*/
	MString bajs;
	for (int i = 0; i < points.length(); i++)
	{
		mVertex[i].x = points[i].x;
		mVertex[i].y = points[i].y;
		mVertex[i].z = points[i].z;
		bajs += mVertex[i].x;
		bajs += ", ";
		bajs += mVertex[i].y;
		bajs += ", ";
		bajs += mVertex[i].z;
		bajs += "\n";
	}
	MGlobal::displayInfo(bajs);

	/*mVertex->x = points[0].x;
	mVertex->y = points[0].y;
	mVertex->z = points[0].z;*/

	/*putting reference to the indexes of the triangles*/
	for (int i = 0; i < indexList.length(); i++)
	{
		mTopology[i].nr = indexList[i];
	}

	/*Calculating the length of the message and sending the creation info to the circular buffer*/
	int length = (sizeof(CircularBuffer::Vertex) * points.length())
		+ (sizeof(CircularBuffer::topology) * indexList.length())
		+ sizeof(CircularBuffer::CreateMesh)
		+ sizeof(CircularBuffer::MainHeader)
		+ sizeof(CircularBuffer::TypeHeader);

	MString info;
	info += length;
	MGlobal::displayInfo(info);

	/*while (true)
	{
		try
		{
			if (producer->pushMesh(sMesh, *mVertex, *mTopology, length))
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

	/*deleting the allocated variables*/
	delete mVertex;
	delete mTopology;

	//mTopology = new CircularBuffer::topology[mMesh.topologyCount];
	/*Iterating over all attributes of the mesh*/
	//MItMeshPolygon polyIt(node, NULL);
	//int bajs = polyIt.count(); //number of faces
	//polyIt.polygonVertexCount();
	//kukirov += bajs;
		//MGlobal::displayInfo(kukirov);
	//MString info;


	//ballefjong.getVertices(vertexCount, vertexList);

	/*To get the vertex points of the mesh*/
	//info += points.length();
	//MGlobal::displayInfo(info);
	//info = "";

	//info += ballefjong.numPolygons(NULL);
	//MGlobal::displayInfo(info);
	//info = "";

	/*info += ballefjong.numFaceVertices();
	MGlobal::displayInfo(info);
	info = "";*/

	//MGlobal::displayInfo(info);
	//info = "";

	//topology[ballefjong.numPolygons(NULL) *polyIt.numTriangles(bajs)]
	/*MGlobal::displayInfo("vertex count: ");
	for (int i = 0; i < vertexCount.length(); i++)
	{
		info += vertexCount[i];
		info += "; ";
	}

	MGlobal::displayInfo(info);
	info = "";

	MGlobal::displayInfo("vertex count: ");
	for (int i = 0; i < vertexList.length(); i++)
	{
		info += vertexList[i];
		info += "; ";
	}

	MGlobal::displayInfo(info);
	info = "";*/

	//polyIt.numTriangles(bajs);
	//info += bajs;

	//MGlobal::displayInfo(info);

	/*loops to get the points of the current face*/
	/*for (unsigned int i = 0; i < fitta.length(); i++)
	{
		kukirov += fitta[i].x;
		kukirov += ", ";
		kukirov += fitta[i].y;
		kukirov += ", ";
		kukirov += fitta[i].z;
		kukirov += "; ";
	}
	MGlobal::displayInfo(kukirov);
	kukirov = "";*/

	/*for topology*/
	/*for (; !polyIt.isDone(); polyIt.next())
	{
		polyIt.getTriangles(points, indexList, MSpace::kWorld);
		for (int i = 0; i < indexList.length(); i++)
		{
			info += indexList[i];
			info += "; ";
		}
		MGlobal::displayInfo(info);
		info = "";
	}*/
	////for (; !polyIt.isDone(); polyIt.next())
	//{
	//	polyIt.getVertices(balle);
	//	for (int i = 0; i < balle.length(); i++)
	//	{
	//		kukirov += balle[i];
	//	}

	//	
	//	//kukirov += bajs;

	//	kukirov = "";
	//}
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
			createMesh(trans.child(0));
			//MItMeshPolygon( const MObject & polyObject, MStatus * ReturnStatus = NULL );
			//producer->push(trans.name().asChar(), trans.name().length());
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


	//free resources here
	MFnPlugin plugin(obj);

	MMessage::removeCallbacks(myCallbackArray);

	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}