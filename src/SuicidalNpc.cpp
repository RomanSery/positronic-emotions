#include "dxstdafx.h"
#include ".\SuicidalNpc.h"
#include "OcclusionCuller.h"

#define WALK_SPEED 0.15f
#define IDLE_SPEED 1.0f

SuicidalNpc::SuicidalNpc(EntityType t, D3DXVECTOR3 tPt, D3DXVECTOR3 initial)
 : Npc(t)
{
  state = IDLE;
  //type = t;
  
  pos = D3DXVECTOR3(initial.x, initial.y, initial.z);
  triggerPoint = D3DXVECTOR3(tPt.x, tPt.y, tPt.z);
  triggered = false;
  animationOver = false;
  isAnimStarted = false;
  time = 0.0;
}


void SuicidalNpc::FrameMove(CXfileEntity *entity, float m_fElapsedTime, IDirect3DDevice9 *device, D3DXVECTOR3 playerPos)
{
  if(isAnimStarted == false && triggered == true){
   entity->SetAnimationSet(0);
   entity->setAnimationSpeed(1.0f);
   isAnimStarted = true;
  }
  
  //determine if the player triggered the animation
  if(triggered == false){
   D3DXVECTOR3 diff = playerPos.operator -(triggerPoint);
   float m = max(abs(diff.x), abs(diff.z));
   if(m < 300) triggered = true;
  }


  if(triggered && animationOver == false){
   time = time + DXUTGetElapsedTime();
   if(time > 6.700) animationOver = true;
  }
    
  
  //set transform matrix and animate if not already finished
  if(animationOver == false){
    D3DXMATRIX matWorld; D3DXMatrixIdentity(&matWorld);
    entity->FrameMove(m_fElapsedTime,matWorld);
  }

}

float SuicidalNpc::getDistanceFromPlayer()
{
  D3DXVECTOR3 playerPos(Player::sPlayerInfo->pos._41, Player::sPlayerInfo->pos._42, Player::sPlayerInfo->pos._43);
  return D3DXVec3Length(&playerPos.operator -(pos));

}

BOOL SuicidalNpc::isInVeiwingFrustum(cFrustum* frustum)
{
 //check the landing point if the animation is over
 if(animationOver){
   if( frustum->CheckSphere(triggerPoint.x, triggerPoint.y, triggerPoint.z, 10) == false)
     return FALSE;
   else
     return !OcclusionCuller::isObjectOccluded(triggerPoint, boundingBox);
 }
 //check the initial position if animation not started yet
 if(triggered == false){
   if( frustum->CheckSphere(pos.x, pos.y, pos.z, 10) == false)
     return FALSE;
   else
     return !OcclusionCuller::isObjectOccluded(pos, boundingBox);
 }

 //always return true if NPC is in flight
 return TRUE;
 
 
}

void SuicidalNpc::talk(){}

void SuicidalNpc::render(IDirect3DDevice9 *device, CXfileEntity *entity)
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
  /*ID3DXMesh *sphere;
  D3DXCreateSphere(device, 10, 10, 10, &sphere, NULL);
  D3DXMATRIX mat; D3DXMatrixIdentity(&mat);
  D3DXMatrixTranslation(&mat, triggerPoint.x,triggerPoint.y,triggerPoint.z);
  //device->SetTransform(D3DTS_WORLD, &mat);
  //sphere->DrawSubset(0);
  sphere->Release();  */
}
