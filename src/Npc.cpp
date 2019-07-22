#include "dxstdafx.h"
#include ".\npc.h"
#include "PrintText.h"
#include "player.h"
#include "NewtonObjects.h"
#include "collision.h"
#include "OcclusionCuller.h"
#include "NPCManager.h"

#define WALK_SPEED 0.15f
#define IDLE_SPEED 1.0f
#define WALK_ID 1
#define IDLE_ID 0


Npc::Npc(EntityType t, int i, Status s)
{
 numNodes = 0;
 currNode = 0;

 m_currentAnimation = -1;
 state = s;
 isBlocked = repressBlockingSound = false;
 loopPatrolNodes = true;
 followDistance = 35.0f; 

 type = t;
 index = i;
 length = XLength = ZLength = ratio = distanceFromMesh = 0.0f;
 ZValue = 1.0f;

 boundingBox = new sBoundingBox(type);
}

//called only by missionNPCs to read in their dialogue
void Npc::readInNpcText()
{
   inFile = fopen("text/NPCText.txt", "r");

   //scan down the file until you get to the start of your NPCs text
   char npcID[100] = "id"; //needs newline char
   char *temp = (char *)malloc(500);
   itoa((int)type, temp, 10);
   strcat(npcID, temp);
   strcat(npcID, "\n");
   char s[500];
   while( strcmp(npcID, s) != 0 )
     fgets(s, 10, inFile);
   //read in how many lines of dialogue there are for the mission
   int nTexts = 0;
   fscanf(inFile, "%d ", &nTexts);
   

   //allocate memory for each individual dialogue
   for(int i = 0; i < nTexts; i++)
     text[i] = (char *)malloc(1000);
   
   //read in succesive dialogues for the mission
   for(int i = 0; i < nTexts; i++)
     fgets(text[i], 1000, inFile);
   
   
   fclose(inFile);
}

void Npc::addNode(float x, float y, float z)
{
  nodes[numNodes].pos = D3DXVECTOR3(x, y, z);
  numNodes++;
}

void Npc::finishAddingNodes()
{
  pos = nodes[0].pos;		  //start postition is the first node
  destination = nodes[1].pos; //destination is the second node
  
  
  calcXZValues();
}

void Npc::talk()
{
  if(state != FOLLOW && state != FOLLOW_IDLE){
    if(state != STATIONARY)
     state = IDLE;
    SoundManager::playNPCConversation();
  }
}

BOOL Npc::isInVeiwingFrustum(cFrustum *frustum)
{
  if( frustum->CheckSphere(pos.x, pos.y, pos.z, 10) == FALSE)
    return FALSE;
  else{
   if(NPCManager::occCulling == false) 
     return TRUE;
   else
    return !OcclusionCuller::isObjectOccluded(pos, boundingBox);     
  }
}

BOOL Npc::canSeePlayer()
{
  DWORD faceIndex; float pU, pV; D3DXVECTOR3 p(pos.x, pos.y + 15, pos.z);
  D3DXIntersect(NewtonObjects::collisionMesh->GetLocalMesh(), &p, &facing, &isIntersectingMesh,
                &faceIndex, &pU, &pV, &distanceFromMesh, NULL, NULL);				


  D3DXMATRIX matPos = Player::sPlayerInfo->pos;
  D3DXVECTOR3 playerPos(matPos._41, matPos._42, matPos._43);
    
  D3DXVECTOR3 vMin = playerPos.operator -(D3DXVECTOR3(40, 30, 40));
  D3DXVECTOR3 vMax = playerPos.operator +(D3DXVECTOR3(40, 30, 40));
  seePlayer = D3DXBoxBoundProbe(&vMin, &vMax, &pos, &facing);
  if(seePlayer && distance > 200.0f) seePlayer = false;

  if(seePlayer && isIntersectingMesh)
  {
    if(distanceFromMesh < distance)
      seePlayer = false;
  }

  return seePlayer;
}

void Npc::doFollow(D3DXVECTOR3 playerPos)
{
  D3DXVECTOR3 diff = playerPos.operator -(pos);
  
  float m = D3DXVec3Length(&diff);
  if( m > followDistance ){  
      state = FOLLOW;
      D3DXVec3Lerp(&pos, &pos, &playerPos, 0.01f);
	  
  }
  else{
     state = FOLLOW_IDLE; 
  }
}

void Npc::calcXZValues()
{
   //calculate x,z movement values
   D3DXVECTOR3 d = destination.operator -(pos);
   length =  D3DXVec3Length(&d);
   ZValue = 1.0f;
   XLength = abs(d.x);
   ZLength = abs(d.z);
   ratio = XLength / ZLength;
   while(ratio > 1.0f){
     ratio = ratio / 2.0f;
     ZValue = ZValue / 2.0f;
   }
}


void Npc::FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *device, D3DXVECTOR3 playerPos)
{
  float speed = 0.5f;
  D3DXVECTOR3 diff = destination.operator -(pos);
  D3DXVec3Normalize(&facing, &diff);  
     
  if(state == WALKING){
    float m = D3DXVec3Length(&diff);
    if( m > 1.0f ){
	  if(destination.x > pos.x)
        pos.x += (ratio * speed);
      else if(destination.x < pos.x)
		pos.x -= (ratio * speed);
	  
	  if(destination.z > pos.z)
        pos.z += (ZValue * speed);
      else if(destination.z < pos.z)
		pos.z -= (ZValue * speed);
    }
    else{	//reached destination, switch to next node
      if(currNode == numNodes - 1){//reached last node
        if(loopPatrolNodes) 
          currNode = -1;
        else{
          state = STATIONARY;
        }
	  }
      else{ //not last node, switch to next node
        currNode++;
        destination = nodes[currNode].pos;
	 
        calcXZValues();
      }
    }
  }
  else if(state == STATIONARY){

  }
  else{//blocked, idle, or follow
   if(state == BLOCKED && repressBlockingSound == false)
    SoundManager::playNPCSound();
   if(state == IDLE && SoundManager::isPlayingConversation == false)
     state = WALKING;
   if(state == FOLLOW || state == FOLLOW_IDLE)
    doFollow(playerPos);
  }
  
  //test if blocked
  isBlocked = D3DXSphereBoundProbe(&playerPos, 6.0f, &pos, &facing);
  D3DXVECTOR3 vecDistance = playerPos.operator -(pos);
  distance = D3DXVec3Length(&vecDistance);
  if(state != STATIONARY){
    if(isBlocked == TRUE && distance < 15 && state != FOLLOW)
	  state = BLOCKED;
    else{
      if(state == BLOCKED && state != STATIONARY)
        state = WALKING;
	  isBlocked = FALSE;
    } 
  }

  //test if NPC can see player
  canSeePlayer();
  
  
  //set animation based on status
  updateAnimationStatus(entity);

  //set transform matrix
  D3DXMATRIX matTrans, matRot, matWorld;
  D3DXMatrixIdentity(&matWorld);D3DXMatrixIdentity(&matRot);D3DXMatrixIdentity(&matWorld);
  
  //get height of the ground below the NPC
  float y = GetHeightBelow(NewtonObjects::collisionMesh->GetLocalMesh(), pos.x, 10, pos.z);
  D3DXMatrixTranslation(&matTrans, pos.x, y, pos.z);
  calcAngle(playerPos);
  D3DXMatrixRotationY(&matRot, angle);
  D3DXMatrixMultiply(&matWorld, &matWorld, &matRot);
  D3DXMatrixMultiply(&matWorld, &matWorld, &matTrans);
  entity->FrameMove(m_fElapsedTime,matWorld);
    
}

void Npc::updateAnimationStatus(CXfileEntity *entity)
{
  if(state == WALKING || state == FOLLOW){
	if(m_currentAnimation != WALK_ID){
	 setAnimationStatus(entity, WALK_ID);
     entity->setAnimationSpeed(WALK_SPEED);
    }
  }
  else {
    if(m_currentAnimation != IDLE_ID){
     setAnimationStatus(entity, IDLE_ID);
	 entity->setAnimationSpeed(IDLE_SPEED);
    }
  }  
}

void Npc::render(IDirect3DDevice9 *device, CXfileEntity *entity)
{
  entity->Render(device, NPC_CODE, NULL);
  /*ID3DXMesh *box;
  D3DXCreateBox(device, boundingBox->width, boundingBox->height, boundingBox->length, &box, NULL);
  
  //render bounding box
  D3DXMATRIX mTrans;D3DXMatrixIdentity(&mTrans);
  D3DXMatrixTranslation(&mTrans, pos.x, pos.y + (boundingBox->height / 2), pos.z);
  device->SetTransform(D3DTS_WORLD, &mTrans);
  box->DrawSubset(0);
  box->Release();*/
  
}

void Npc::calcAngle(D3DXVECTOR3 playerPos)
{
  D3DXVECTOR2 v1(pos.x, pos.z);
  D3DXVECTOR2 v2(destination.x, destination.z);
  if(state == IDLE || state == FOLLOW || state == FOLLOW_IDLE || state == STATIONARY){
    v1 = D3DXVECTOR2(pos.x, pos.z);
    v2 = D3DXVECTOR2(playerPos.x, playerPos.z);
  }
  
  float deltaY, deltaX;
  if(v2.y == v1.y)
   deltaY = (v2.y - v1.y);
  else
   deltaY = (v1.y - v2.y);

  if(v2.x == v2.x)
   deltaX = (v2.x - v1.x);
  else
   deltaX = (v1.x - v2.x);

  angle  = atan2(deltaY ,deltaX);

}


void Npc::setAnimationStatus(CXfileEntity *entity, int n)
{
  entity->SetAnimationSet(n);
  m_currentAnimation = n;
}

void Npc::initDeviceObjects(IDirect3DDevice9* device){}
void Npc::invalidateDeviceObjects(){}