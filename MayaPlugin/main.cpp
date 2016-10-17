/*Creator: David Wigelius*/
#include "maya_includes.h"
#include "structs.h"
#include <iostream>
#include "CircularBuffer.h"//<-------------------------- fix this later so that it's a lib
#include <queue>
#include "Utils.h"

using namespace std;
#define UPDATE_TIME 0.5f
#define QUEUE_VALUE 20

CircularBuffer *producer;
MCallbackIdArray myCallbackArray;
float CurrentTime = 0; //kanske göra en pekare för att kunna kontrollera minne
bool cameraMovement = false;
MObject modelQueue[QUEUE_VALUE];
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
			if (mNode == transformNode)
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
	if (cameraMovement)
	{
		//MGlobal::displayInfo(panelName);
		M3dView mCam = mCam.active3dView();
		MDagPath camPath;
		MStatus rs;

		rs = mCam.getCamera(camPath);
		MFnCamera sCamera(camPath);

		MFnTransform transform = sCamera.parent(0);



		//MGlobal::displayInfo(transform.name() + " worldmatrix changed");
		MainHeader mHead{ 4 };

		//kanske sätta en bool variabel som du skickar som "data
		//ändra denna i worldmatrix changed för att visa att kameran har flyttat sig

		size_t length =
			sizeof(MainHeader)
			+ sizeof(Transformation)
			+ transform.name().length()
			+ sizeof(unsigned int)
			+ sizeof(Vector) * 2
			+ sizeof(Vector4);

		/*this will also vary*/
		Vector sTrans, sScale; double tempScale[3];
		Vector4 sRot; double tempRot[4];

		transform.getScale(tempScale);
		sScale = tempScale;

		transform.getRotationQuaternion(tempRot[0], tempRot[1], tempRot[2], tempRot[3], MSpace::kTransform);
		sRot.x = tempRot[0];
		sRot.y = tempRot[1];
		sRot.z = tempRot[2];
		sRot.w = tempRot[3];

		sTrans = transform.getTranslation(MSpace::kTransform, NULL);
		unsigned int nameLength = transform.name().length();
		Transformation mTransform{ 1, 3 };

		char * pek = msg;
		memcpy(pek, (char*)&mHead, sizeof(MainHeader));
		pek += sizeof(MainHeader);

		memcpy(pek, (char*)&mTransform, sizeof(Transformation));
		pek += sizeof(Transformation);

		memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
		pek += sizeof(unsigned int);

		memcpy(pek, (char*)transform.name().asChar(), nameLength);
		pek += nameLength;

		memcpy(pek, (char*)&sScale, sizeof(Vector));
		pek += sizeof(Vector);

		memcpy(pek, (char*)&sRot, sizeof(Vector4));
		pek += sizeof(Vector4);

		memcpy(pek, (char*)&sTrans, sizeof(Vector));

		//memcpy(pek, (char*)&nameLength, sizeof(unsigned int));
		//pek += sizeof(unsigned int);


		cameraMovement = false;
		producer->push(msg, length);
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

void attributeChanged(MNodeMessage::AttributeMessage Amsg, MPlug &plug, MPlug &otherPlug, void*clientData)
{
	MSelectionList sList;
	MGlobal::getActiveSelectionList(sList);
	//MPlug tempPlug;
	MItSelectionList iter(sList);
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
	//if (Amsg & MNodeMessage::kLast)
		//MGlobal::displayInfo("knulla röv");
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
			//MDataHandle terrra = plug.asMDataHandle();
			//info += item.fullPathName();
			iter.getDagPath(item);

			MObject mNode = plug.node();
			MFnMesh mMesh = mNode;
			MPoint cPoint;
			mMesh.getPoint(plug.logicalIndex(), cPoint);
			MItMeshVertex mIt(item, component);
			MPoint selVert; 
			selVert = mIt.position();
			if (cPoint == selVert)
			{
				MainHeader mHead{ 5 };
				MIntArray offsetIdList, indexList;
				mMesh.getTriangles(offsetIdList, indexList);
				MFnTransform mTran = mMesh.parent(0);
				modifyVertex mVert{ mTran.name().length(), mIt.count(), indexList.length() };
				//int bajs = mIt.count();
				//int sugrov = mIt.index();
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

				sendVertex tempSendVert;
				for (; !mIt.isDone(); mIt.next())
				{
					tempSendVert.id = mIt.index();
					tempSendVert.translation = mIt.position();
					//sendVertex vertInfo{ mIt.index() , mIt.position()};

					memcpy(pek, (char*)&tempSendVert, sizeof(sendVertex));
					pek += sizeof(sendVertex);
					length += sizeof(sendVertex);
				}

				MGlobal::displayInfo("done!");
				producer->push(msg, length);
			}
			//MPlug kuken = plug.parent();
			//component = kuken.asMObject();
			//component == terrra;
			//if (kiss == )
			//info += cPoint.className();
			//MPoint * balle = component;
				//MGlobal::displayInfo("bajs");

			//MGlobal::displayInfo(info);
			//i++;
		//MGlobal::displayInfo(plug.name());
		////plug.isCompound()
		////MFnTransform(plug.attribute()).getTranslation(MSpace::kWorld, NULL).x;
		////MString values;
		//Vector values;
		////values.x = plug.child(0).asFloat();
		////values.y = plug.child(1).asFloat();
		////values.z = plug.child(2).asFloat();
		////values += x; values += ", "; values += y; values += ", "; values += z;
		////MGlobal::displayInfo("attribute changed: " + plug.name() + "\nNew value: " + values);
		//modifyVertex sVert;
		//MObject mNode = plug.node();
		////MFnTransform kiss = bajs;
		////bajs = kiss.parent(0);
		//MFnMesh mMesh = mNode;
		//MIntArray offsetIdList, indexList;
		//mMesh.getTriangles(offsetIdList, indexList);
		//MString info;
		//for (int i = 0; i < indexList.length(); ++i)
		//{
		//	info += indexList[i];
		//	info += ", ";
		//}
		//MGlobal::displayInfo(info);
		//info = "";
		//mNode = mMesh.parent(0);
		////plug.legicalIndex();
		////MString knulla;
		////plug.info()
		////unsigned int plugIndex = plug.logicalIndex();
		////plug.selectAncestorLogicalIndex(0, bajs);
		//MFnTransform mTran = mNode;
		////bajs = kiss.parent(0);
		////bajs = kiss.child(0);
		//info += mTran.name();
		////MStringArray balle;
		////knulla.split('.', balle);
		//sVert.indexLength = indexList.length();
		//sVert.nameLength = mTran.name().length();
		//MainHeader mHead{ 5 };
		//MPoint tempPos;
		//mMesh.getPoint(plug.logicalIndex(), tempPos);
		//values = tempPos;
		//sendVertex vertInfo{ plug.logicalIndex() , values };
		//

		//char*pek = msg;
		//memcpy(pek, (char*)&mHead, sizeof(MainHeader));
		//pek += sizeof(MainHeader);

		//memcpy(pek, (char*)&sVert, sizeof(modifyVertex));
		//pek += sizeof(modifyVertex);

		//memcpy(pek, mTran.name().asChar(), sVert.nameLength);
		//pek += sVert.nameLength;
		//
		//memcpy(pek, (char*)&indexList[0], (sizeof(Index)*sVert.indexLength));
		//pek += sizeof(Index)*sVert.indexLength;

		//memcpy(pek, (char*)&vertInfo, sizeof(sendVertex));

		//MGlobal::displayInfo(info);
		//size_t length =
		//	sizeof(MainHeader) +
		//	sizeof(modifyVertex) +
		//	sVert.nameLength +
		//	sizeof(Index)*sVert.indexLength +
		//	sizeof(sendVertex);


		//producer->push(msg, length);

		//modifyVertex sVert;


	}
}

void changedNameFunction(MObject &node, const MString &str, void*clientData)
{
	MGlobal::displayInfo("name changed, new name: " + MFnDagNode(node).name());
}
//fix adde node funtion so that you just use a function that creates everything/adds callbakcs
//add topology changed callback
#pragma endregion
bool getTextureFileInfo(MObject &shaderNode, MString type, MString &path) //takes a material node, and a type which specifies plugtype (f.ex.: "normalCamera") for wanted texture, returns in &path
{
	MStatus res;
	MPlugArray plugArray;
	MFnDependencyNode depNode(shaderNode, &res);

	MPlug attrib = depNode.findPlug(type, &res);
	bool connected = attrib.connectedTo(plugArray, true, false, &res);

	if (res == MS::kSuccess && plugArray.length() > 0)
	{
		MObject source = plugArray[0].node();

		MPlug FileTexNamePlug;

		if (source.apiType() == MFn::kFileTexture)
		{
			FileTexNamePlug = MFnDependencyNode(source).findPlug("ftn", &res);
			if (res == MS::kSuccess)
			{
				FileTexNamePlug.getValue(path);
				return true;
			}
		}

		MItDependencyGraph dgIt(source,
			MFn::kFileTexture,
			MItDependencyGraph::kUpstream,
			MItDependencyGraph::kBreadthFirst,
			MItDependencyGraph::kNodeLevel);

		dgIt.disablePruningOnFilter();
		MFnDependencyNode dgNodeFnSet;

		for (; !dgIt.isDone(); dgIt.next()) {

			MObject thisNode = dgIt.thisNode();
			dgNodeFnSet.setObject(thisNode);

			MPlug FileTexNamePlug = dgNodeFnSet.findPlug("ftn", &res);
			if (res == MS::kSuccess)
			{
				FileTexNamePlug.getValue(path);
				return true;
			}
		}
	}
	else
	{
		path = "";
		return false; //not using a texture
	}
	
	return false;
}

	

#pragma region Creation
bool createMaterial(MObject &node, bool isPhong)
{
	MStatus res;

	MString paths[tex::TEXTURE_TYPE_COUNT];
	MainHeader mainHeader{ 3 };


	unsigned int length;
	CreateMaterial mHeader{ 0, 0, 0, 0, 0, isPhong };
	ambient amb;
	diffuse diff;
	specular spec;
	MString materialName;

	MGlobal::displayInfo("\nMATERIAL FOUND\n");

	MFnLambertShader lambert(node);

	MString INFO = "NAME: ";
	INFO += lambert.name();
	INFO += "\n";
	MGlobal::displayInfo(INFO);
	INFO = "";

	materialName = lambert.name();
	mHeader.nameLength = materialName.length();

	if (getTextureFileInfo(node, tex::textureTypes[tex::NORMAL], paths[tex::NORMAL]))
	{
		mHeader.normalPathLength = paths[tex::NORMAL].length();
		MString INFO = "Normal: ";
		INFO += paths[tex::NORMAL];
		INFO += "\n";
		MGlobal::displayInfo(INFO);
		INFO = "";
	}

	MGlobal::displayInfo("Ambient: ");
	if (!getTextureFileInfo(node, tex::textureTypes[tex::AMBIENT], paths[tex::AMBIENT]))
	{
		

		lambert.ambientColor().get((float*)&amb);
		INFO += amb.r;
		INFO += ", ";
		INFO += amb.g;
		INFO += ", ";
		INFO += amb.b;
		INFO += "\n";
		MGlobal::displayInfo(INFO);
		INFO = "";
	}
	mHeader.ambientPathLength = paths[tex::AMBIENT].length();

	MGlobal::displayInfo("Diffuse: \n");
	if (!getTextureFileInfo(node, tex::textureTypes[tex::DIFFUSE], paths[tex::DIFFUSE]))
	{
		lambert.color().get((float*)&diff);
		INFO += diff.r;
		INFO += ", ";
		INFO += diff.g;
		INFO += ", ";
		INFO += diff.b;
		INFO += ", ";
	}
	mHeader.texturePathLength = paths[tex::DIFFUSE].length();
	INFO += "Color: ";
	INFO += paths[tex::DIFFUSE];
	INFO += "\n";
	diff.coeff = lambert.diffuseCoeff();
	INFO += "diffuse coefficient : ";
	INFO += diff.coeff;
	INFO += ", ";
	MGlobal::displayInfo(INFO);
	INFO = "";


	if (isPhong)
	{
		MGlobal::displayInfo("AND IT HAS PHONG SHIT\n");
		MFnPhongShader phong(node);
		
		MGlobal::displayInfo("Spec:");
		if (!getTextureFileInfo(node, tex::textureTypes[tex::SPECULAR], paths[tex::SPECULAR]))
		{
			mHeader.specularPathlength = paths[tex::SPECULAR].length();

			phong.specularColor().get((float*)&spec);
			INFO += spec.r;
			INFO += ", ";
			INFO += spec.g;
			INFO += ", ";
			INFO += spec.b;
			INFO += "\n";
			MGlobal::displayInfo(INFO);
			INFO = "";
		}
		mHeader.specularPathlength = paths[tex::SPECULAR].length();
		spec.shine = phong.cosPower();
		MGlobal::displayInfo("Shine:");
		INFO += spec.shine;
		INFO += "\n";
		MGlobal::displayInfo(INFO);
		INFO = "";

		//MGlobal::displayInfo("Reflection color:");
		//INFO += phong.reflectedColor().r;
		//INFO += ", ";
		//INFO += phong.reflectedColor().g;
		//INFO += ", ";
		//INFO += phong.reflectedColor().b;
		//INFO += "\n";
		//MGlobal::displayInfo(INFO);
		//INFO = "";

		//MGlobal::displayInfo("Reflectivity:");
		//INFO += phong.reflectivity();
		//INFO += "\n";
		//MGlobal::displayInfo(INFO);
		//INFO = "";
	}

	//MGlobal::displayInfo("Translucence:");
	//INFO += lambert.translucenceCoeff();
	//INFO += "\n";
	//MGlobal::displayInfo(INFO);
	//INFO = "";

	//if (!getTextureFileInfo(node, tex::textureTypes[tex::TRANSPARENCY]));
	//{
	//	MGlobal::displayInfo("Transparency:");
	//	INFO += lambert.transparency().r;
	//	INFO += ", ";
	//	INFO += lambert.transparency().g;
	//	INFO += ", ";
	//	INFO += lambert.transparency().b;
	//	INFO += "\n";
	//	MGlobal::displayInfo(INFO);
	//	INFO = "";
	//}

	//MGlobal::displayInfo("Glow:");
	//INFO += lambert.glowIntensity();
	//INFO += "\n";
	//MGlobal::displayInfo(INFO);
	//INFO = "";

	//if (!getTextureFileInfo(node, tex::textureTypes[tex::INCANDESCENCE]));
	//{
	//	MGlobal::displayInfo("Incandescence:");
	//	INFO += lambert.incandescence().r;
	//	INFO += ", ";
	//	INFO += lambert.incandescence().g;
	//	INFO += ", ";
	//	INFO += lambert.incandescence().b;
	//	INFO += "\n";
	//	MGlobal::displayInfo(INFO);
	//	INFO = "";
	//} 


	//MEMCOPY DIS WOOOOO
	length = (sizeof(MainHeader)
		+ sizeof(CreateMaterial)
		+ mHeader.nameLength
		+ mHeader.texturePathLength
		+ mHeader.normalPathLength
		+ mHeader.ambientPathLength
		+ mHeader.specularPathlength
		+ sizeof(ambient)
		+ sizeof(diffuse)
		+ sizeof(specular));

	char * pek = msg;

	memcpy(pek, (char*)&mainHeader, sizeof(MainHeader));
	pek += sizeof(MainHeader);

	memcpy(pek, (char*)&mHeader, sizeof(CreateMaterial));
	pek += sizeof(CreateMaterial);


	//THIS MIGHT WORK IF MSTRING DATA IS STRUCTURED THE WAY IT OUGHT TO BE... MEN SÅ KUL SKULLE VI VISST INTE HA
	//char * tetas[256];
	//int nameChunk = (mHeader.nameLength
	//	+ mHeader.texturePathLength
	//	+ mHeader.normalPathLength
	//	+ mHeader.ambientPathLength
	//	+ mHeader.specularPathlength);
	//memcpy(pek, (char*)materialName.asChar(), nameChunk);
	//*tetas = (pek + mHeader.nameLength);
	//pek += nameChunk;


	memcpy(pek, (char*)materialName.asChar(), mHeader.nameLength);
	pek += mHeader.nameLength;

	memcpy(pek, (char*)paths[tex::DIFFUSE].asChar(), mHeader.texturePathLength);
	pek += mHeader.texturePathLength;

	memcpy(pek, (char*)paths[tex::NORMAL].asChar(), mHeader.normalPathLength);
	pek += mHeader.normalPathLength;

	memcpy(pek, (char*)paths[tex::AMBIENT].asChar(), mHeader.ambientPathLength);
	pek += mHeader.ambientPathLength;

	memcpy(pek, (char*)paths[tex::SPECULAR].asChar(), mHeader.specularPathlength);
	pek += mHeader.specularPathlength;

	memcpy(pek, (char*)&amb, sizeof(ambient));
	pek += sizeof(ambient);

	memcpy(pek, (char*)&diff, sizeof(diffuse));
	pek += sizeof(diffuse);

	memcpy(pek, (char*)&spec, sizeof(specular));
	pek += sizeof(specular);

	

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

bool createMesh(MObject &node)
{
	//setAttr "pCubeShape1.quadSplit" 0;
	if (node.apiType() != MFn::kInvalid)
	{
		MFnMesh mMesh(((MFnTransform)(node)).child(0), NULL);
		if (mMesh.canBeWritten())
		{
			MString quadSplit = "setAttr """;
			quadSplit += mMesh.name();
			quadSplit += ".quadSplit"" 0;";
			MGlobal::executeCommandStringResult(quadSplit);

		
			MString materialName;
			MFnTransform transform = node;
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
			MStatus res;
			CreateMesh sMesh;
			sMesh.vertexCount = points.length();
			sMesh.indexCount = indexList.length();
			sMesh.nameLength = transform.name().length();
			mMesh.getTriangleOffsets(normalCount, offsetIdList);
			MIntArray vertexList, vertexCount;
			//mMesh.getVertices(vertexCount, vertexList);

			/*getting material name n length*/
			MItDependencyNodes materialIt(MFn::kLambert, &res);
			for (; !materialIt.isDone(); materialIt.next())
			{
				MFnLambertShader mMaterial(materialIt.thisNode());
				materialName = mMaterial.name();
			}
			sMesh.materialNameLength = materialName.length();

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
				+ sMesh.nameLength
				+ sMesh.materialNameLength;

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

			memcpy(pek, (char*)&v[0], sizeof(float)*u.length());
			pek += sizeof(float)*u.length();

			memcpy(pek, (char*)&uvIds[0], sizeof(Index)*sMesh.uvIndexCount);
			pek += sizeof(Index)*sMesh.uvIndexCount;

			memcpy(pek, materialName.asChar(), sMesh.materialNameLength);

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
void nodeRemoved(MObject &node, void *data)
{
	MGlobal::displayInfo(MFnTransform(node).name() + " removed.");
	MainHeader mHead{ 9 };
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
		newId = MModelMessage::addNodeRemovedFromModelCallback(node, nodeRemoved, NULL, &Result);
		if (Result == MS::kSuccess)
		{
			if (myCallbackArray.append(newId) == MS::kSuccess)
				MGlobal::displayInfo("made connection to the removed node callback");
		}
		else
			MGlobal::displayInfo("failed to connect removed shit");
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
			if (!createMesh(node))
			{
				MGlobal::displayInfo(MFnTransform(node).name() + "sent to the queue");
				appendQueue(node);
			}
		}
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
	}
	MItDependencyNodes materialIt(MFn::kLambert, &res);
	for (; !materialIt.isDone(); materialIt.next())
	{
		MFn::Type currentType = materialIt.thisNode().apiType();
		createMaterial(materialIt.thisNode(), materialIt.thisNode().hasFn(MFn::kPhong));

	}
	createViewportCamera();
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