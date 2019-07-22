#include "dxstdafx.h"
#include ".\interior.h"
#include "Player.h"
#include "PrintText.h"
#include "NewtonObjects.h"
#include "Materials.h"
#include "NPCManager.h"


Interior::Interior(int id, D3DXVECTOR3 entry, char* n, LPCWSTR m, LPCWSTR c)
{
  entryPoint = entry;
  name = n;
  meshFile = m;
  interiorID = id;
  collisionFile = c;
  numNpcs = 0;

  mesh = new CDXUTMesh();
  collisionMesh = new CDXUTMesh();
  isInhabitedByPlayer = isInRangeOfPlayer = false;

}

void Interior::addNpc(Npc *npc)
{
  characters[numNpcs] = npc;
  numNpcs++;

}

float Interior::getDistanceFromPlayer()
{
  D3DXVECTOR3 playerPos(Player::sPlayerInfo->pos._41, Player::sPlayerInfo->pos._42, Player::sPlayerInfo->pos._43);
  D3DXVECTOR3 distance = playerPos.operator -(entryPoint);
  return D3DXVec3Length(&distance);
}

void Interior::processTalkRequest()
{
  float min = characters[0]->getDistanceFromPlayer();
  int index = 0;  //index of NPC player is talking to
 
  for(int i = 1; i < numNpcs; i++)
  {
     float distance = characters[i]->getDistanceFromPlayer();
     if(min > distance){
       min = distance;
	   index = i;
     } 
  }
  
  if(min < 20.0f)
    characters[index]->talk();
  
}

void Interior::processEnterRequestByPlayer()
{
  isInhabitedByPlayer = !isInhabitedByPlayer;  
  Player::sPlayerInfo->isInInterior = !Player::sPlayerInfo->isInInterior;
  
  //player is inside
  if(isInhabitedByPlayer){
    NewtonObjects::isWorldCollidable = false;
    NewtonMaterialSetDefaultCollidable(NewtonObjects::nWorld, characterID, worldInteriorID, 1);
    Player::sPlayerInfo->interiorID = interiorID;
  }
  else{	//player is outside
    NewtonObjects::isWorldCollidable = true;
    NewtonMaterialSetDefaultCollidable(NewtonObjects::nWorld, characterID, worldInteriorID, 0);
    Player::sPlayerInfo->interiorID = -1;
  }
  
}

void Interior::frameMove(float time, IDirect3DDevice9 *device)
{
  //test if player in range of door
  getDistanceFromPlayer() < 30.0f ? isInRangeOfPlayer = true : isInRangeOfPlayer = false;
  
  if(isInhabitedByPlayer){
    D3DXVECTOR3 playerPos(Player::sPlayerInfo->pos._41, Player::sPlayerInfo->pos._42, Player::sPlayerInfo->pos._43);
    for(int i = 0; i < numNpcs; i++)
      characters[i]->FrameMove(NPCManager::entities[characters[i]->getEntityType()], time, device, playerPos);
  }
}

//return true if the interior is being rendered, false if not
bool Interior::render(IDirect3DDevice9 *device)
{
  if(isInhabitedByPlayer){
   mesh->Render(device);
   for(int i = 0; i < numNpcs; i++)
     characters[i]->render(device, NPCManager::entities[characters[i]->getEntityType()]);

   //PrintText::printInt(Player::sPlayerInfo->interiorID, 10, 10);
  }


  if(isInRangeOfPlayer){
   if(isInhabitedByPlayer)
     PrintText::printString("Press 'U' to exit ", 400, 600);
   else
     PrintText::printString("Press 'U' to enter ", 400, 600);

   PrintText::printString(name, 540, 600);
  }
  
  

  if(isInhabitedByPlayer)
   return true;
  return false;
}

void Interior::InitDeviceObjects(IDirect3DDevice9 *device)
{
  mesh->Create(device, meshFile);
  collisionMesh->Create(device, collisionFile);

  readInCollisionGeometry();
  NewtonMaterialSetDefaultCollidable(NewtonObjects::nWorld, characterID, interiorID, 0);
}

void Interior::invalidateDeviceObjects()
{
  mesh->InvalidateDeviceObjects();
  collisionMesh->InvalidateDeviceObjects();

}

void Interior::destroyDeviceObjects()
{
  mesh->Destroy();
  collisionMesh->Destroy();

}

void Interior::RestoreDeviceObjects(IDirect3DDevice9 *device)
{
  mesh->RestoreDeviceObjects(device);
  collisionMesh->RestoreDeviceObjects(device);

}

void Interior::readInCollisionGeometry()
{
  NewtonCollision *nCollision;
  LPD3DXMESH level = collisionMesh->GetSysMemMesh();
  LPDIRECT3DVERTEXBUFFER9 VBuffer;
  LPDIRECT3DINDEXBUFFER9 IBuffer;
  float *pVertices;
  WORD *pIndices;
 
  //load collision geometry
  nCollision = NewtonCreateTreeCollision(NewtonObjects::nWorld, NULL);
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
  levelBody = NewtonCreateBody(NewtonObjects::nWorld, nCollision);
  NewtonBodySetMaterialGroupID (levelBody, worldInteriorID);
  D3DXMATRIX matWorld; D3DXMatrixIdentity(&matWorld);
  NewtonBodySetMatrix (levelBody, (FLOAT *)matWorld);
  NewtonReleaseCollision (NewtonObjects::nWorld, nCollision);
  NewtonMaterialSetDefaultCollidable(NewtonObjects::nWorld, characterID, worldInteriorID, 0);

}