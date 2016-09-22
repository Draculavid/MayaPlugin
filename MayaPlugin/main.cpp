#include "maya_includes.h"
#include <iostream>

using namespace std;

MCallbackIdArray myCallbackArray;
float CurrentTime = 0; //kanske göra en pekare för att kunna kontrollera minne

void WorldMatrixModified(MObject &transformNode, MDagMessage::MatrixModifiedFlags &modified, void *clientData)
{
	MGlobal::displayInfo(MFnTransform(transformNode).name() + " worldmatrix changed");
}

void elapsedTimeFunction(float elapsedTime, float lastTime, void*clientData)
{
	CurrentTime += elapsedTime;
	MGlobal::displayInfo((MString("Current time: ")+=(CurrentTime)));
}

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
	}
}
//attrubute change

void changedNameFunction(MObject &node, const MString &str, void*clientData)
{
	MGlobal::displayInfo("name changed, new name: " + MFnDagNode(node).name());
}

EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus res = MS::kSuccess;

	MFnPlugin myPlugin(obj, "maya plinxxx", "1.0", "any", &res);
	if (MFAIL(res))
		CHECK_MSTATUS(res);

	MGlobal::displayInfo("Maya plugin loaded!");

	MStatus loopResults = MS::kSuccess;

	/*adding callback for matrix change in items that already exist*/
	MItDag meshIt(MItDag::kBreadthFirst, MFn::kTransform, &res);
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
			/*newId = MNodeMessage::addNameChangedCallback(trans.child(0), changedNameFunction, NULL, &loopResults);
			if (loopResults == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
				{
					MGlobal::displayInfo("Connected the NameChangeFunction");
				}
				else
					MGlobal::displayInfo("failed to connect NameChangeFunction");
			}*/
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

	newId = MNodeMessage::addNameChangedCallback(MObject::kNullObj, changedNameFunction, NULL, &loopResults);
	if (loopResults == MS::kSuccess)
	{
		if (myCallbackArray.append(newId) == MS::kSuccess)
		{
			MGlobal::displayInfo("Connected the NameChangeFunction");
		}
		else
			MGlobal::displayInfo("failed to connect NameChangeFunction");
	}

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
	MFnPlugin plugin(obj);

	//free resources here
	MMessage::removeCallbacks(myCallbackArray);

	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}