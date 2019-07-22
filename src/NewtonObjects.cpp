#include "dxstdafx.h"
#include ".\newtonobjects.h"
#include "Player.h"
#include "Materials.h"
#include "vector.h"


#define STARTINGHEIGHT 6 //radius of player sphere collsion primitive
CDXUTMesh*	 NewtonObjects::collisionMesh;
bool	     NewtonObjects::isWorldCollidable = true;
NewtonWorld* NewtonObjects::nWorld;
CarManager*  NewtonObjects::carManager;

NewtonObjects::NewtonObjects(Player *player)
{
  collisionMesh = new CDXUTMesh(L"meshes/collision.x");
  carManager = new CarManager();

  D3DXMatrixIdentity(&matWorld);
  p = player;
  m_AccumlativeTimeSlice = 0;
}

void NewtonObjects::initNewton()
{
  
  // create the newton world
  nWorld = NewtonCreate (PhysicsAlloc, PhysicsFree);
  SetupMaterials(nWorld);
  readInLevel();

  //init player
  addPlayer();
  
  carManager->addCars(nWorld, vehicleID);
    
}

void NewtonObjects::frameMove(IDirect3DDevice9* device)
{
	carManager->frameMove(device);

    if(isWorldCollidable)
      NewtonMaterialSetDefaultCollidable(nWorld, characterID, levelID, 1);
    else
      NewtonMaterialSetDefaultCollidable(nWorld, characterID, levelID, 0);

    if( Player::sPlayerInfo->isInCar){
      NewtonMaterialSetDefaultCollidable(nWorld, characterID, vehicleID, 0);
      NewtonMaterialSetDefaultCollidable(nWorld, characterID, levelID, 0);
    }
    else{		
      NewtonMaterialSetDefaultCollidable(nWorld, characterID, vehicleID, 1);      
    }
}

void NewtonObjects::drawNewtonScene(IDirect3DDevice9* pd3dDevice, float fElapsedTime)
{
  //update Newton world
  float sleepTime = 1.0f / 200.0f;
  m_AccumlativeTimeSlice += fElapsedTime;
  while (m_AccumlativeTimeSlice > sleepTime){
   NewtonUpdate(nWorld, sleepTime);
   m_AccumlativeTimeSlice -= sleepTime;
  }
  
  
  carManager->render(pd3dDevice, p->getFrustum()); 	
    
  
}

void NewtonObjects::addPlayer()
{
  collision = NewtonCreateSphere(nWorld, STARTINGHEIGHT,STARTINGHEIGHT,STARTINGHEIGHT, NULL);
  playerBody = NewtonCreateBody(nWorld, collision);
  NewtonBodySetAutoFreeze (playerBody, 0);
  NewtonWorldUnfreezeBody (nWorld, playerBody);
  NewtonBodySetMaterialGroupID (playerBody, characterID);
  NewtonBodySetDestructorCallback (playerBody, PhysicsBodyDestructor);
  NewtonBodySetTransformCallback (playerBody, PhysicsSetTransform);
  NewtonBodySetForceAndTorqueCallback (playerBody, CharacterApplyForceAndTorque);
  NewtonBodySetUserData(playerBody, p->getPlayerInfo());
  NewtonReleaseCollision (nWorld, collision);

  D3DXMATRIX location;
  D3DXMatrixIdentity(&location);
  D3DXMatrixTranslation(&location, 0,STARTINGHEIGHT,0);
  NewtonBodySetMassMatrix (playerBody, 1.0f, 1.0f, 1.0f, 1.0f);
  NewtonBodySetMatrix(playerBody, (FLOAT *)location);
  PhysicsSetTransform (playerBody, (FLOAT *)location);

  // add and up vector contraint to help in keepin the body upright
  NewtonJoint* upVector;
  D3DXVECTOR3 upDirection (0.0f, 1.0f, 0.0f);
  upVector = NewtonConstraintCreateUpVector (nWorld, (FLOAT *)upDirection, playerBody); 

  
}
void _cdecl NewtonObjects::CharacterApplyForceAndTorque(const NewtonBody *body)
{
 	float mass, Ixx, Iyy, Izz;
	D3DXVECTOR3 force(0,0,0), heading, zero(0,0,0), velocity;
	// get the associated structure
	playerInfo *info = (playerInfo*)NewtonBodyGetUserData(body);
	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	
	if(info->isInCar == false){
	// rotate the force direction to align with the camera
	if(info->isMoving == true){    	
	  //slow down for turning
	  if(info->isTurning){
       NewtonBodyGetVelocity(body, (FLOAT*)velocity);
	   velocity.operator /=(10.15f);
	   NewtonBodySetVelocity(body, (FLOAT*)velocity);
	  }

	  heading = info->facing * 100;// * 600;
	  force = D3DXVECTOR3(heading.x,0,heading.z);
	  NewtonBodySetVelocity(body, (FLOAT *)force);
	}
	else{
	  NewtonBodySetVelocity(body, (FLOAT *)zero);
	  NewtonBodySetOmega(body, (FLOAT *)zero);
	}
		

	//add gravity force
	force = D3DXVECTOR3(0.0f, -mass * 500.0f, 0.0f);
	NewtonBodySetForce(body, (FLOAT *)force);
	}
	else{//player is in car
      force = carManager->car->getCarInfo()->velocity;
	  force.operator *(60);
	  NewtonBodySetVelocity(body, (FLOAT *)force);	 
	}

	//update player info
	D3DXMATRIX location;
	NewtonBodyGetMatrix(body, (FLOAT *)location);
	D3DXMATRIX newLocation;D3DXMatrixIdentity(&newLocation);
	newLocation._41 = location._41;
	newLocation._42 = location._42;
	newLocation._43 = location._43;
	info->pos = location;
	NewtonBodyGetVelocity(body, info->velocity);
	

	 
}


// add force and torque to rigid body
void _cdecl NewtonObjects::PhysicsApplyForceAndTorque (const NewtonBody* body)
{
	static float speed = 50.0f;
	float mass;
	float Ixx;
	float Iyy;
	float Izz;
    
	NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz);
	D3DXVECTOR3 force(0.0f, -mass * speed, 0.0f);
	NewtonBodySetForce (body, (FLOAT *)force);
	
}

// set the tranformation of a rigid body
void _cdecl NewtonObjects::PhysicsSetTransform (const NewtonBody* body, const float* matrix)
{
    playerInfo *info = (playerInfo*)NewtonBodyGetUserData(body);	
    float* tempMatrix = (float *)matrix;
    if(info->isInCar)
      tempMatrix[1] += 50.0f;
    // set the transformation matrix for this rigid body
	NewtonBodySetMatrix(body, tempMatrix);
}


// rigid body destructor
void _cdecl NewtonObjects::PhysicsBodyDestructor (const NewtonBody* body)
{
	CDXUTMesh* primitive;

	// get the graphic object form the rigid body
	primitive = (CDXUTMesh*) NewtonBodyGetUserData (body);

	// destroy the graphic object
	delete primitive;
}

void NewtonObjects::readInLevel()
{
	
  NewtonCollision *nCollision;
  LPD3DXMESH level = collisionMesh->GetSysMemMesh();
  LPDIRECT3DVERTEXBUFFER9 VBuffer;
  LPDIRECT3DINDEXBUFFER9 IBuffer;
  float *pVertices;
  WORD *pIndices;
 
  //load level
  nCollision = NewtonCreateTreeCollision(nWorld, NULL);
  NewtonTreeCollisionBeginBuild(nCollision);
  level->GetVertexBuffer(&VBuffer);
  level->GetIndexBuffer(&IBuffer);
  VBuffer->Lock(0, 0, (void**)&pVertices, 0);
  IBuffer->Lock(0, 0, (void**)&pIndices, D3DLOCK_READONLY);
   
  for(unsigned int i=0; i < level->GetNumFaces()*3; i=i+3){
	 
	  float verts[9];
	  int index=0;
		  
	  index = pIndices[i];
 	  verts[0] = pVertices[3*index];
      verts[1] = pVertices[3*index+1];
      verts[2] = pVertices[3*index+2];
	  index = pIndices[i+1];
	  verts[3] = pVertices[3*index];
      verts[4] = pVertices[3*index+1];
      verts[5] = pVertices[3*index+2];	
	  index = pIndices[i+2];
	  verts[6] = pVertices[3*index];
      verts[7] = pVertices[3*index+1];
      verts[8] = pVertices[3*index+2];	
	  	  
      NewtonTreeCollisionAddFace(nCollision, 3, (const float*)verts, 12, 0); 
  }

  NewtonTreeCollisionEndBuild(nCollision, 1);
  VBuffer->Unlock();
  IBuffer->Unlock();

  VBuffer->Release();
  IBuffer->Release();
 
  // create the level body
  levelBody = NewtonCreateBody(nWorld, nCollision);
  NewtonBodySetMaterialGroupID (levelBody, levelID);
  NewtonBodySetMatrix (levelBody, (FLOAT *)matWorld);
  NewtonReleaseCollision (nWorld, nCollision);
}

void NewtonObjects::initDeviceObjects(IDirect3DDevice9 *device)
{
  pd3dDevice = device;
  collisionMesh->Create(pd3dDevice, L"meshes/collision.x");
  carManager->InitDeviceObjects(device);
  initNewton();
   
  
}

void NewtonObjects::restoreDeviceObjects()
{
  collisionMesh->RestoreDeviceObjects(pd3dDevice);
  carManager->RestoreDeviceObjects(pd3dDevice);
}

void NewtonObjects::invalidateDeviceObjects()
{
  collisionMesh->InvalidateDeviceObjects();
  carManager->invalidateDeviceObjects();
}

void NewtonObjects::destroyDeviceObjects()
{
  collisionMesh->Destroy();
  carManager->destroyDeviceObjects();
  
}