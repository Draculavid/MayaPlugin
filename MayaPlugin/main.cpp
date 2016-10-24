/*Creator: David Wigelius*/
#include "maya_includes.h"
#include "structs.h"
#include <iostream>
#include "CircularBuffer.h"//<-------------------------- fix this later so that it's a lib
#include <queue>

using namespace std;
#define UPDATE_TIME 0.5f
#define QUEUE_VALUE 20

CircularBuffer *producer;
MCallbackIdArray myCallbackArray;
float CurrentTime = 0; //kanske göra en pekare för att kunna kontrollera minne
bool cameraMovement = false;
bool meshTopChanged = false;
MObject modelQueue[QUEUE_VALUE];
MString lastCamera = "";
unsigned int nrInQueue = 0;
char * msg;
//std::queue <MObject>modelQueue;

void popQueue(int * position)
{
	modelQueue[*position] = MObject::kNullObj;
	nrInQueue--;

	//MObject * temp;
	//temp = &modelQueue[*position];
	modelQueue[*position] = modelQueue[nrInQueue];
	modelQueue[nrInQueue] = MObject::kNullObj;

}
bool appendQueue(MObject & node)
{
	if (nrInQueue <= QUEUE_VALUE)
	{
		modelQueue[nrInQueue] = node;
		nrInQueue++;
		return true;
	}
	return false;
}
#pragma region callbacks
//void WorldMatrixModified(MObject &transformNode, MDagMessage::MatrixModifiedFlags &modified, void *clientData)
//{
//	if (MFnTransform(transformNode).child(0).apiType() == MFn::kCamera)
//		cameraMovement = true;
//	 	//if (modifiedTime > UPDATE_TIME) //fix so it's only mesh 
//	{
//			/*	if (!transformNode.hasFn(MFn::kCamera))*/
//		//if (modified & MDagMessage::kTranslation) 
//		if (MFnTransform(transformNode).child(0).apiType() == MFn::kMesh)
//		{
//			//M3dView kiss; 
//			//kiss.active3dView().updateViewingParameters(); 
//			MFnTransform trans = transformNode;
//			MGlobal::displayInfo(trans.name() + " worldmatrix changed");
//			MainHeader mHead{ 4 };
//		
//
//				/*MDagMessage
//				60 			kScale         = kScaleX        | kScaleY        | kScaleZ,
//				61 			kRotation      = kRotateX       | kRotateY       | kRotateZ,
//				62 			kTranslation   = kTranslateX    | kTranslateY    | kTranslateZ,*/
//		
//
//			if (modified & MDagMessage::kScale)
//			{
//				Transformation mTransform{ trans.name().length() , 0 };
//		
//
//		 				/*this will vary*/
//		 		size_t length =
//		 			sizeof(MainHeader)
//					+ sizeof(Transformation)
//					+ mTransform.nameLength
//					+ sizeof(Vector);
//		
//
//		 				/*this will also vary*/
//		 		Vector sScale; double tempScale[3];
//		
//
//		 		trans.getScale(tempScale);
//				sScale = tempScale;
//		
//
//		 		char * pek = msg;
//				memcpy(pek, (char*)&mHead, sizeof(MainHeader));
//				pek += sizeof(MainHeader);
//		
//
//		 		memcpy(pek, (char*)&mTransform, sizeof(Transformation));
//				pek += sizeof(Transformation);
//		
//
//		 		memcpy(pek, (char*)trans.name().asChar(), mTransform.nameLength);
//				pek += mTransform.nameLength;
//		
//
//		 		memcpy(pek, (char*)&sScale, sizeof(Vector));
//				pek += sizeof(Vector);
//		
//
//		 		//modifiedTime = 0; 
//
//		 		//while (true) 
//		 		{
//		 			//try 
//					{
//						if (producer->push(msg, length))
//							{
//								//break; 
//							}
//					}
//				//catch (...) 
//					{
//						//Sleep(1); 
//					}
//				}
//			}
//			else if (modified & MDagMessage::kRotation)
//			{
//				Transformation mTransform{ trans.name().length() , 1 };
//		
//
//		 		/*this will vary*/
//		 		size_t length =
//		 			sizeof(MainHeader)
//					+ sizeof(Transformation)
//					+ mTransform.nameLength
//					+ sizeof(Vector4);
//		
//
//		 		/*this will also vary*/
//		 		double tempRot[4]; Vector4 sRot;
//		
//
//		 		trans.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
//				sRot.x = (float)tempRot[0];
//				sRot.y = (float)tempRot[1];
//				sRot.z = (float)tempRot[2];
//				sRot.w = (float)tempRot[3];
//		
//
//		 		char * pek = msg;
//				memcpy(pek, (char*)&mHead, sizeof(MainHeader));
//				pek += sizeof(MainHeader);
//		
//
//		 		memcpy(pek, (char*)&mTransform, sizeof(Transformation));
//				pek += sizeof(Transformation);
//		
//
//		 		memcpy(pek, (char*)trans.name().asChar(), mTransform.nameLength);
//				pek += mTransform.nameLength;
//		
//
//		 		memcpy(pek, (char*)&sRot, sizeof(Vector4));
//				pek += sizeof(Vector);
//		
//
//		 		//modifiedTime = 0; 
//		
//
//		 		producer->push(msg, length);
//			}
//			else if (modified & MDagMessage::kTranslation)
//			{
//				Transformation mTransform{ trans.name().length() , 2 };
//		
//
//
//		 				/*this will vary*/
//		 		size_t length =
//		 		sizeof(MainHeader)
//				+ sizeof(Transformation)
//				+ mTransform.nameLength
//				+ sizeof(Vector);
//		
//
//		 		/*this will also vary*/
//		 		Vector sTran;
//				sTran = trans.getTranslation(MSpace::kTransform, NULL);
//		
//				MString info;
//				info += trans.name();
//				info += ": ";
//				info += sTran.x;
//				info += ", ";
//				info += sTran.y;
//				info += ", ";
//				info += sTran.z;
//				info += "\n";
//				MGlobal::displayInfo(info);
//
//		 		char * pek = msg;
//				memcpy(pek, (char*)&mHead, sizeof(MainHeader));
//				pek += sizeof(MainHeader);
//		
//
//		 		memcpy(pek, (char*)&mTransform, sizeof(Transformation));
//				pek += sizeof(Transformation);
//		
//
//				memcpy(pek, (char*)trans.name().asChar(), mTransform.nameLength);
//				pek += mTransform.nameLength;
//		
//
//				memcpy(pek, (char*)&sTran, sizeof(Vector));
//				pek += sizeof(Vector);
//		
//
//				//modifiedTime = 0; 
//		
//
//				producer->push(msg, length);
//			}
//		}
//	}
//}

void WorldMatrixModified(MObject &transformNode, MDagMessage::MatrixModifiedFlags &modified, void *clientData)
{
	if (MFnTransform(transformNode).child(0).apiType() == MFn::kCamera)
		cameraMovement = true;
	//if (modifiedTime > UPDATE_TIME) //fix so it's only mesh
	{
	/*	if (!transformNode.hasFn(MFn::kCamera))*/
		//if (modified & MDagMessage::kTranslation)
		if (MFnTransform(transformNode).child(0).apiType() == MFn::kMesh)
		{
			//MString listName;
			MSelectionList sList;
			MGlobal::getActiveSelectionList(sList);
			MItSelectionList iter(sList);
			MObject mNode;
			iter.getDependNode(mNode);
			//MDagPath nodePath;
			MObject mNode2;
			//MGlobal::displayInfo(mNode.apiTypeStr());
			//MGlobal::displayInfo(mDep.name());
			if (mNode.apiType() != MFn::kTransform)
			{
				MFnDependencyNode mDep(mNode);
				MPlugArray mPlugA;
				MPlug mPlug;
				mPlug = mDep.findPlug("out");
				bool depTree = mPlug.connectedTo(mPlugA, false, true, NULL);
				if (depTree && mPlugA.length() > 0)
					mNode2 = mPlugA[0].node();
			}
			//MItDependencyGraph MltIter(mNode, MFn::kMesh, MItDependencyGraph::kUpstream, MItDependencyGraph::kDepthFirst
			//, MItDependencyGraph::kNodeLevel, NULL);
			//MltIter.disablePruningOnFilter();
			/*mNode2 = MltIter.currentItem();
			for (; !MltIter.isDone(); MltIter.next())
			{
				mNode2 = MltIter.currentItem();
				if (mNode2 == transformNode)
					break;
			}*/

			if (mNode == transformNode || mNode2 == MFnTransform(transformNode).child(0))
			{
				//MGlobal::displayInfo(listName);
				//MString info;



				//M3dView kiss;
				//kiss.active3dView().updateViewingParameters();

				//MGlobal::displayInfo(trans.name() + " worldmatrix changed");
				MainHeader mHead{ 4 };
				/*MDagMessage
				kScale         = kScaleX        | kScaleY        | kScaleZ,
				kRotation      = kRotateX       | kRotateY       | kRotateZ,
				kTranslation   = kTranslateX    | kTranslateY    | kTranslateZ,*/

				if (modified & MDagMessage::kScale)
				{
					char * pek = msg;
					memcpy(pek, (char*)&mHead, sizeof(MainHeader));
					pek += sizeof(MainHeader);

					Transformation mTransform{ sList.length() , 0 };
					size_t length = 0;
					unsigned int nameLength = 0;

					memcpy(pek, (char*)&mTransform, sizeof(Transformation));
					pek += sizeof(Transformation);


					if (mNode.apiType() == MFn::kTransform)
					{
						for (; !iter.isDone(); iter.next())
						{

							iter.getDependNode(mNode);
							MFnTransform trans = mNode;
							//MString ballefjong;
							//ballefjong += trans.name();
							//MGlobal::displayInfo(ballefjong);

							//sList.getDagPath(i, nodePath);
							nameLength = trans.name().length();

							memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
							pek += sizeof(unsigned int);
							length += sizeof(unsigned int);

							memcpy(pek, trans.name().asChar(), nameLength);
							pek += nameLength;
							length += nameLength;


							Vector sScale; double tempScale[3];

							trans.getScale(tempScale);
							sScale = tempScale;

							memcpy(pek, (char*)&sScale, sizeof(Vector));
							pek += sizeof(Vector);
							length += sizeof(Vector);
						}
					}
					else
					{
						for (; !iter.isDone(); iter.next())
						{

							MFnTransform trans = MFnMesh(mNode2).parent(0);
							iter.getDependNode(MFnMesh(mNode2).parent(0));
							//MString ballefjong;
							//ballefjong += trans.name();
							//MGlobal::displayInfo(ballefjong);

							//sList.getDagPath(i, nodePath);
							nameLength = trans.name().length();

							memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
							pek += sizeof(unsigned int);
							length += sizeof(unsigned int);

							memcpy(pek, trans.name().asChar(), nameLength);
							pek += nameLength;
							length += nameLength;


							Vector sScale; double tempScale[3];

							trans.getScale(tempScale);
							sScale = tempScale;

							memcpy(pek, (char*)&sScale, sizeof(Vector));
							pek += sizeof(Vector);
							length += sizeof(Vector);
						}
					}


					length +=
						sizeof(MainHeader)
						+ sizeof(Transformation);
					//+ sizeof(Vector);

				/*this will also vary*/

					producer->push(msg, length);

				}
				else if (modified & MDagMessage::kRotation)
				{
					//Transformation mTransform{ trans.name().length() , 1 };

					///*this will vary*/
					//size_t length =
					//	sizeof(MainHeader)
					//	+ sizeof(Transformation)
					//	+ mTransform.nameLength
					//	+ sizeof(Vector4);
					char * pek = msg;
					memcpy(pek, (char*)&mHead, sizeof(MainHeader));
					pek += sizeof(MainHeader);

					Transformation mTransform{ sList.length() , 1 };
					size_t length = 0;
					unsigned int nameLength = 0;

					memcpy(pek, (char*)&mTransform, sizeof(Transformation));
					pek += sizeof(Transformation);

					//double tempRot[4]; Vector4 sRot;

					////trans.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
					//sRot.x = (float)tempRot[0];
					//sRot.y = (float)tempRot[1];
					//sRot.z = (float)tempRot[2];
					//sRot.w = (float)tempRot[3];

					/*memcpy(pek, (char*)&sRot, sizeof(Vector4));
					pek += sizeof(Vector);*/
					if (mNode.apiType() == MFn::kTransform)
					{
						for (; !iter.isDone(); iter.next())
						{
							//MObject mNode;
							iter.getDependNode(mNode);
							MFnTransform trans = mNode;

							//sList.getDagPath(i, nodePath);
							nameLength = trans.name().length();

							memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
							pek += sizeof(unsigned int);
							length += sizeof(unsigned int);

							memcpy(pek, trans.name().asChar(), nameLength);
							pek += nameLength;
							length += nameLength;

							double tempRot[4]; Vector4 sRot;

							trans.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
							sRot.x = (float)tempRot[0];
							sRot.y = (float)tempRot[1];
							sRot.z = (float)tempRot[2];
							sRot.w = (float)tempRot[3];

							memcpy(pek, (char*)&sRot, sizeof(Vector4));
							pek += sizeof(Vector4);
							length += sizeof(Vector4);
						}
					}
					else
					{
						for (; !iter.isDone(); iter.next())
						{
							//MObject mNode;
							iter.getDependNode(MFnMesh(mNode2).parent(0));
							MFnTransform trans = MFnMesh(mNode2).parent(0);

							//sList.getDagPath(i, nodePath);
							nameLength = trans.name().length();

							memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
							pek += sizeof(unsigned int);
							length += sizeof(unsigned int);

							memcpy(pek, trans.name().asChar(), nameLength);
							pek += nameLength;
							length += nameLength;

							double tempRot[4]; Vector4 sRot;

							trans.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
							sRot.x = (float)tempRot[0];
							sRot.y = (float)tempRot[1];
							sRot.z = (float)tempRot[2];
							sRot.w = (float)tempRot[3];

							memcpy(pek, (char*)&sRot, sizeof(Vector4));
							pek += sizeof(Vector4);
							length += sizeof(Vector4);
						}
					}


					length +=
						sizeof(MainHeader)
						+ sizeof(Transformation);
					//+ sizeof(Vector4);
				/*this will also vary*/
				//char * pek = msg;
				//memcpy(pek, (char*)&mHead, sizeof(MainHeader));
				//pek += sizeof(MainHeader);

				//memcpy(pek, (char*)&mTransform, sizeof(Transformation));
				//pek += sizeof(Transformation);

				//memcpy(pek, (char*)trans.name().asChar(), mTransform.nameLength);
				//pek += mTransform.nameLength;


				//modifiedTime = 0;

					producer->push(msg, length);
				}
				else if (modified & MDagMessage::kTranslation)
				{
					//Transformation mTransform{ trans.name().length() , 2 };

					///*this will vary*/
					//size_t length =
					//	sizeof(MainHeader)
					//	+ sizeof(Transformation)
					//	+ mTransform.nameLength
					//	+ sizeof(Vector);

					char * pek = msg;
					memcpy(pek, (char*)&mHead, sizeof(MainHeader));
					pek += sizeof(MainHeader);

					Transformation mTransform{ sList.length() , 2 };
					size_t length = 0;
					unsigned int nameLength = 0;

					memcpy(pek, (char*)&mTransform, sizeof(Transformation));
					pek += sizeof(Transformation);



					/*memcpy(pek, (char*)&sTran, sizeof(Vector));
					pek += sizeof(Vector);*/
					//MTransformationMatrix test = trans.transformation();
					//MString kuken;
					if (mNode.apiType() == MFn::kTransform)
					{
						for (; !iter.isDone(); iter.next())
						{
							//MObject mNode;
							iter.getDependNode(mNode);
							MFnTransform trans = mNode;

							nameLength = trans.name().length();

							memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
							pek += sizeof(unsigned int);
							length += sizeof(unsigned int);

							memcpy(pek, trans.name().asChar(), nameLength);
							pek += nameLength;
							length += nameLength;

							Vector sTran;
							sTran = trans.getTranslation(MSpace::kTransform, NULL);

							memcpy(pek, (char*)&sTran, sizeof(Vector));
							pek += sizeof(Vector);
							length += sizeof(Vector);
						}
					}
					else
					{
						for (; !iter.isDone(); iter.next())
						{
							//MObject mNode;
							iter.getDependNode(MFnMesh(mNode2).parent(0));
							MFnTransform trans = MFnMesh(mNode2).parent(0);

							nameLength = trans.name().length();

							memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
							pek += sizeof(unsigned int);
							length += sizeof(unsigned int);

							memcpy(pek, trans.name().asChar(), nameLength);
							pek += nameLength;
							length += nameLength;

							Vector sTran;
							sTran = trans.getTranslation(MSpace::kTransform, NULL);

							memcpy(pek, (char*)&sTran, sizeof(Vector));
							pek += sizeof(Vector);
							length += sizeof(Vector);
						}
					}
					//MGlobal::displayInfo(kuken);

					length +=
						sizeof(MainHeader)
						+ sizeof(Transformation);
					//+ sizeof(Vector);

				/*this will also vary*/
				//char * pek = msg;
				//memcpy(pek, (char*)&mHead, sizeof(MainHeader));
				//pek += sizeof(MainHeader);

				//memcpy(pek, (char*)&mTransform, sizeof(Transformation));
				//pek += sizeof(Transformation);

				//memcpy(pek, (char*)trans.name().asChar(), mTransform.nameLength);
				//pek += mTransform.nameLength;


				//modifiedTime = 0;

					producer->push(msg, length);
				}
			}
		}
	}
	
}

void preRenderCB(const MString& panelName, void * data)
{
	if (lastCamera == "")
		lastCamera = panelName;
	//lastCamera = &panelName;
	//MGlobal::displayInfo("changed somethingaaskfnalvnevoåiaernv");
	if (cameraMovement || lastCamera != panelName)
	{
		if (lastCamera != panelName)
			lastCamera = panelName;
		MGlobal::displayInfo(panelName);
		M3dView mCam = mCam.active3dView();
		MDagPath camPath;
		MStatus rs;

		rs = mCam.getCamera(camPath);
		MFnCamera sCamera(camPath);

		MFnTransform transform = sCamera.parent(0);
		MainHeader mHead{ 8 };

		bool isOrtho = sCamera.isOrtho();
		if (isOrtho)
		{
			size_t length =
				sizeof(MainHeader)
				+ sizeof(bool)
				+ sizeof(Vector)
				+ sizeof(float)
				+ sizeof(Vector4);

			
			
			float orthoWidth = sCamera.orthoWidth();
			Vector sTrans;
			Vector4 sRot; double tempRot[4];
			sTrans = transform.getTranslation(MSpace::kTransform, NULL);
			transform.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
			sRot.x = tempRot[0];
			sRot.y = tempRot[1];
			sRot.z = tempRot[2];
			sRot.w = tempRot[3];

			char * pek = msg;
			memcpy(pek, (char*)&mHead, sizeof(MainHeader));
			pek += sizeof(MainHeader);

			memcpy(pek, (char*)&isOrtho, sizeof(bool));
			pek += sizeof(bool);

			memcpy(pek, (char*)&sRot, sizeof(Vector4));
			pek += sizeof(Vector4);

			memcpy(pek, (char*)&sTrans, sizeof(Vector));
			pek += sizeof(Vector);

			memcpy(pek, (char*)&orthoWidth, sizeof(float));

			cameraMovement = false;
			producer->push(msg, length);
		}
		else
		{
			size_t length =
				sizeof(MainHeader)
				+ sizeof(bool)
				+ sizeof(Vector)
				+ sizeof(Vector4);

			/*this will also vary*/
			Vector sTrans;
			Vector4 sRot; double tempRot[4];


			transform.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
			sRot.x = tempRot[0];
			sRot.y = tempRot[1];
			sRot.z = tempRot[2];
			sRot.w = tempRot[3];

			sTrans = transform.getTranslation(MSpace::kTransform, NULL);

			char * pek = msg;
			memcpy(pek, (char*)&mHead, sizeof(MainHeader));
			pek += sizeof(MainHeader);

			memcpy(pek, (char*)&isOrtho, sizeof(bool));
			pek += sizeof(bool);

			memcpy(pek, (char*)&sRot, sizeof(Vector4));
			pek += sizeof(Vector4);

			memcpy(pek, (char*)&sTrans, sizeof(Vector));

			//memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
			//pek += sizeof(unsigned int);


			cameraMovement = false;
			producer->push(msg, length);
		}
		
		////MGlobal::displayInfo(transform.name() + " worldmatrix changed");

		//	//kanske sätta en bool variabel som du skickar som "data
		//	//ändra denna i worldmatrix changed för att visa att kameran har flyttat sig

		//	size_t length =
		//		sizeof(MainHeader)
		//		+ sizeof(bool)
		//		+ sizeof(Vector) * 2
		//		+ sizeof(Vector4);

		//	/*this will also vary*/
		//	Vector sTrans, sScale; double tempScale[3];
		//	Vector4 sRot; double tempRot[4];

		//	transform.getScale(tempScale);
		//	sScale = tempScale;

		//	transform.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
		//	sRot.x = tempRot[0];
		//	sRot.y = tempRot[1];
		//	sRot.z = tempRot[2];
		//	sRot.w = tempRot[3];

		//	sTrans = transform.getTranslation(MSpace::kTransform, NULL);
		//	unsigned int nameLength = transform.name().length();
		//	Transformation mTransform{ 1, 3 };

		//	/*temp shit for camera testing*/
		//	MString mPersp = "persp";
		//	unsigned int perspL = mPersp.length();

		//	char * pek = msg;
		//	memcpy(pek, (char*)&mHead, sizeof(MainHeader));
		//	pek += sizeof(MainHeader);

		//	memcpy(pek, (char*)&mTransform, sizeof(Transformation));
		//	pek += sizeof(Transformation);

		//	memcpy(pek, (char*)&perspL, sizeof(unsigned int));
		//	pek += sizeof(unsigned int);

		//	memcpy(pek, (char*)mPersp.asChar(), perspL);
		//	pek += perspL;

		//	/*memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
		//	pek += sizeof(unsigned int);

		//	memcpy(pek, (char*)transform.name().asChar(), nameLength);
		//	pek += nameLength;*/

		//	memcpy(pek, (char*)&sScale, sizeof(Vector));
		//	pek += sizeof(Vector);

		//	memcpy(pek, (char*)&sRot, sizeof(Vector4));
		//	pek += sizeof(Vector4);

		//	memcpy(pek, (char*)&sTrans, sizeof(Vector));

		//	//memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
		//	//pek += sizeof(unsigned int);


		//	cameraMovement = false;
		//	producer->push(msg, length); 
			
	}
}

bool updateCamera()
{
	M3dView derp;
	derp.updateViewingParameters();

	MStatus rs;
	MDagPath camPath;
	rs = derp.getCamera(camPath);
	MFnCamera sCamera(camPath);

	MFnTransform transform = sCamera.parent(0);
	return true;
}
bool createMesh(MObject &node)
{
	//setAttr "pCubeShape1.quadSplit" 0;
	if (node.apiType() != MFn::kInvalid)
	{
		//MFnMesh mMesh(((MFnTransform)(node)).child(0), NULL);
		MFnMesh mMesh(node, NULL);
		if (mMesh.canBeWritten())
		{
			MString quadSplit = "setAttr """;
			quadSplit += mMesh.name();
			quadSplit += ".quadSplit"" 0;";
			MGlobal::executeCommandStringResult(quadSplit);

			MFnTransform transform = mMesh.parent(0);

			MGlobal::displayInfo(transform.name());

			MIntArray indexList, offsetIdList, normalCount, uvCount, uvIds;
			MFloatPointArray points;
			MFloatArray u, v;
			MFloatVectorArray normals;
			mMesh.getPoints(points, MSpace::kObject);
			mMesh.getTriangles(offsetIdList, indexList);
			/*assigning the main header to creation mode*/
			MainHeader sHeader{ 0 };

			//MString kissaner = "";
			//for (int i = 0; i < indexList.length(); i++)
			//{
			//	kissaner += indexList[i];
			//	kissaner += ", ";
			//	//MGlobal::displayInfo(indexList[i] + "\n");
			//}
			//MGlobal::displayInfo(kissaner);
			//kissaner = "";

			/*Creating the headers to send*/
			CreateMesh sMesh;
			sMesh.vertexCount = points.length();
			sMesh.indexCount = indexList.length();
			sMesh.nameLength = transform.name().length();
			mMesh.getTriangleOffsets(normalCount, offsetIdList);
			MIntArray vertexList, vertexCount;
			//mMesh.getVertices(vertexCount, vertexList);

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

			//kissaner = sMesh.normalCount;
			//kissaner += ", ";
			//kissaner += sMesh.indexCount;
			//kissaner += ", nr of normalids: ";
			//kissaner += normalId.length();
			//kissaner += ", nr of offsetids: ";
			//kissaner += offsetIdList.length();
			//MGlobal::displayInfo(kissaner);

			//MString kissaner;
			//kissaner = "";
			//for (int i = 0; i < sMesh.indexCount; i++)
			//{
			//	kissaner += i;
			//	kissaner += ": ";
			//	kissaner += normalId[offsetIdList[i]];
			//	kissaner += ", ";
			//	//kissaner += offsetIdList[indexList[i]];
			//	kissaner += indexList[i];
			//	kissaner += "\n";
			//}
			//MGlobal::displayInfo(kissaner);
			//kissaner = "";
			//for (int i = 0; i < test.length(); i++)
			//{
			//	kissaner += test[i];
			//	kissaner += ", ";
			//}
			//MGlobal::displayInfo(kissaner);
			//kissaner = "";
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
			int length = (sizeof(Vertex) * points.length())
				+ (sizeof(Index) * indexList.length())
				+ (sizeof(Normals) * normals.length())
				+ (sizeof(Index) * offsetIdList.length())
				+ (sizeof(Index) * normalId.length())
				+ sizeof(CreateMesh)
				+ sizeof(MainHeader)
				+ sizeof(Vector) * 2
				+ sizeof(Vector4)
				+ (sizeof(float) * u.length()) * 2
				+ (sizeof(Index)*sMesh.uvIndexCount)
				+ sMesh.nameLength;

			sMesh.normalIndexCount = normalId.length();

			/*constructing the message*/
			char * pek = msg;

			memcpy(pek, (char*)&sHeader, sizeof(MainHeader));
			pek += sizeof(MainHeader);

			memcpy(pek, (char*)&sMesh, sizeof(CreateMesh));
			pek += sizeof(CreateMesh);

			memcpy(pek, transform.name().asChar(), sMesh.nameLength);
			pek += sMesh.nameLength;

			memcpy(pek, (char*)&sScal, sizeof(Vector));
			pek += sizeof(Vector);

			memcpy(pek, (char*)&sRot, sizeof(Vector4));
			pek += sizeof(Vector4);

			memcpy(pek, (char*)&sTran, sizeof(Vector));
			pek += sizeof(Vector);

			memcpy(pek, (char*)mMesh.getRawPoints(NULL), (sizeof(Vertex)*sMesh.vertexCount));
			pek += sizeof(Vertex)*sMesh.vertexCount;

			memcpy(pek, (char*)&indexList[0], (sizeof(Index)*sMesh.indexCount));
			pek += sizeof(Index)*sMesh.indexCount;

			memcpy(pek, (char*)mMesh.getRawNormals(NULL), (sizeof(Normals)*sMesh.normalCount));
			pek += sizeof(Normals)*sMesh.normalCount;

			memcpy(pek, (char*)&normalId[0], (sizeof(Index) * sMesh.normalIndexCount));
			pek += sizeof(Index)*sMesh.normalIndexCount;

			memcpy(pek, (char*)&offsetIdList[0], (sizeof(Index)*sMesh.indexCount));
			pek += sizeof(Index)*sMesh.indexCount;

			memcpy(pek, (char*)&u[0], sizeof(float)*u.length());
			pek += sizeof(float)*u.length();

			memcpy(pek, (char*)&v[0], sizeof(float)*v.length());
			pek += sizeof(float)*u.length();

			memcpy(pek, (char*)&uvIds[0], sizeof(Index)*sMesh.uvIndexCount);


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


			return true;
		}
	}
	return false;
}

void attributeChanged(MNodeMessage::AttributeMessage Amsg, MPlug &plug, MPlug &otherPlug, void*clientData)
{
	
	
	/*if an orthographic camera "zooms"*/
	if ((Amsg == (Amsg & MNodeMessage::kAttributeSet | Amsg & MNodeMessage::kIncomingDirection)))
	{
		if (plug.node().apiType() == MFn::kCamera)
		{
			cameraMovement = true;
		}
	}
		//iter.kPlugSelectionItem;
	/*MObject hejsan;
	sList.getDependNode(0, hejsan);
	sList.getPlug(0, tempPlug);
	hejsan = tempPlug.asMObject();*/

	/*for (int i = 0; i < 1; iter.next())
	{
		MDagPath item;
		MObject component;
		iter.getDagPath(item, component);
		i++;
	}*/
	//if (hejsan == plug.asMObject())
		//MGlobal::displayInfo("bajs");
	//MGlobal::displayInfo(plug.name());
	//MGlobal::displayInfo(tempPlug.name());
	/*if (Amsg & MNodeMessage::kAttributeSet && plug.isArray())
	{
		MObject mNode = plug.node();
		MFnMesh mMesh = mNode;
		MFnTransform mTran = mNode;
		unsigned int hejsan = plug.evaluateNumElements();
		MString info;
		info += mTran.name();
		info += ": ";
		info += hejsan;
		MGlobal::displayInfo(info);
	}*/
	if (Amsg & MNodeMessage::kAttributeEval)
	{
		MString nams = plug.partialName(false, false, false, false, true);
		MGlobal::displayInfo(nams);
		if (plug.partialName() == "o" && meshTopChanged)
		{
			MGlobal::displayInfo(plug.partialName());
			meshTopChanged = false;
			if (!createMesh(plug.node()))
			{
				appendQueue(plug.node());
			}
		}
	}
	//if (Amsg & MNodeMessage::kLast)
	//MGlobal::displayInfo("knulla röv");
	MGlobal::displayInfo(plug.name());
	MObject mTest = plug.asMObject();
	MSelectionList sList;
	MGlobal::getActiveSelectionList(sList);
	//MPlug tempPlug;
	if (plug.asMObject().apiType() == MFn::kMeshData && Amsg & MNodeMessage::kIncomingDirection)
	{
		MGlobal::displayInfo("face moving");
	}
	MItSelectionList iter(sList);
	if (Amsg & MNodeMessage::kAttributeSet && !plug.isArray() && plug.isElement())
	{
		//MString info;
		//info = MGlobal::executePythonCommandStringResult("filterExpand -sm 31");
		//MGlobal::displayInfo(info);
		
		//MString info;
		MDagPath item;
		MObject component;
		//MPlug kiss;
		
		iter.getDagPath(item, component);
		iter.getDagPath(item);
		
		MObject mNode = plug.node();
		MFnMesh mMesh = mNode;
		//MPoint cPoint;
		//mMesh.getPoint(plug.logicalIndex(), cPoint);
		
		//MPoint selVert; 
		//selVert = mIt.position();
		if (component.apiType() == MFn::kMeshVertComponent)
		{
			MItMeshVertex mIt(item, component);
			if (plug.logicalIndex() == mIt.index())
			{
				MainHeader mHead{ 5 };
				MIntArray offsetIdList, indexList, temp;
				mMesh.getTriangles(offsetIdList, indexList);
				mMesh.getTriangleOffsets(temp, offsetIdList);
				MFnTransform mTran = mMesh.parent(0);
				modifyVertex mVert{ mTran.name().length(), mIt.count(), indexList.length() };
				MVectorArray vNormals;
				MString info;
				MIntArray normId;
				mIt.updateSurface();
				mIt.getNormalIndices(normId);


				MGlobal::displayInfo(info);
				char * pek = msg;
				size_t length = 0;

				memcpy(pek, (char*)&mHead, sizeof(MainHeader));
				pek += sizeof(MainHeader);
				length += sizeof(MainHeader);

				memcpy(pek, (char*)&mVert, sizeof(modifyVertex));
				pek += sizeof(modifyVertex);
				length += sizeof(modifyVertex);

				memcpy(pek, mTran.name().asChar(), mVert.nameLength);
				pek += mVert.nameLength;
				length += mVert.nameLength;

				memcpy(pek, (char*)&indexList[0], sizeof(Index)*mVert.indexLength);
				pek += sizeof(Index)*mVert.indexLength;
				length += sizeof(Index)*mVert.indexLength;

				memcpy(pek, (char*)&offsetIdList[0], sizeof(Index)*mVert.indexLength);
				pek += sizeof(Index)*mVert.indexLength;
				length += sizeof(Index)*mVert.indexLength;

				sendVertex tempSendVert;
				sendNormal tempSendNormal;
				for (; !mIt.isDone(); mIt.next())
				{
					mIt.getNormals(vNormals);
					MItMeshFaceVertex fvmIt(item, mIt.currentItem());

					tempSendVert.id = mIt.index();
					tempSendVert.nrNorm = vNormals.length();
					tempSendVert.translation = mIt.position();
					//sendVertex vertInfo{ mIt.index() , mIt.position()};

					memcpy(pek, (char*)&tempSendVert, sizeof(sendVertex));
					pek += sizeof(sendVertex);
					length += sizeof(sendVertex);

					/*verkar skicka samma normaler hela tiden, för att ex. 3 punkter
					kan alla hålla i samma normal, men det ver inte algoritmen om. Kanske
					skicka normaler efetr på något sätt?*/
					/*unsigned int balle = vNormals.length();
					unsigned int kuk = 0;
					for (; !fvmIt.isDone(); fvmIt.next())
					{
						MString fitta;
						kuk = fvmIt.faceId();
						fitta += kuk;
						MGlobal::displayInfo(fitta);
					}*/
					for (int i = 0; i < vNormals.length(); ++i)
					{
						tempSendNormal.id = fvmIt.faceId();
						fvmIt.next();
						tempSendNormal.translation = vNormals[i];
						
						memcpy(pek, (char*)&tempSendNormal, sizeof(sendNormal));
						pek += sizeof(sendNormal);
						length += sizeof(sendNormal);
					}
				}

				//MGlobal::displayInfo("done!");
				producer->push(msg, length);
			}
		}
		else if (component.apiType() == MFn::kMeshPolygonComponent)
		{
			MItMeshPolygon pIt(item, component);
			
			//unsigned int balle;
			//MItMeshVertex mIt(item, MFnTransform(component).parent(0));
			/*for (; !mIt.isDone(); mIt.next())
			{
				unsigned int balle = 0;
				balle = mIt.index();
				MString kuk = "";
				kuk += balle;
				MGlobal::displayInfo(kuk);
			}*/
			MIntArray idArr;
			//balle = plug.logicalIndex();
			pIt.getVertices(idArr);
			//balle = idArr[0];

			if (plug.logicalIndex() == idArr[0])
			{
				char * pek = msg;
				MainHeader mHead{ 5 };
				MIntArray offsetIdList, indexList, temp;
				mMesh.getTriangles(offsetIdList, indexList);
				mMesh.getTriangleOffsets(temp, offsetIdList);
				MFnTransform mTran = mMesh.parent(0);
				modifyVertex mVert{ mTran.name().length(), 0, indexList.length() };
				size_t length = 0;

				memcpy(pek, (char*)&mHead, sizeof(MainHeader));
				pek += sizeof(MainHeader);
				length += sizeof(MainHeader);

				//memcpy(pek, (char*)&mVert, sizeof(modifyVertex));
				pek += sizeof(modifyVertex);
				length += sizeof(modifyVertex);

				memcpy(pek, mTran.name().asChar(), mVert.nameLength);
				pek += mVert.nameLength;
				length += mVert.nameLength;

				memcpy(pek, (char*)&indexList[0], sizeof(Index)*mVert.indexLength);
				pek += sizeof(Index)*mVert.indexLength;
				length += sizeof(Index)*mVert.indexLength;

				memcpy(pek, (char*)&offsetIdList[0], sizeof(Index)*mVert.indexLength);
				pek += sizeof(Index)*mVert.indexLength;
				length += sizeof(Index)*mVert.indexLength;
				
				//sendVertex tempSendVert;
				sendVertex tempSendVert;
				for (; !pIt.isDone(); pIt.next())
				{
					MPointArray pointz;
					MVectorArray vNormals;

					pIt.getPoints(pointz);
					pIt.getVertices(idArr);
					//pIt.getNormals(normals);
					//offsetIdList.fin
					int terr = 0;

					MItMeshVertex mIt(item, MFnMesh(component).child(0));
					//mIt.setIndex(10, terr);
					for (int i = 0; i < idArr.length(); ++i)
					{
						mIt.setIndex(idArr[i], terr);
						mIt.getNormals(vNormals);
						MItMeshFaceVertex fvmIt(item, mIt.currentItem());

						tempSendVert.id = mIt.index();
						tempSendVert.nrNorm = vNormals.length();
						tempSendVert.translation = mIt.position();
						//sendVertex vertInfo{ mIt.index() , mIt.position()};

						memcpy(pek, (char*)&tempSendVert, sizeof(sendVertex));
						pek += sizeof(sendVertex);
						length += sizeof(sendVertex);

						sendNormal tempSendNormal;
						for (int i = 0; i < vNormals.length(); ++i)
						{
							tempSendNormal.id = fvmIt.faceId();
							fvmIt.next();
							tempSendNormal.translation = vNormals[i];

							memcpy(pek, (char*)&tempSendNormal, sizeof(sendNormal));
							pek += sizeof(sendNormal);
							length += sizeof(sendNormal);
						}
						MString indo;
						indo += mIt.index();
						MGlobal::displayInfo(indo);
						mVert.nrOfVertices++;
					}

					/*for (int i = 0; i < normals.length(); ++i)
					{
						MString info;
						info += normals[i].x;
						info += ", ";
						info += normals[i].y;
						info += ", ";
						info += normals[i].z;
						info += "; ";
						MGlobal::displayInfo(info);
					}*/
					

					/*pIt.getTriangles(ldvdvdvdv, test);
					for (int i = 0; i < test.length(); ++i)
					{
						unsigned int bajs = offsetIdList[test[i]];
						MString kuk;
						kuk += bajs;
						MGlobal::displayInfo(kuk);
					}*/
					
					//unsigned int bajs = knulla.length();
					//MString info;
					//MGlobal::displayInfo(info);
					

					/*for (int i = 0; i < pointz.length(); ++i)
					{
						tempSendVert.id = idArr[i];
						tempSendVert.translation.x = pointz[i].x;
						tempSendVert.translation.y = pointz[i].y;
						tempSendVert.translation.z = pointz[i].z;

						memcpy(pek, (char*)&tempSendVert, sizeof(sendVertex));
						pek += sizeof(sendVertex);
						length += sizeof(sendVertex);
						mVert.nrOfVertices++;
					}*/
				}

				memcpy((msg + sizeof(MainHeader)), (char*)&mVert, sizeof(modifyVertex));


				//for (; !mIt.isDone(); mIt.next())
				//{
				//	tempSendVert.id = mIt.index();
				//	tempSendVert.translation = mIt.position();
				//	//sendVertex vertInfo{ mIt.index() , mIt.position()};

				//	memcpy(pek, (char*)&tempSendVert, sizeof(sendVertex));
				//	pek += sizeof(sendVertex);
				//	length += sizeof(sendVertex);
				//}

				//MGlobal::displayInfo("done!");
				producer->push(msg, length);
			}
		}

	}
}

void changedNameFunction(MObject &node, const MString &str, void*clientData)
{
	MGlobal::displayInfo("name changed, new name: " + MFnDagNode(node).name());
	MGlobal::displayInfo(str);
	MainHeader mHead{ 9 };
	nameChange mSend{ str.length(), MFnDagNode(node).name().length() };
	size_t length = sizeof(nameChange)
		+ sizeof(MainHeader)
		+ mSend.nameLength
		+ mSend.newNameLength
		+ 1;
	char * pek = msg;
	
	memcpy(pek, (char*)&mHead, sizeof(MainHeader));
	pek += sizeof(MainHeader);

	memcpy(pek, (char*)&mSend, sizeof(nameChange));
	pek += sizeof(nameChange);

	memcpy(pek, str.asChar(), mSend.nameLength);
	pek += mSend.nameLength + 1;

	memcpy(pek, MFnDagNode(node).name().asChar(), mSend.newNameLength);

	producer->push(msg, length);
}
//fix adde node funtion so that you just use a function that creates everything/adds callbakcs
//add topology changed callback
#pragma endregion
#pragma region Creation
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
//void createViewportCamera(MObject & node)
//{
//	MFnTransform transform = node;
//	MFnCamera sCamera = transform.child(0);
//
//
//
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
//}
#pragma endregion
#pragma region Modified
void topologyChanged(MObject & node, void * data)
{
	MGlobal::displayInfo("haärasdce");
	meshTopChanged = true;
}
void nodeRemoved(MObject &node, void *data)
{
	MGlobal::displayInfo(MFnTransform(node).name() + " removed.");
	MainHeader mHead{ 10 };
	Index deleteInfo{ MFnTransform(node).name().length() };

	char * pek = msg;
	memcpy(pek, (char*)&mHead, sizeof(MainHeader));
	pek += sizeof(MainHeader);

	memcpy(pek, (char*)&deleteInfo, sizeof(Index));
	pek += sizeof(Index);

	memcpy(pek, MFnTransform(node).name().asChar(), deleteInfo.nr);

	size_t length =
		sizeof(MainHeader) +
		sizeof(Index) +
		deleteInfo.nr;

	producer->push(msg, length);
}
void elapsedTimeFunction(float elapsedTime, float lastTime, void*clientData)
{
	//model queue here!
	
	//DeltaTime = elapsedTime - lastTime;
	//modifiedTime += elapsedTime;
	CurrentTime = elapsedTime;
	if (nrInQueue != 0)
	{
		for (int i = 0; i < nrInQueue; ++i)
		{
			if (createMesh(modelQueue[i]))
				popQueue(&i);
		}
	}
	//MGlobal::displayInfo((MString("Current time: ")+=(CurrentTime)));
	//updateCamera();

	/*if (CurrentTime > 0 && CurrentTime < 9)
	{
		producer->push("bajskorven", (size_t)11);
		MGlobal::displayInfo("TIMKE IS NOW SENDING!");
	}*/
}
void addedNodeFunction(MObject &node, void*clientData) //look at this function with teachers
{
	MGlobal::displayInfo("created: " + MFnTransform(node).name());

	//if (node.hasFn(MFn::kTransform) && !node.hasFn(MFn::kMesh))
	//{
	//	MFnTransform trans = node;
	//	if(trans.canBeWritten())
	//		MGlobal::displayInfo("locked");
	//	/*if (trans.isDefaultNode())
	//		MGlobal::displayInfo("default");
	//	if (trans.isLocked())
	//		MGlobal::displayInfo("locked");
	//	if (trans.isShared())
	//		MGlobal::displayInfo("shared");
	//	if (node.isNull())
	//		MGlobal::displayInfo("isNull");
	//	if (trans.isTrackingEdits())
	//		MGlobal::displayInfo("tracking");
	//	if (trans.isInstanced())
	//		MGlobal::displayInfo("instanced");
	//	if (trans.isIntermediateObject())
	//		MGlobal::displayInfo("interwhat");*/
	//	if (trans.isInstanceable())
	//		MGlobal::displayInfo("instance");

	//}
	//if (node.hasFn(MFn::kMesh))
	//{
	//	MGlobal::displayInfo("sug skit!");
	//}
	/*connecting the new node the the worldmatrix function*/
	if (node.hasFn(MFn::kMesh))
	{
		MStatus Result = MS::kSuccess;
		//MObject trans = MFnMesh(node).parent(0);
		//MObject tranas = MFnMesh(node).child(0);

		//MGlobal::displayInfo(trans.name());
		//MGlobal::displayInfo(tranas.name());

		MDagPath meshDag = MDagPath::getAPathTo(node);
		MCallbackId newId = MDagMessage::addWorldMatrixModifiedCallback(meshDag, WorldMatrixModified, NULL, &Result);
		if (Result == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
			{
				//MGlobal::displayInfo(trans.name() + " Successfully added to the MatrixModified Function");
			}
		}
		newId = MPolyMessage::addPolyTopologyChangedCallback(node, topologyChanged, NULL, &Result);

		if (Result == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
				MGlobal::displayInfo("made connection to the topology changed callback");
		}
		else
			MGlobal::displayInfo("failed to connect topology changed callback");
		//if (trans.child(0).hasFn(MFn::kMesh))
		//{
			newId = MNodeMessage::addAttributeChangedCallback(node, attributeChanged, NULL, &Result);
			if (Result == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
					MGlobal::displayInfo("made connection to the attributtes");
			}
			else
				MGlobal::displayInfo("failed to connect attributes");
			if (!createMesh(node))
			{
				MGlobal::displayInfo(MFnTransform(node).name() + "sent to the queue");
				appendQueue(node);
			}
		//}
	}
	if (node.hasFn(MFn::kTransform))
	{
		MStatus Result = MS::kSuccess;
		MFnTransform trans = node;
		/*MDagPath meshDag = MDagPath::getAPathTo(trans.child(0));
		MCallbackId newId = MDagMessage::addWorldMatrixModifiedCallback(meshDag, WorldMatrixModified, NULL, &Result);
		if (Result == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
			{
				MGlobal::displayInfo(trans.name() + " Successfully added to the MatrixModified Function");
			}
		}*/
		MCallbackId newId = MNodeMessage::addNameChangedCallback(node, changedNameFunction, NULL, &Result);
		if (Result == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
			{
				MGlobal::displayInfo("Connected the NameChangeFunction");
			}
			else
				MGlobal::displayInfo("failed to connect NameChangeFunction");
		}
		newId = MModelMessage::addNodeRemovedFromModelCallback(node, nodeRemoved, NULL, &Result);
		if (Result == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
				MGlobal::displayInfo("made connection to the removed node callback");
		}
		else
			MGlobal::displayInfo("failed to connect removed shit");
		/*if (trans.child(0).hasFn(MFn::kMesh))
		{
			newId = MNodeMessage::addAttributeChangedCallback(trans.child(0), attributeChanged, NULL, &Result);
			if (Result == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
					MGlobal::displayInfo("made connection to the attributtes");
			}
			else
				MGlobal::displayInfo("failed to connect attributes");
			if (!createMesh(trans.child(0)))
			{
				MGlobal::displayInfo(MFnTransform(node).name() + "sent to the queue");
				appendQueue(trans.child(0));
			}
		}*/
	}
}
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
	producer = new CircularBuffer(L"poop3", 200, true, 256);
	msg = new char[(200 * (1 << 10))/4];

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
			newId = MNodeMessage::addNameChangedCallback(meshIt.currentItem(), changedNameFunction, NULL, &loopResults);
			if (loopResults == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
				{
					MGlobal::displayInfo("Connected the NameChangeFunction");
				}
				else
					MGlobal::displayInfo("failed to connect NameChangeFunction");
			}
			
			newId = MModelMessage::addNodeRemovedFromModelCallback(meshIt.currentItem(), nodeRemoved, NULL, &loopResults);
			if (loopResults == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
					MGlobal::displayInfo("made connection to the removed node callback");
			}
			else
				MGlobal::displayInfo("failed to connect removed shit");
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

			newId = MPolyMessage::addPolyTopologyChangedCallback(trans.child(0), topologyChanged, NULL, &loopResults);

			if (loopResults == MS::kSuccess)
			{
				if (myCallbackArray.append(newId) == MS::kSuccess)
					MGlobal::displayInfo("made connection to the topology changed callback");
			}
			else
				MGlobal::displayInfo("failed to connect topology changed callback");


			createMesh(trans.child(0));
			//createMesh(trans.child(0));
			//MItMeshPolygon( const MObject & polyObject, MStatus * ReturnStatus = NULL );
			//producer->push(trans.name().asChar(), trans.name().length());
		}
		if (trans.child(0).hasFn(MFn::kCamera))
		{
			//if (MFnTransform(meshIt.currentItem()).name() == "persp" || MFnTransform(meshIt.currentItem()).name() == "front")
				//createViewportCamera(meshIt.currentItem());
			if (MFnTransform(meshIt.currentItem()).name() != "persp")
			{
				MCallbackId newId = MNodeMessage::addAttributeChangedCallback(trans.child(0), attributeChanged, NULL, &loopResults);
				if (myCallbackArray.append(newId) == MS::kSuccess)
					MGlobal::displayInfo("created test");
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
	MCallbackId newId = MTimerMessage::addTimerCallback(UPDATE_TIME, elapsedTimeFunction, NULL, &loopResults);
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

	newId = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel4", preRenderCB, NULL, &loopResults);
	if (loopResults == MS::kSuccess)
	{
		if (myCallbackArray.append(newId) == MS::kSuccess)
		{
			MGlobal::displayInfo("created 3dpreprocess function");
		}
	}
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

	newId = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel1", preRenderCB, NULL, &loopResults);
	if (loopResults == MS::kSuccess)
	{
		if (myCallbackArray.append(newId) == MS::kSuccess)
		{
			MGlobal::displayInfo("created 3dpreprocess function");
		}
	}
	else
		MGlobal::displayInfo("Failed to create 3dpreprocess function");
	newId = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel2", preRenderCB, NULL, &loopResults);
	if (loopResults == MS::kSuccess)
	{
		if (myCallbackArray.append(newId) == MS::kSuccess)
		{
			MGlobal::displayInfo("created 3dpreprocess function");
		}
	}
	else
		MGlobal::displayInfo("Failed to create 3dpreprocess function");
	newId = MUiMessage::add3dViewPreRenderMsgCallback("modelPanel3", preRenderCB, NULL, &loopResults);
	if (loopResults == MS::kSuccess)
	{
		if (myCallbackArray.append(newId) == MS::kSuccess)
		{
			MGlobal::displayInfo("created 3dpreprocess function");
		}
	}
	else
		MGlobal::displayInfo("Failed to create 3dpreprocess function");



	return res;
}

/*MStatus 	remove (unsigned int index)
 	Remove the array element at the given index. More...*/

EXPORT MStatus uninitializePlugin(MObject obj)
{
	/*deleting the producer*/
	delete producer;

	delete msg;

	//delete modelQueue;

	//free resources here
	MFnPlugin plugin(obj);

	MMessage::removeCallbacks(myCallbackArray);

	MGlobal::displayInfo("Maya plugin unloaded!");

	return MS::kSuccess;
}